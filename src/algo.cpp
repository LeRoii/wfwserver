#include <vector>
#include "algo.h"

std::vector<StMapMarkerInfo> m_markers;

void AnalysisAlgo(std::vector<StMapMarkerInfo>& markers, std::vector<StTsAnaResultData>& result)
{
    
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

    output.data = ResultData;
    output.i32Type = 0;
    output.api = "mapPlugin/ai";
}

// void AnalysisForceDeployment();
// void AnalysisFirepowerCoverage();
// void AnalysisTS();

void Ts_Anals(StTsAnaInput& input, StTsAnaResultOutput& output)
{
    output.i32Type = 1;
    output.api = "mapPlugin/ai";

    // switch(input.data.i32Type)
    // {
    //     //1.兵力部署
    //     case 1:
    //         AnalysisForceDeployment();
    //         break;
    //     //2.火力覆盖范围分析
    //     case 2:
    //         AnalysisFirepowerCoverage();
    //         break;
    //     //3.态势预测
    //     case 3:
    //         AnalysisTS();
    //         break;
    //     default:
    //         break;
    // }


    for(auto& marker:m_markers)
    {
        StTsAnaResultData AnaResultData;
        AnaResultData.longitude;
        AnaResultData.latitude;
        AnaResultData.i32Type;
        AnaResultData.angle;

        std::vector<StMapMarkerInfo> LineMarkers;
        std::vector<StMapMarkerInfo> IntentMarkers;
        std::vector<StMapMarkerInfo> ShapeMarkers;

        //意图预测
        StMapMarkerInfo IntentMarker;
        IntentMarker.intent = "";
        IntentMarkers.push_back(IntentMarker);

        //轨迹预测

        //火力覆盖范围预测

        AnaResultData.line = LineMarkers;
        AnaResultData.intent = IntentMarkers;
        AnaResultData.shape = ShapeMarkers;

        output.data.push_back(AnaResultData);
    }
}