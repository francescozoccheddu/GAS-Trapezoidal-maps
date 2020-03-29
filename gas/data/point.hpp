/// GAS::Point data structure.
/// \file
/// \author Francesco Zoccheddu

#ifndef GAS_DATA_POINT_INCLUDED
#define GAS_DATA_POINT_INCLUDED

#include <cg3/geometry/point2.h>

namespace GAS
{

	/// 2D point.
	/// \tparam Scalar
	/// The scalar type.
	template<class Scalar>
	using Point = cg3::Point2<Scalar>;

}

#endif