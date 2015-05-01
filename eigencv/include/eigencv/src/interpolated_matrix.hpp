#pragma once

#include "Eigen\Core"

#include "includes.hpp"

// Either Eigen or MSVC is retarded and won't compile this for whatever reason.  I blame both, 
// personally.  In any case, we could potentially improve performance of convolutions if we could 
// modify Eigen to to perform border interpolation at the Matrix/MatrixBase/Dense/whatever level, 
// (basically, whatever method is called when each matrix element is loaded)
// allowing us to use the already built-in vectorization.  For now this remains a convoluted pipe
// dream.  Dear god is the Eigen source messy as fuck.

//namespace eigencv
//{
//	template<typename expression_type, typename border>
//	class interpolated_block;
//}
//
//namespace Eigen
//{
//	namespace internal
//	{
//		template<typename XprType, typename border>
//		struct traits<::eigencv::interpolated_block<XprType, border>>
//		{
//			typedef typename traits<XprType>::Scalar Scalar;
//			typedef typename traits<XprType>::StorageKind StorageKind;
//			typedef typename traits<XprType>::XprKind XprKind;
//			typedef typename nested<XprType>::type XprTypeNested;
//			typedef typename remove_reference<XprTypeNested>::type _XprTypeNested;
//			enum{
//				MatrixRows = traits<XprType>::RowsAtCompileTime,
//				MatrixCols = traits<XprType>::ColsAtCompileTime,
//				RowsAtCompileTime = Dynamic,
//				ColsAtCompileTime = Dynamic,
//				MaxRowsAtCompileTime = RowsAtCompileTime != Dynamic ? int(RowsAtCompileTime) : int(traits<XprType>::MaxRowsAtCompileTime),
//				MaxColsAtCompileTime = ColsAtCompileTime != Dynamic ? int(ColsAtCompileTime) : int(traits<XprType>::MaxColsAtCompileTime),
//				XprTypeIsRowMajor = (int(traits<XprType>::Flags)&RowMajorBit) != 0, 
//				IsRowMajor = (MaxRowsAtCompileTime == 1 && MaxColsAtCompileTime != 1) ? 1
//					: (MaxColsAtCompileTime == 1 && MaxRowsAtCompileTime != 1) ? 0
//					: XprTypeIsRowMajor,
//				HasSameStorageOrderAsXprType = (IsRowMajor == XprTypeIsRowMajor),
//				InnerSize = IsRowMajor ? int(ColsAtCompileTime) : int(RowsAtCompileTime),
//				InnerStrideAtCompileTime = HasSameStorageOrderAsXprType
//					? int(inner_stride_at_compile_time<XprType>::ret)
//					: int(outer_stride_at_compile_time<XprType>::ret),
//				OuterStrideAtCompileTime = HasSameStorageOrderAsXprType
//					? int(outer_stride_at_compile_time<XprType>::ret)
//					: int(inner_stride_at_compile_time<XprType>::ret),
//				MaskPacketAccessBit = (InnerSize == Dynamic || (InnerSize % packet_traits<Scalar>::size) == 0)
//					&& (InnerStrideAtCompileTime == 1)
//					? PacketAccessBit : 0,
//				MaskAlignedBit = ((OuterStrideAtCompileTime != Dynamic) && (((OuterStrideAtCompileTime * int(sizeof(Scalar))) % 16) == 0)) ? AlignedBit : 0,
//				FlagsLinearAccessBit = (RowsAtCompileTime == 1 || ColsAtCompileTime == 1 || ((traits<XprType>::Flags&LinearAccessBit))) ? LinearAccessBit : 0,
//				FlagsLvalueBit = is_lvalue<XprType>::value ? LvalueBit : 0,
//				FlagsRowMajorBit = IsRowMajor ? RowMajorBit : 0,
//				Flags0 = traits<XprType>::Flags & ((HereditaryBits & ~RowMajorBit) |
//				DirectAccessBit |
//				MaskPacketAccessBit |
//				MaskAlignedBit),
//				Flags = Flags0 | FlagsLinearAccessBit | FlagsLvalueBit | FlagsRowMajorBit
//			};
//		};
//	}
//}

namespace eigencv
{
	//template<typename index_type>
	//class border_reflect
	//{
	//public:
	//	// 
	//	static index_type interpolate(
	//		index_type max_index,
	//		index_type index)
	//	{
	//		index = std::abs(index);
	//		return index > max_index ? interpolate(max_index + (max_index - index - 2)) : index;
	//	}
	//};

	//template<typename scalar, int rows, int cols, int options, int max_rows, int max_cols>
	//class interpolated_matrix : public Eigen::Matrix<scalar, rows, cols, options, max_rows, max_cols>
	//{
	//	typedef Eigen::Matrix<scalar, rows, cols, 0, 1, 1> base_type;
	//public:

