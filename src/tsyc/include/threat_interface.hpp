#ifndef PROJ_THREAT_INTERFACE_HPP_
#define PROJ_THREAT_INTERFACE_HPP_

#include <cmath>
#include <Eigen/Dense>
#include <vector>
#include <iostream>

namespace proj_threat
{
class ThreatInterface {
    public:
        virtual Eigen::VectorXf danger_evaluate(std::vector<int> enemy_info, 
                Eigen::MatrixXf enemy_pos, std::vector<float> enemy_vel, 
                std::vector<int> enemy_intention,
                Eigen::Vector2f target) = 0;
                
        virtual Eigen::VectorXf get_weight(std::vector<int> enemy_info, 
                Eigen::MatrixXf enemy_pos, std::vector<float> enemy_vel, 
                std::vector<int> enemy_intention,
                Eigen::Vector2f target) = 0;

    public:
                            //   max_v max_a attack_range max_angle
        float enemy_info_[7][4] = {9.0, 1.0, 5.0, 0.5,   //  solider
                                25.0, 1.0, 15.0, 0.1,   //  fav
                                20.0, 1.0, 25.0, 0.05,  //  tank
                                16.0, 1.0, 15.0, 0.06,  //  armor
                                15.0, 1.0, 1.0, 0.05,   //  command
                                40.0, 1.0, 40.0, 0.04,  //  rover
                                50.0, 1.0, 50.0, 0.02}; //  fixuav

        
};
}

#endif // PROJ_THREAT_INTERFACE_HPP_