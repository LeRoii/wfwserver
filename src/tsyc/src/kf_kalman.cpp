#include "kf_kalman.hpp"

 namespace proj_kalman 
 {
    void kf_kalman::init(Eigen::VectorXd &x_k) {
        this->x_p_k = x_k;
        this->x_l_k = x_k;
        this->P = Eigen::MatrixXd::Zero(dim_x, dim_x);
    }

    Eigen::VectorXd kf_kalman::predict(Eigen::VectorXd &u, double t) {
        double delta_t = t;
        // A(0, 2) = delta_t;
        // A(1, 3) = delta_t;
        for (int i = 0; i < dim_x/2; i++) {
            A(i, dim_x/2 + i) = delta_t;
        }
        x_p_k = A * x_l_k;
		P = A * P * A.transpose() + Q;
		return x_p_k;
    }

    Eigen::VectorXd kf_kalman::update(Eigen::VectorXd &z_k) {
        for (int i = 0; i < dim_z; i++) {
            H(i,i) = 1;
        }
        K = P * H.transpose() * (H * P * H.transpose() + R).inverse();
		x_k = x_p_k + K * (z_k - H * x_p_k);
		P = P - K * H * P;
        x_l_k = x_k;
		return x_k;
    }
 }