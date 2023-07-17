#include "enemy_intention.hpp"
#include "proj_tool.hpp"

namespace proj_intention
{
int EnemyIntention::single_intention(std::vector<float> enemy_posx, 
        std::vector<float> enemy_posy, Eigen::Vector2f target) {
    Eigen::VectorXd count;
    count = Eigen::VectorXd::Zero(4,1);

    if (enemy_posx.size() * enemy_posy.size() == 0 || enemy_posx.size() != enemy_posy.size()) {
        std::cout << "Error! check the pos size" << std::endl;
        return 0;
    }

    for (int i = 1; i < enemy_posx.size(); i++) {

        Eigen::Vector2f current_pos(enemy_posx[i], enemy_posy[i]);
        Eigen::Vector2f last_pos(enemy_posx[i], enemy_posy[i]);
        Eigen::Vector2f init_pos(enemy_posx[0], enemy_posy[0]);
        double current_dis = math_tool.get_distance(current_pos, target, "Euclidean");
        double last_dis = math_tool.get_distance(last_pos, target, "Euclidean");
        double init_dis = math_tool.get_distance(init_pos, target, "Euclidean");

        double dx = enemy_posx[i] - enemy_posx[i-1];
        double dy = enemy_posy[i] - enemy_posy[i-1];
        double enemy_vel = sqrt(pow((dx / iter_step_), 2) + pow((dy / iter_step_), 2));
        double angle = math_tool.get_angle(last_pos, target, last_pos, current_pos, "angle");

        if (current_dis < last_dis && abs(enemy_vel) > 1 && angle < 60) {
            count[0] += 1;
            continue;
        }
        if (current_dis > last_dis && abs(enemy_vel) > 1 && angle > 120) {
            count[1] += 1;
            continue;
        }
        if (abs(enemy_vel) < 1) {
            count[2] += 1;
            continue;
        }
        if (current_dis - init_dis < 1 && angle > 60 && angle < 120) {
            count[3] += 1;
            continue;
        }

    }

    int index = math_tool.get_max(count);
    return index;
}

// 矩阵行数为敌方个数，列数为坐标个数
int EnemyIntention::all_intention(Eigen::MatrixXf enemy_posx, 
        Eigen::MatrixXf enemy_posy, Eigen::Vector2f target) {
    Eigen::VectorXd count;
    count = Eigen::VectorXd::Zero(6,1);

    int enemy_num = enemy_posx.rows();
    std::vector<float> temp_x, temp_y;
    for(int i = 0; i < enemy_num; i++) {
        for (int j = 0; j < enemy_posx.cols(); j++) {
            temp_x.push_back(enemy_posx(i, j));
        }
        for (int j = 0; j < enemy_posy.cols(); j++) {
            temp_y.push_back(enemy_posy(i, j));
        }

        int index = single_intention(temp_x, temp_y, target);
        count[index] += 1;
    }

    if (count[0] > 0.8 * enemy_num) 
        return 0;
    if (count[1] > 0.8 * enemy_num) 
        return 1;
    if (count[2] > 0.8 * enemy_num) 
        return 2;
    if (count[3] > 0.8 * enemy_num) 
        return 3;
    if (count[0] > 0.2 * enemy_num && count[3] > 0.6 * enemy_num) 
        return 4;
    if (count[0] > 0.2 * enemy_num && count[2] > 0.6 * enemy_num) 
        return 5; 

    return math_tool.get_max(count);
}
}