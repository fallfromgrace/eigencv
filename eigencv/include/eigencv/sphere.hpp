#ifndef EIGENCV_TRANSFORM_HPP
#define EIGENCV_TRANSFORM_HPP

#include <vector>
#include "Eigen\Core"

namespace Eigen
{
	namespace cv
	{

		template<typename matrix>
		Eigen::MatrixBase<Eigen::Matrix<typename matrix::Scalar, matrix::Rows + 1, 1>> fit_sphere(
			const Eigen::MatrixBase<matrix>& a)
		{
			auto normalized = a.colwise() - a.rowwise().mean();
		}
	}
}

#endif..