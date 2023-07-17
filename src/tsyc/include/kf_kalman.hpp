#ifndef PROJ_KF_KALMAN_HPP_
#define PROJ_KF_KALMAN_HPP_

#include "kalman_interface.hpp"

namespace proj_kalman 
{
    class kf_kalman : public KalmanInterface
    {
    public:
        kf_kalman(int dim_x, int dim_z, int dim_u = 0) : KalmanInterface(dim_x, dim_z, dim_u) {
            A = Eigen::MatrixXd::Identity(dim_x, dim_x);
            if (dim_u > 0) {
                B = Eigen::MatrixXd::Zero(dim_x, dim_u);
            }
            else {
                B = Eigen::MatrixXd::Zero(dim_x, dim_x);
            }
            H = Eigen::MatrixXd::Zero(dim_z, dim_x);
        }

        kf_kalman(int dim_x, int dim_z, int dim_u, const Eigen::MatrixXd Q, const Eigen::MatrixXd R)
            : kf_kalman(dim_x, dim_z, dim_u) {
            this->Q = Q;
            this->R = R;
        }

        void init(Eigen::VectorXd &x_k) override;
        Eigen::VectorXd predict(Eigen::VectorXd &u, double t) override;
        Eigen::VectorXd update(Eigen::VectorXd &z_k) override;

    public:
        Eigen::MatrixXd B;
        Eigen::MatrixXd H;

    };
    
}

#endif // PROJ_KF_KALMAN_HPP_