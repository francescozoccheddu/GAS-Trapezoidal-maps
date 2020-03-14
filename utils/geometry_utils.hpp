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

		template<class Scalar>
		ESide getPointSideWithVerticalLine (const Scalar &lineX, const Point<Scalar> &point);

		template<class Scalar>
		ESide getPointSideWithSegment (const Segment<Scalar> &segment, const Point<Scalar> &point);

		template<class Scalar>
		Scalar evalLine (const Segment<Scalar> &line, Scalar x);

		template<class Scalar>
		bool areSegmentPointsHorizzontallySorted (const Segment<Scalar> &segment);

		template<class Scalar>
		bool isSegmentVertical (const Segment<Scalar> &segment);

		template<class Scalar>
		Segment<Scalar> sortSegmentPointsHorizontally (const Segment<Scalar> &segment);

	}

}

#include "geometry_utils.tpp"