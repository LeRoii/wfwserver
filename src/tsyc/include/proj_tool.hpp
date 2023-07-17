#ifndef PROJ_TOOL_HPP_
#define PROJ_TOOL_HPP_

#include <cmath>
#include <iostream>
#include <Eigen/Dense>
#include <vector>

namespace proj_tool 
{
class ToolFun {
    public:
        double get_distance(Eigen::Vector2f pos1, Eigen::Vector2f pos2, 
                std::string type);
        double get_angle(Eigen::Vector2f pos1, Eigen::Vector2f pos2,
                Eigen::Vector2f pos3, Eigen::Vector2f pos4, std::string type);
        int get_max(Eigen::VectorXd count);
        Eigen::VectorXf get_min_info(Eigen::Vector2f pos1, Eigen::MatrixXf pos2);
        int id_format(std::string enemyid);

};

}

#endif // PROJ_TOOL_HPP_