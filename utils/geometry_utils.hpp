#pragma once

#include <data_structures/point.hpp>
#include <data_structures/segment.hpp>

namespace GAS
{

	namespace Geometry
	{

		enum class ESide
		{
			Left, Right, Collinear
		};

		enum class EPolarizedSide
		{
			Left, Right
		};

		template<class Scalar>
		ESide getPointSideWithRespectToVerticalLine (const Scalar &lineX, const Point<Scalar> &point);

		template<class Scalar>
		ESide getPointSideWithRespectToSegment (const Segment<Scalar> &segment, const Point<Scalar> &point);

		template<class Scalar>
		Scalar evalLine (const Segment<Scalar> &line, Scalar x);

	}

}

#include "geometry_utils.tpp"