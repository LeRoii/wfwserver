#include "topsis_threat.hpp"

namespace proj_threat
{
Eigen::MatrixXf TopsisThreat::topsis_matrix(std::vector<int> enemy_info, 
        Eigen::MatrixXf enemy_pos, std::vector<float> enemy_vel, 
        std::vector<int> enemy_intention,
        Eigen::Vector2f target) {
    Eigen::MatrixXf topsis_matrix_ = Eigen::MatrixXf::Zero(enemy_info.size(), 3);

    for (int i = 0; i < enemy_info.size(); i++) {
        float intention_class = intention_dict[enemy_intention[i]];

        if(intention_class < 0)
            enemy_vel[i] = -1 * enemy_vel[i];
        Eigen::Vector2f current_pos(enemy_pos(i,0), enemy_pos(i,1));
        double distance = math_tool.get_distance(current_pos, target, "Euclidean");
        double damage_dis = distance - enemy_info_[enemy_info[i]][2] -  enemy_info_[enemy_info[i]][3];
        damage_dis = distance;

        if (damage_dis <= 0)
            damage_dis = pow(10,-5);

        topsis_matrix_(i,0) = intention_class;
        topsis_matrix_(i,1) = damage_dis;
        topsis_matrix_(i,2) = enemy_vel[i];
    }
    float max_array[3] = {topsis_matrix_.colwise().maxCoeff()[0], 
                            topsis_matrix_.colwise().maxCoeff()[1], topsis_matrix_.colwise().maxCoeff()[2]};
    for (int i = 0; i < enemy_info.size(); i++) {
        topsis_matrix_(i,0) = max_array[0] - topsis_matrix_(i,0);
        if (topsis_matrix_(i,0) == 0) {
            topsis_matrix_(i,0) = pow(10,-5);
        }
        topsis_matrix_(i,2) = max_array[2] - topsis_matrix_(i,2);
        if (topsis_matrix_(i,2) == 0) {
            topsis_matrix_(i,2) = pow(10,-5);
        } 
    }

    return topsis_matrix_;
}

Eigen::VectorXf TopsisThreat::get_weight(std::vector<int> enemy_info, 
        Eigen::MatrixXf enemy_pos, std::vector<float> enemy_vel, 
        std::vector<int> enemy_intention,
        Eigen::Vector2f target) {
    // Eigen::MatrixXf topsis_matrix_ = 
    //     topsis_matrix(enemy_info, enemy_pos, enemy_vel, enemy_intention, target);

    // Eigen::MatrixXf p_matrix = topsis_matrix_;
    // float entropy_array[3] = {0, 0, 0};
    // Eigen::MatrixXf norm_matrix = topsis_matrix_;
    // for (int i = 0; i < enemy_info.size(); i++) {
    //     entropy_array[0] += topsis_matrix_(i,0) * topsis_matrix_(i,0);
    //     entropy_array[1] += topsis_matrix_(i,1) * topsis_matrix_(i,1);
    //     entropy_array[2] += topsis_matrix_(i,2) * topsis_matrix_(i,2);
    // }
    // for (int i = 0; i < enemy_info.size(); i++) {
    //     norm_matrix(i,0) = topsis_matrix_(i,0) / sqrt(entropy_array[0]);
    //     norm_matrix(i,1) = topsis_matrix_(i,1) / sqrt(entropy_array[1]);
    //     norm_matrix(i,2) = topsis_matrix_(i,2) / sqrt(entropy_array[2]);
    // }

    // for (int i = 0; i < enemy_info.size(); i++) {
    //     p_matrix(i,0) = norm_matrix(i,0) / norm_matrix.colwise().sum()[0];
    //     p_matrix(i,1) = norm_matrix(i,1) / norm_matrix.colwise().sum()[1];
    //     p_matrix(i,2) = norm_matrix(i,2) / norm_matrix.colwise().sum()[2];
    // }
    // float e_array[3] = {0};
    // float d_array[3] = {0};
    // for (int i = 0; i < enemy_info.size(); i++) {
    //     e_array[0] += p_matrix(i,0) * log(p_matrix(i,0));
    //     e_array[1] += p_matrix(i,1) * log(p_matrix(i,1));
    //     e_array[2] += p_matrix(i,2) * log(p_matrix(i,2));
    // }
    // for (int i = 0; i < 3; i++) {
    //     e_array[i] = -1 * 1 / log(enemy_info.size()) * e_array[i];
    //     d_array[i] = 1 - e_array[i];
    // }

    // Eigen::VectorXf weight(3);
    // weight << d_array[0] / (d_array[0] + d_array[1] + d_array[2]),
    //         d_array[1] / (d_array[0] + d_array[1] + d_array[2]),
    //         d_array[2] / (d_array[0] + d_array[1] + d_array[2]);

    // return weight;

}

Eigen::VectorXf TopsisThreat::get_weight(std::vector<int> enemy_info, Eigen::MatrixXf data_matrix) {
   
    Eigen::MatrixXf topsis_matrix_ = data_matrix;
    Eigen::MatrixXf p_matrix = topsis_matrix_;
    float entropy_array[3] = {0, 0, 0};
    Eigen::MatrixXf norm_matrix = topsis_matrix_;
    for (int i = 0; i < enemy_info.size(); i++) {
        entropy_array[0] += topsis_matrix_(i,0) * topsis_matrix_(i,0);
        entropy_array[1] += topsis_matrix_(i,1) * topsis_matrix_(i,1);
        entropy_array[2] += topsis_matrix_(i,2) * topsis_matrix_(i,2);
    }
    for (int i = 0; i < enemy_info.size(); i++) {
        norm_matrix(i,0) = topsis_matrix_(i,0) / sqrt(entropy_array[0]);
        norm_matrix(i,1) = topsis_matrix_(i,1) / sqrt(entropy_array[1]);
        norm_matrix(i,2) = topsis_matrix_(i,2) / sqrt(entropy_array[2]);
    }

    for (int i = 0; i < enemy_info.size(); i++) {
        p_matrix(i,0) = norm_matrix(i,0) / norm_matrix.colwise().sum()[0];
        p_matrix(i,1) = norm_matrix(i,1) / norm_matrix.colwise().sum()[1];
        p_matrix(i,2) = norm_matrix(i,2) / norm_matrix.colwise().sum()[2];
    }
    float e_array[3] = {0};
    float d_array[3] = {0};
    for (int i = 0; i < enemy_info.size(); i++) {
        e_array[0] += p_matrix(i,0) * log(p_matrix(i,0));
        e_array[1] += p_matrix(i,1) * log(p_matrix(i,1));
        e_array[2] += p_matrix(i,2) * log(p_matrix(i,2));
    }
    for (int i = 0; i < 3; i++) {
        e_array[i] = -1 * 1 / log(enemy_info.size()) * e_array[i];
        d_array[i] = 1 - e_array[i];
    }

    Eigen::VectorXf weight(3);
    weight << d_array[0] / (d_array[0] + d_array[1] + d_array[2]),
            d_array[1] / (d_array[0] + d_array[1] + d_array[2]),
            d_array[2] / (d_array[0] + d_array[1] + d_array[2]);

    return weight;

}

Eigen::VectorXf TopsisThreat::danger_evaluate(std::vector<int> enemy_info, 
        Eigen::MatrixXf enemy_pos, std::vector<float> enemy_vel, 
        std::vector<int> enemy_intention,
        Eigen::Vector2f target) {
    Eigen::MatrixXf topsis_matrix_ = 
        topsis_matrix(enemy_info, enemy_pos, enemy_vel, enemy_intention, target);

    Eigen::VectorXf weight = 
        get_weight(enemy_info, topsis_matrix_);

    Eigen::MatrixXf norm_matrix = topsis_matrix_;
    Eigen::MatrixXf final_matrix = topsis_matrix_;

    std::vector<float> score_vector;

    float topsis_array[3] = {0};

    for (int i = 0; i < enemy_info.size(); i++) {
        topsis_array[0] += topsis_matrix_(i,0) * topsis_matrix_(i,0);
        topsis_array[1] += topsis_matrix_(i,1) * topsis_matrix_(i,1);
        topsis_array[2] += topsis_matrix_(i,2) * topsis_matrix_(i,2);
    }
    for (int i = 0; i < enemy_info.size(); i++) {
        norm_matrix(i,0) = topsis_matrix_(i,0) / sqrt(topsis_array[0]);
        norm_matrix(i,1) = topsis_matrix_(i,1) / sqrt(topsis_array[1]);
        norm_matrix(i,2) = topsis_matrix_(i,2) / sqrt(topsis_array[2]);
    }

    Eigen::VectorXf max_array = norm_matrix.colwise().maxCoeff();
    Eigen::VectorXf min_array = norm_matrix.colwise().minCoeff();

    for (int i = 0; i < enemy_info.size(); i++) {
        final_matrix(i,0) = sqrt(weight(0,0) * pow(max_array[0] - norm_matrix(i,0),2) + 
                                weight(1,0) * pow(max_array[1] - norm_matrix(i,1),2) + 
                                weight(2,0) * pow(max_array[2] - norm_matrix(i,2),2));
        final_matrix(i,1) = sqrt(weight(0,0) * pow(min_array[0] - norm_matrix(i,0),2) + 
                                weight(1,0) * pow(min_array[1] - norm_matrix(i,1),2) + 
                                weight(2,0) * pow(min_array[2] - norm_matrix(i,2),2));
        final_matrix(i,2) = 1 - final_matrix(i,1) / (final_matrix(i,0) + final_matrix(i,1));
        score_vector.push_back(final_matrix(i,2));
    }

    Eigen::VectorXf score_result(enemy_info.size(), 1);
    for (int i = 0; i < enemy_info.size(); i++) {
        score_result(i,0) = score_vector[i];
    }

    return score_result;
}
}