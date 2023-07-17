#ifndef PROJ_INTENTION_INTERFACE_HPP_
#define PROJ_INTENTION_INTERFACE_HPP_

#include <cmath>
#include <Eigen/Dense>
#include <vector>
#include <iostream>

namespace proj_intention
{
class IntentionInterface {
    public:
        IntentionInterface(double iter_step, double iter) {
            this->iter_ = iter;
            this->iter_step_ = iter_step;
        }

        virtual int single_intention(std::vector<float> enemy_posx, 
                std::vector<float> enemy_posy, Eigen::Vector2f target) = 0;
        virtual int all_intention(Eigen::MatrixXf enemy_posx, 
                Eigen::MatrixXf enemy_posy, Eigen::Vector2f target) = 0;

    public:
        double iter_step_;
        double iter_;
};
}

#endif // PROJ_INTENTION_INTERFACE_HPP_