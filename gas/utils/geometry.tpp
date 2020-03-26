#pragma once

#include "geometry.hpp"

namespace GAS
{

	namespace Geometry
	{

		template<class Scalar>
		ESide getPointSideWithVerticalLine (const Scalar &_lineX, const Point<Scalar> &_point)
		{
			const Scalar diff { _point.x () - _lineX };
			return diff > 0 ? ESide::Right : diff < 0 ? ESide::Left : ESide::Collinear;
		}

		template<class Scalar>
		ESide getPointSideWithSegment (const Segment<Scalar> &_segment, const Point<Scalar> &_point)
		{
			const Point<Scalar> &a { _segment.p1 () }, &b { _segment.p2 () };
			const Scalar det { (b.x () - a.x ()) * (_point.y () - a.y ()) - (b.y () - a.y ()) * (_point.x () - a.x ()) };
			return det > 0 ? ESide::Left : det < 0 ? ESide::Right : ESide::Collinear;
		}

		template<class Scalar>
		Scalar evalLine (const Segment<Scalar> &_line, Scalar _x)
		{
			const Point<Scalar> &a { _line.p1 () }, &b { _line.p2 () };
			return (b.y () - a.y ()) * (_x - a.x ()) / (b.x () - a.x ()) + a.y ();
		}

		template<class Scalar>
		bool areSegmentPointsHorizzontallySorted (const Segment<Scalar> &_segment)
		{
			return _segment.p1 ().x () <= _segment.p2 ().x ();
		}

		template<class Scalar>
		bool isSegmentVertical (const Segment<Scalar> &_segment)
		{
			return _segment.p1 ().x () == _segment.p2 ().x ();
		}

		template<class Scalar>
		Segment<Scalar> sortSegmentPointsHorizontally (const Segment<Scalar> &_segment)
		{
			return areSegmentPointsHorizzontallySorted (_segment) ? _segment : Segment<Scalar> { _segment.p2 (), _segment.p1 () };
		}

	}

}