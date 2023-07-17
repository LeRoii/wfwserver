#ifndef PROJ_TOPSIS_THREAT_HPP_
#define PROJ_TOPSIS_THREAT_HPP_

#include "threat_interface.hpp"
#include "proj_tool.hpp"

namespace proj_threat
{
class TopsisThreat : ThreatInterface {
    public:
        Eigen::VectorXf danger_evaluate(std::vector<int> enemy_info, 
                Eigen::MatrixXf enemy_pos, std::vector<float> enemy_vel, 
                std::vector<int> enemy_intention,
                Eigen::Vector2f target) override;

        Eigen::VectorXf get_weight(std::vector<int> enemy_info, 
                Eigen::MatrixXf enemy_pos, std::vector<float> enemy_vel, 
                std::vector<int> enemy_intention,
                Eigen::Vector2f target) override;

        Eigen::VectorXf get_weight(std::vector<int> enemy_info, Eigen::MatrixXf data_matrix);

        Eigen::MatrixXf topsis_matrix(std::vector<int> enemy_info, 
                Eigen::MatrixXf enemy_pos, std::vector<float> enemy_vel,
                std::vector<int> enemy_intention,
                Eigen::Vector2f target);
    private:
        float intention_dict[4] = {1, -1, 0, 0.5};
        proj_tool::ToolFun math_tool;
};
}

#endif // PROJ_TOPSIS_THREAT_HPP_