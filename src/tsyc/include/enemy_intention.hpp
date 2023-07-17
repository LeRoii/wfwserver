#ifndef PROJ_ENEMY_INTENTION_HPP_
#define PROJ_ENEMY_INTENTION_HPP_

#include "intention_interface.hpp"
#include "proj_tool.hpp"

namespace proj_intention
{
class EnemyIntention : IntentionInterface {
    public:
        EnemyIntention(double iter_step, double iter) : IntentionInterface(iter_step, iter) {}
        
        int single_intention(std::vector<float> enemy_posx, 
            std::vector<float> enemy_posy, Eigen::Vector2f target) override;
        int all_intention(Eigen::MatrixXf enemy_posx, 
            Eigen::MatrixXf enemy_posy, Eigen::Vector2f target) override;
        
    public:
        proj_tool::ToolFun math_tool;
};
}

#endif // PROJ_ENEMY_INTENTION_HPP_