	//	interpolated_matrix() : 
	//		base_type() 
	//	{
	//		
	//	}

	//	// This constructor allows you to construct MyVectorType from Eigen expressions
	//	template<typename OtherDerived>
	//	interpolated_matrix(const Eigen::MatrixBase<OtherDerived>& other)
	//		: base_type(other)
	//	{ 

	//	}

	//	// This method allows you to assign Eigen expressions to MyVectorType
	//	template<typename OtherDerived>
	//	interpolated_matrix& operator =(const Eigen::MatrixBase<OtherDerived>& other)
	//	{
	//		this->Base::operator=(other);
	//		return *this;
	//	}

	//	inline Scalar& coeffRef(Index row, Index col)
	//	{
	//		EIGEN_STATIC_ASSERT_LVALUE(expression_type)
	//			return this->expression
	//			.const_cast_derived()
	//			.coeffRef(
	//				border::interpolate(this->block_rows, this->start_row + row),
	//				border::interpolate(this->block_cols, this->start_col + col));
	//	}

	//	inline const Scalar& coeffRef(Index row, Index col) const
	//	{
	//		return this->expression
	//			.derived()
	//			.coeffRef(
	//			border::interpolate(this->block_rows, this->start_row + row),
	//			border::interpolate(this->block_cols, this->start_col + col));
	//	}

	//	EIGEN_STRONG_INLINE const CoeffReturnType coeff(Index row, Index col) const
	//	{
	//		return this->expression
	//			.coeff(
	//			border::interpolate(this->block_rows, this->start_row + row),
	//			border::interpolate(this->block_cols, this->start_col + col));
	//	}

	//	inline Scalar& coeffRef(Index index)
	//	{
	//		EIGEN_STATIC_ASSERT_LVALUE(expression_type)
	//			return this->expression
	//			.const_cast_derived()
	//			.coeffRef(
	//			border::interpolate(this->block_rows, this->start_row + (RowsAtCompileTime == 1 ? 0 : index)),
	//			border::interpolate(this->block_cols, this->start_col + (RowsAtCompileTime == 1 ? index : 0)));
	//	}

	//	inline const Scalar& coeffRef(Index index) const
	//	{
	//		return this->expression
	//			.derived()
	//			.coeffRef(
	//				border::interpolate(this->block_rows, this->start_row + (RowsAtCompileTime == 1 ? 0 : index)),
	//				border::interpolate(this->block_cols, this->start_col + (RowsAtCompileTime == 1 ? index : 0)));
	//	}

	//	inline const CoeffReturnType coeff(Index index) const
	//	{
	//		return this->expression
	//			.coeff(
	//				border::interpolate(this->block_rows, this->start_row + (RowsAtCompileTime == 1 ? 0 : index)),
	//				border::interpolate(this->block_cols, this->start_col + (RowsAtCompileTime == 1 ? index : 0)));
	//	}

	//	template<int LoadMode>
	//	inline PacketScalar packet(Index row, Index col) const
	//	{
	//		return this->expression
	//			.template packet<Eigen::Unaligned>(
	//				border::interpolate(this->block_rows, this->start_row + row),
	//				border::interpolate(this->block_cols, this->start_col + col));
	//	}

	//	template<int LoadMode>
	//	inline void writePacket(Index row, Index col, const PacketScalar& val)
	//	{
	//		this->expression
	//			.const_cast_derived()
	//			.template writePacket<Eigen::Unaligned>(
	//				border::interpolate(this->block_rows, this->start_row + row),
	//				border::interpolate(this->block_cols, this->start_col + col),
	//				val);
	//	}

	//	template<int LoadMode>
	//	inline PacketScalar packet(Index index) const
	//	{
	//		return this->expression
	//			.template packet<Eigen::Unaligned>(
	//				border::interpolate(this->block_rows, this->start_row + (RowsAtCompileTime == 1 ? 0 : index)),
	//				border::interpolate(this->block_cols, this->start_col + (RowsAtCompileTime == 1 ? index : 0)));
	//	}

	//	template<int LoadMode>
	//	inline void writePacket(Index index, const PacketScalar& val)
	//	{
	//		this->expression
	//			.const_cast_derived()
	//			.template writePacket<Eigen::Unaligned>(
	//			border::interpolate(this->block_rows, this->start_row + (RowsAtCompileTime == 1 ? 0 : index)),
	//			border::interpolate(this->block_cols, this->start_col + (RowsAtCompileTime == 1 ? index : 0)),
	//			val);
	//	}
	//};

	//template<typename expression, typename border = border_reflect<expression>>
	//interpolated_block<expression, border> make_interpolated_block(
	//	expression& expr,
	//	typename expression::Index start_row,
	//	typename expression::Index start_col,
	//	typename expression::Index rows,
	//	typename expression::Index cols)
	//{
	//	return interpolated_block<expression, border>(expr, start_row, start_col, rows, cols);
	//}
}