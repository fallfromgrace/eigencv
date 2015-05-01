#pragma once

#include "convolve.hpp"

namespace eigencv
{
	// 
	template<typename matrix_type, size_t ksize, typename border = border_reflect<matrix_type>>
	Eigen::MatrixBase<matrix_type> gaussian_blur(const Eigen::MatrixBase<matrix_type>& matrix)
	{

	}
}