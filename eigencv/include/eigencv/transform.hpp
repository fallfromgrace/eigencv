#ifndef EIGENCV_TRANSFORM_HPP
#define EIGENCV_TRANSFORM_HPP

#include <vector>
#include "Eigen\Core"

namespace Eigen
{
	namespace cv
	{

		template<typename matrix>
		Eigen::MatrixBase<Eigen::Matrix<typename matrix::Scalar, matrix::Rows + 1, matrix::Rows + 1>> get_rigid_transform(
			const Eigen::MatrixBase<matrix>& a,
			const Eigen::MatrixBase<matrix>& b)
		{
			Eigen::Matrix<typename matrix::Scalar, matrix::Rows + 1, matrix::Rows + 1> transform = 
				Eigen::Matrix<typename matrix::Scalar, matrix::Rows + 1, matrix::Rows + 1>::Identity();

			auto centroid_a = a.colwise() - a.rowwise().mean();
			auto centroid_b = b.colwise() - b.rowwise().mean();

			auto svd = (centered_a * centered_b.transpose()).eval().jacobiSVD(Eigen::ComputeFullU | Eigen::ComputeFullV);
			auto U = svd.matrixU();
			auto V = svd.matrixV();
			auto R = (V * U.transpose()).eval();
			auto C = Eigen::Matrix<typename matrix::Scalar, matrix::Rows, matrix::Rows>::Identity().eval();
			C(matrix::Rows - 1, matrix::Rows - 1) = R.determinant();
			R = V * C * U.transpose();
			auto T = -R * centroid_a + centroid_b;

			transform.block<matrix::Rows, matrix::Rows>(0, 0) = R;
			transform.block<matrix::Rows, 1>(0, matrix::Rows) = T;

			return transform;
		}

		template<typename vector>
		Eigen::MatrixBase<Eigen::Matrix<typename vector::Scalar, vector::Rows + 1, vector::Rows + 1>> get_rigid_transform(
			const std::vector<vector>& a,
			const std::vector<vector>& b)
		{
			return get_rigid_transform(
				Eigen::Map<Eigen::Matrix<typename vector::Scalar, vector::Rows, Eigen::Dynamic>>(a.data(), a.size()),
				Eigen::Map<Eigen::Matrix<typename vector::Scalar, vector::Rows, Eigen::Dynamic>>(b.data(), b.size()));
		}
	}
}

#endif//EIGENCV_TRANSFORM_HPP