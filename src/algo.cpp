#include "spdlog/spdlog.h"
#include <vector>
#include "algo.h"
#include "proj_main.hpp"

std::vector<StMapMarkerInfo> m_markers;

static void printMarkerInfo()
{
    for(int i=0;i<m_markers.size();i++)
    {
        spdlog::debug("marker idx:[{}]", i);
        printf("timestampAndUserId:%s, longitude:%f, latitude:%f, markerUrl:%s, jbMarkerCode:%d, addMarkerTime:%d, delMarkerTime:%d\n",
        m_markers[i].timestampAndUserId.c_str(), m_markers[i].longitude, m_markers[i].latitude,
        m_markers[i].markerUrl.c_str(), m_markers[i].jbMarkerCode, m_markers[i].addMarkerTime,
        m_markers[i].delMarkerTime);

    }
}

void AnalysisAlgo(std::vector<StMapMarkerInfo>& markers, std::vector<StTsAnaResultData>& result)
{
    double form_group_thread = 100.0;
    float iter_step = 0.1;
    float predict_step = 5;
    Eigen::MatrixXf enemy_marker_info = Eigen::MatrixXf(markers.size(), 2);
    Eigen::Vector2f enemy_pos1;
    enemy_pos1 << 0, 0;
    StTsAnaResultData temp_data;

    for (int i = 0; i < markers.size(); i++) {
        enemy_marker_info(i,0) = markers[i].latitude;
        enemy_marker_info(i,1) = markers[i].longitude;
        temp_data.latitude = markers[i].latitude;
        temp_data.longitude = markers[i].longitude;
        result.push_back(temp_data);
    }
    for (int i = 0; i < markers.size(); i++) {
        enemy_pos1(0) = enemy_marker_info(i,0);
        enemy_pos1(1) = enemy_marker_info(i,1);
        Eigen::VectorXf result_vector = math_tool.get_min_info(enemy_pos1, enemy_marker_info);
        if (result_vector(1) < form_group_thread) {
            StMapMarkerInfo group;
            group.latitude = enemy_marker_info(result_vector(0), 0);
            group.longitude = enemy_marker_info(result_vector(0), 1);
            result[i].force.push_back(group);
        }
    }

    Eigen::MatrixXd Q(4,4);
    Q << 0.1, 0, 0, 0,
        0, 0.1, 0, 0,
        0, 0, 0.1, 0,
        0, 0, 0, 0.1;
    Eigen::MatrixXd R(2, 2);
    R << 0.1, 0,
        0, 0.1;
    Eigen::VectorXd x_init = Eigen::VectorXd::Zero(4);
    Eigen::VectorXd u = Eigen::VectorXd::Zero(2);
    Eigen::VectorXd x_predict, x_update;
    Eigen::VectorXd z_k(2);
    std::vector<int> enemy_intention;
    for (int i = 0; i < markers.size(); i++) {

        auto kf = std::make_shared<proj_kalman::kf_kalman>(4, 2, 0, Q, R);
        x_init << enemy_marker_info(i,0), enemy_marker_info(i,1), 0, 0;
        kf->init(x_init);
        StMapMarkerInfo predict_point;
        for (int j = 0; j < markers[i].pos.size() + predict_step; j++) {
            x_predict = kf->predict(u, iter_step);
            if (j >= markers[i].pos.size()) {
                z_k << x_predict(0), x_predict(1);
                x_update = kf->update(z_k);
                predict_point.latitude = x_update(0);
                predict_point.longitude = x_update(1);
                // result[i].line[0].pos.push_back(std::make_pair(x_update(0), x_update(1)));
                result[i].line.push_back(predict_point);
            }
            else {
                z_k << enemy_marker_info(i,0), enemy_marker_info(i,1);
                x_update = kf->update(z_k);
            }
        }

        StMapMarkerInfo intention_point;
        auto intention = std::make_shared<proj_intention::EnemyIntention>(iter_step, markers[i].pos.size());
        std::vector<float> enemy_posx, enemy_posy;
        Eigen::Vector2f target(2);
        target << 0, 0;
        for (int j = 0; j < markers[i].pos.size(); j++) {
            enemy_posx.push_back(markers[i].pos[j].first);
            enemy_posy.push_back(markers[i].pos[j].second);
        }
        int single_inten = intention->single_intention(enemy_posx, enemy_posy, target);
        intention_point.intent = intention_map.at(single_inten);
        result[i].intent.push_back(intention_point);
        enemy_intention.push_back(single_inten);
        std::cout << "single enemy_intention:" << intention_point.intent << std::endl;
    }

    auto threat = std::make_shared<proj_threat::TopsisThreat>();
    Eigen::MatrixXf enemy_pos = Eigen::MatrixXf::Zero(markers.size(),2);
    std::vector<int> enemy_info;
    std::vector<float> enemy_vel;
    for (int i = 0; i < markers.size(); i++) {
        enemy_info.push_back(enemyid_map.at(markers[i].detailInfo.targetType));
        enemy_pos(i,0) = markers[i].latitude;
        enemy_pos(i,1) = markers[i].longitude;
        double vel = sqrt(pow((markers[i].pos[-1].first - markers[i].pos[-2].first) / iter_step, 2) + 
            pow((markers[i].pos[-1].second - markers[i].pos[-2].second) / iter_step, 2));
        enemy_vel.push_back(vel);
    }
    Eigen::Vector2f target(2);
    target << 0, 0;
    Eigen::VectorXf enemy_threat = Eigen::VectorXf::Zero(2);
    enemy_threat = threat->danger_evaluate(enemy_info, enemy_pos, enemy_vel, enemy_intention, target);
    std::cout << "enemy_threat:" << enemy_threat << std::endl;

}

