#ifndef PROJ_KALMAN_INTERFACE_HPP_
#define PROJ_KALMAN_INTERFACE_HPP_

#include <cmath>
#include <string>
#include <iostream>
#include <limits>
#include <Eigen/Dense>

namespace proj_kalman
{
    
    class KalmanInterface {

    public:

        KalmanInterface(int dim_x, int dim_z, int dim_u = 0): dim_x(dim_x), dim_z(dim_z), dim_u(dim_u) {
            x_p_k = Eigen::VectorXd::Zero(dim_x, 1);
            x_l_k = Eigen::VectorXd::Zero(dim_x, 1);
            x_k = Eigen::VectorXd::Zero(dim_x, 1);
            z_k = Eigen::VectorXd::Zero(dim_z, 1);
            A = Eigen::MatrixXd::Identity(dim_x, dim_x);
			K = Eigen::MatrixXd::Zero(dim_x, dim_z);
			Q = Eigen::MatrixXd::Zero(dim_x, dim_x);
			P = Eigen::MatrixXd::Zero(dim_x, dim_x);
			R = Eigen::MatrixXd::Zero(dim_z, dim_z);
        }

        virtual void init(Eigen::VectorXd &x_k) = 0;
        virtual Eigen::VectorXd predict(Eigen::VectorXd &u, double t) = 0;
        virtual Eigen::VectorXd update(Eigen::VectorXd &z_k) = 0;

    public:

		int dim_x, dim_z, dim_u;			// 状态维度, 测量维度, 控制维度

		Eigen::VectorXd x_p_k;				// 预测值
		Eigen::VectorXd	x_k;				// 状态值
		Eigen::VectorXd	x_l_k;				// last状态值
		Eigen::VectorXd	z_k;				// 观测量
		Eigen::VectorXd	u;					// 控制量

		Eigen::MatrixXd A;					// 状态转移矩阵
		Eigen::MatrixXd	P;					// 状态误差协方差矩阵
		Eigen::MatrixXd	K;					// 卡尔曼增益

		Eigen::MatrixXd	Q;					// 过程误差协方差矩阵
		Eigen::MatrixXd R;					// 观测误差协方差矩阵
    };
}

#endif // PROJ_KALMAN_INTERFACE_HPP_