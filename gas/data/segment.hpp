/// GAS::Segment data structure.
/// \file
/// \author Francesco Zoccheddu

#ifndef GAS_DATA_SEGMENT_INCLUDED
#define GAS_DATA_SEGMENT_INCLUDED

#include <cg3/geometry/segment2.h>

namespace GAS
{

	/// 2D Segment.
	/// \tparam Scalar
	/// The scalar type.
	template<class Scalar>
	using Segment = cg3::Segment2<Scalar>;

}

#endif