#include "proj_tool.hpp"

namespace proj_tool
{
double ToolFun::get_distance(Eigen::Vector2f pos1, Eigen::Vector2f pos2, 
        std::string type = "Euclidean") {
    double distance = 0;
    distance = sqrt(pow(pos1(0,0) - pos2(0,0), 2) + pow(pos1(1,0) - pos2(1,0), 2));
    
    return distance;
}

double ToolFun::get_angle(Eigen::Vector2f pos1, Eigen::Vector2f pos2,
        Eigen::Vector2f pos3, Eigen::Vector2f pos4, std::string type = "angle") {
    Eigen::Vector2f vector1(0,0), vector2(0,0);
    vector1(0,0) = pos1(0,0) - pos2(0,0);
    vector1(1,0) = pos1(1,0) - pos2(1,0);
    vector2(0,0) = pos3(0,0) - pos4(0,0);
    vector2(1,0) = pos3(1,0) - pos4(1,0);
    double cos_angle = vector1.dot(vector2) / (vector1.norm() * vector2.norm());
    if (cos_angle > 1 || cos_angle < -1) {
        std::cout << "Error, out of range(cos)!" << std::endl;
    }
    double angle = acos(cos_angle) * 180 / M_PI;

    return angle;
}

int ToolFun::get_max(Eigen::VectorXd count) {
    int temp = 0, temp_index = 0;
    for (int i = 0; i < count.size(); i++) {
        if (count(i) > temp) {
            temp = count(i);
            temp_index = i;
        }
    }

    return temp_index;
}

Eigen::VectorXf ToolFun::get_min_info(Eigen::Vector2f pos1, Eigen::MatrixXf pos_matrix) {
    Eigen::Vector2f pos;
    int temp_i = 0;
    pos << pos_matrix(0,0), pos_matrix(0,1);
    double temp_dis = get_distance(pos1, pos);

    for (int i = 0; i < pos_matrix.rows(); i++) {
        pos(0) = pos_matrix(i,0);
        pos(1) = pos_matrix(i,1);
        double dis = get_distance(pos1, pos);
        if (temp_dis > dis && dis > 1) {
            temp_dis = dis;
            temp_i = i;
        }
    }

    Eigen::VectorXf result = pos;
    result << temp_i, temp_dis;
    
    return result;
}
}