#pragma once

#include <type_traits>
#include <cg3/geometry/point2.h>
#include <cg3/geometry/segment2.h>

namespace GAS
{

	template<class Scalar, typename = typename std::enable_if<std::is_arithmetic<Scalar>::value, Scalar>::type>
	class Trapezoid final
	{

	private:

		using Point = cg3::Point2<Scalar>;
		using Segment = cg3::Segment2<Scalar>;

	public:

		const Point *left {}, *right {};
		const Segment *bottom {}, *top {};

		Trapezoid *lowerLeftNeighbor {}, *upperLeftNeighbor {}, *lowerRightNeighbor {}, *upperRightNeighbor {};

	};

}