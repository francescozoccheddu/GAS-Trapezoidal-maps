#pragma once

#include "point.hpp"
#include "segment.hpp"

namespace GAS
{

	template<class Scalar>
	struct Trapezoid final
	{

		const Point<Scalar> *left {}, *right {};
		const Segment<Scalar> *bottom {}, *top {};

		Trapezoid *lowerLeftNeighbor {}, *upperLeftNeighbor {}, *lowerRightNeighbor {}, *upperRightNeighbor {};

	};

}