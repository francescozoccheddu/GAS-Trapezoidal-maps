#pragma once

#include "geometry_utils.hpp"

namespace GAS
{

	namespace Geometry
	{
		template<class Scalar>
		ESide getPointSideWithRespectToVerticalLine (const Scalar &_lineX, const Point<Scalar> &_point)
		{
			const Scalar diff { _point.x () - _lineX };
			return diff > 0 ? ESide::Right : diff < 0 ? ESide::Left : ESide::Collinear;
		}

		template<class Scalar>
		ESide getPointSideWithRespectToSegment (const Segment<Scalar> &_segment, const Point<Scalar> &_point)
		{
			const Point<Scalar> &a { _segment.p1 () }, &b { _segment.p2 () };
			const Scalar det { (b.x () - a.x ()) * (_point.y () - a.y ()) - (b.y () - a.y ()) * (_point.x () - a.x ()) };
			return det > 0 ? ESide::Right : det < 0 ? ESide::Left : ESide::Collinear;
		}

	}

}