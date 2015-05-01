#pragma once

#include "includes.hpp"

#include "Eigen\Core"

namespace eigencv
{
	class border_reflect
	{
		typedef std::ptrdiff_t index_type;
	public:
		// 
		static inline index_type interpolate(
			index_type max_index,
			index_type index)
		{
			index = std::abs(index);
			return index > max_index ? interpolate(max_index, max_index + (max_index - index - 2)) : index;
		}
	};

	class border_wrap
	{
		typedef std::ptrdiff_t index_type;
	public:
		// 
		static inline index_type interpolate(
			index_type max_index,
			index_type index)
		{
			return index % max_index;
		}
	};

	class border_extend
	{
		typedef std::ptrdiff_t index_type;
	public:
		// 
		static inline index_type interpolate(
			index_type max_index,
			index_type index)
		{
			return std::min(std::abs(index), max_index);
		}
	};

	class border_none
	{

	};

	namespace detail
	{
		template<typename matrix_type, typename kernel_type, typename border>
		class convolve
		{
		public:
			// 
			matrix_type operator()(
				const Eigen::MatrixBase<matrix_type>& matrix,
				const Eigen::MatrixBase<kernel_type>& kernel)
			{
				typedef typename matrix_type::Index index_type;

				index_type col, row, kernel_row, kernel_col;
				matrix_type result = matrix_type::Zero(matrix.rows(), matrix.cols());

				auto kernel_rows = kernel.rows();
				auto kernel_cols = kernel.cols();
				auto matrix_rows = matrix.rows();
				auto matrix_cols = matrix.cols();
				auto min_row = 0;
				auto min_col = 0;
				auto max_row = matrix_rows - 1;
				auto max_col = matrix_cols - 1;
				auto kernel_row_radius = kernel_rows / 2;
				auto kernel_col_radius = kernel_cols / 2;

				bool_t interpolate_row = false;
				bool_t interpolate_col = false;
				for (row = 0; row < matrix_rows; row++)
				{
					auto start_row = row - kernel_row_radius;
					//interpolate_row = start_row < min_row || row + kernel_row_radius > max_row;

					for (col = 0; col < matrix_cols; col++)
					{
						auto start_col = col - kernel_col_radius;
						//interpolate_col = start_col < min_col || col + kernel_col_radius > max_col;

						//if (interpolate_row == false && 
						//	interpolate_col == false)
						//	result.coeffRef(row, col) = matrix
						//		.block(start_row, start_col, kernel_rows, kernel_cols)
						//		.cwiseProduct(kernel)
						//		.sum();
						//else
						for (kernel_row = 0; kernel_row < kernel_rows; kernel_row++)
						{
							auto interpolated_row = border::interpolate(max_row, start_row + kernel_row);
							for (kernel_col = 0; kernel_col < kernel_cols; kernel_col++)
							{
								auto interpolated_col = border::interpolate(max_col, start_col + kernel_col);
								result.coeffRef(row, col) +=
									matrix.coeff(interpolated_row, interpolated_col) *
									kernel.coeff(kernel_row, kernel_col);
							}
						}
					}
				}

				return result;
			}
		};

		template<typename matrix_type, typename kernel_type>
		class convolve<matrix_type, kernel_type, border_none>
		{
		public:
			// 
			matrix_type operator()(
				const Eigen::MatrixBase<matrix_type>& matrix,
				const Eigen::MatrixBase<kernel_type>& kernel)
			{
				typedef typename matrix_type::Index index_type;

				index_type col, row;
				matrix_type result = matrix_type::Zero(matrix.rows(), matrix.cols());

				auto kernel_rows = kernel.rows();
				auto kernel_cols = kernel.cols();
				auto matrix_rows = matrix.rows();
				auto matrix_cols = matrix.cols();
				auto min_row = 0;
				auto min_col = 0;
				auto max_row = matrix_rows - 1;
				auto max_col = matrix_cols - 1;
				auto kernel_row_radius = kernel_rows / 2;
				auto kernel_col_radius = kernel_cols / 2;

				bool_t interpolate_row = false;
				bool_t interpolate_col = false;
				for (row = 0; row < matrix_rows; row++)
				{
					auto start_row = row - kernel_row_radius;
					interpolate_row = start_row < min_row || row + kernel_row_radius > max_row;

					for (col = 0; col < matrix_cols; col++)
					{
						auto start_col = col - kernel_col_radius;
						interpolate_col = start_col < min_col || col + kernel_col_radius > max_col;

						if (interpolate_row == false &&
							interpolate_col == false)
							result.coeffRef(row, col) = matrix
								.block(start_row, start_col, kernel_rows, kernel_cols)
								.cwiseProduct(kernel)
								.sum();
					}
				}

				return result;
			}
		};
	}

	// 
	template<typename border, typename matrix_type, typename kernel_type>
	matrix_type convolve(
		const Eigen::MatrixBase<matrix_type>& matrix,
		const Eigen::MatrixBase<kernel_type>& kernel)
	{
		return detail::convolve<matrix_type, kernel_type, border>()(matrix, kernel);
	}
}