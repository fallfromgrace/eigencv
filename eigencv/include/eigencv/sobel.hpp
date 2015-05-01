#pragma once

#include "convolve.hpp"

namespace eigencv
{
	template<typename scalar, size_t ksize>
	Eigen::Matrix<scalar, ksize, ksize> make_sobel_kernel_x()
	{
		Eigen::Matrix<scalar, 3, 3> kernel;
		kernel <<
			1, 2, 1,
			2, 4, 2,
			1, 2, 1;

		return convolve(kernel, make_sobel_kernel_x<scalar, ksize - 2>());
	}

	template<typename scalar>
	Eigen::Matrix<scalar, 3, 3> make_sobel_kernel_x<scalar, 3>()
	{
		Eigen::Matrix<scalar, 3, 3> kernel;
		kernel <<
			1, 0 -1,
			2, 0, -2,
			1, 0, -1;

		return kernel;
	}

	template<typename scalar, size_t ksize>
	Eigen::Matrix<scalar, ksize, ksize> make_sobel_kernel_y()
	{
		return make_sobel_kernel_x<scalar, ksize>().transpose();
	}

	// 
	template<typename matrix_type, size_t ksize, typename border = border_reflect<matrix_type>>
	Eigen::MatrixBase<matrix_type> sobel(const Eigen::MatrixBase<matrix_type>& matrix)
	{
		auto kernel_x = make_sobel_kernel_x<typename matrix_type::Scalar, ksize>();
		auto kernel_y = make_sobel_kernel_y<typename matrix_type::Scalar, ksize>();
		auto result_x = convolve(matrix, kernel_x);
		auto result_y = convolve(matrix, kernel_y);
		
		auto norm = 8;
		while (ksize > 3)
		{
			norm *= 16;
			ksize -= 2;
		}

		return (result_x.cwiseProduct(result_x) + result_y.cwiseProduct(result_ya)).cwiseSqrt() / norm;
	}
}