static int Is_In_List(StMapMarkerInfo &target)
{
    if(m_markers.empty())
        return -1;

    for(int i=0;i<m_markers.size();i++)
    {
        if(m_markers[i].timestampAndUserId.compare(target.timestampAndUserId) == 0)
            return i;
    }

    return -1;
}

void Ts_Fus(StTsFusInput& input, StTsFusResultOutput& output)
{
    spdlog::debug("Ts_Fus start");
    std::vector<StTsFusResultData> ResultData;
    for(auto &dataElement:input.data)
    {
        StTsFusResultData ResultDataElement;
        ResultDataElement.timestampAndUserId = dataElement.MapMarker.timestampAndUserId;
        ResultDataElement.i32Type = dataElement.i32Type;
        ResultDataElement.markerInfo = dataElement.MapMarker;
        ResultDataElement.aiSource = "zx";

        if(dataElement.i32Type == 0)//add
        {
            if(Is_In_List(dataElement.MapMarker) == -1)//is not in list
            {
                m_markers.push_back(dataElement.MapMarker);

                ResultDataElement.i32Res = 0;
            }
            else // is in list
            {
                ResultDataElement.i32Res = 1;
            }
        }
        else if(dataElement.i32Type == 1)//del
        {
            int idx = Is_In_List(dataElement.MapMarker);
            idx == -1 ? ResultDataElement.i32Res = 1 : ResultDataElement.i32Res = 0;
            if(idx != -1)
            {
                m_markers.erase(m_markers.begin() + idx);
            }
        }
        else if(dataElement.i32Type == 2)//modify
        {
            int idx = Is_In_List(dataElement.MapMarker);
            idx == -1 ? ResultDataElement.i32Res = 1 : ResultDataElement.i32Res = 0;
            if(idx != -1)
            {
                m_markers[idx] = dataElement.MapMarker;
            }
        }

        ResultData.push_back(ResultDataElement);
    }

    for(auto &dataElement:input.data)
    {
        int idx = Is_In_List(dataElement.MapMarker);
        if(idx != -1)
        {
            m_markers[idx].pos.push_back(std::pair<double, double>(dataElement.MapMarker.longitude, dataElement.MapMarker.latitude));
        }
    }

    //debug
    for(auto& marker:m_markers)
    {
        spdlog::debug("marker timestampAndUserId:{}, logitude:{}, latitude:{}, pos size:{}", marker.timestampAndUserId, 
        marker.longitude, marker.latitude, marker.pos.size());
    }

    output.data = ResultData;
    output.i32Type = 0;
    output.api = "mapPlugin/ai";
}


void Ts_Anals(StTsAnaInput& input, StTsAnaResultOutput& output)
{
    spdlog::debug("Ts_Anals start, type:{}", input.data.i32Type);
    output.i32Type = 1;
    output.api = "mapPlugin/ai";

    printMarkerInfo();

    AnalysisAlgo(m_markers, output.data);
    // for(auto& dataElement:output.data)
    // {
    //     dataElement.i32Type = input.data.i32Type;

    // }

    for(int i=0;i<output.data.size();i++)
    {
        output.data[i].i32Type = input.data.i32Type;
        spdlog::debug("marker [{}],mv dir:", i);
        printf("longtitude:%f, latitude:%f, angle:%f\n", 
        output.data[i].longitude, output.data[i].latitude, output.data[i].angle);
        printf("line size:%d\n", output.data[i].line.size());
        printf("intent size:%d, intent:%s\n", output.data[i].intent.size(), output.data[i].intent.back().intent.c_str());
        printf("shape size:%d\n", output.data[i].shape.size());
        printf("force size:%d\n", output.data[i].force.size());

    }
}

void Threat_Anals(StThreatResultOutput& output)
{
    output.i32Type = 3;
    output.api = "mapPlugin/ai";

    for(int i=0;i<m_markers.size();i++)
    {
        StThreatResultData DataElement;
        DataElement.id = m_markers[i].timestampAndUserId;
        DataElement.targetName = m_markers[i].detailInfo.targetName;
        DataElement.grade = m_markers[i].grade;

        output.data.push_back(DataElement);
    }
}