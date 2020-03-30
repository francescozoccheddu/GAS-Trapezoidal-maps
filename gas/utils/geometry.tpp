#ifndef GAS_UTILS_GEOMETRY_IMPL_INCLUDED
#define GAS_UTILS_GEOMETRY_IMPL_INCLUDED

#ifndef GAS_UTILS_GEOMETRY_INCLUDED
#error 'gas/utils/geometry.tpp' should not be directly included
#endif

#include "geometry.hpp"
#include <cassert>

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
			assert (!isSegmentDegenerate (_segment));
			const Point<Scalar> &a { _segment.p1 () }, &b { _segment.p2 () };
			const Scalar det { (b.x () - a.x ()) * (_point.y () - a.y ()) - (b.y () - a.y ()) * (_point.x () - a.x ()) };
			return det > 0 ? ESide::Left : det < 0 ? ESide::Right : ESide::Collinear;
		}

		template<class Scalar>
		Scalar evalLine (const Segment<Scalar> &_line, Scalar _x)
		{
			assert (!isSegmentVertical (_line));
			const Point<Scalar> &a { _line.p1 () }, &b { _line.p2 () };
			return (b.y () - a.y ()) * (_x - a.x ()) / (b.x () - a.x ()) + a.y ();
		}

		template<class Scalar>
		bool areSegmentPointsHorizzontallySorted (const Segment<Scalar> &_segment)
		{
			return _segment.p1 ().x () <= _segment.p2 ().x ();
		}

		template<class Scalar>
		bool isSegmentDegenerate (const Segment<Scalar> &_segment)
		{
			return _segment.p1 () == _segment.p2 ();
		}

		template<class Scalar>
		bool isSegmentVertical (const Segment<Scalar> &_segment)
		{
			assert (!isSegmentDegenerate (_segment));
			return _segment.p1 ().x () == _segment.p2 ().x ();
		}

		template<class Scalar>
		Segment<Scalar> sortSegmentPointsHorizontally (const Segment<Scalar> &_segment)
		{
			return areSegmentPointsHorizzontallySorted (_segment) ? _segment : Segment<Scalar> { _segment.p2 (), _segment.p1 () };
		}

		template<class Scalar>
		bool isPointInsideBox (const Point<Scalar> &point, const Point<Scalar> &_bottomLeft, const Point<Scalar> &_topRight)
		{
			assert (_bottomLeft.x () <= _topRight.x () && _bottomLeft.y () <= _topRight.y ());
			return point.x () > _bottomLeft.x () && point.x () < _topRight.x ()
				&& point.y () > _bottomLeft.y () && point.y () < _topRight.y ();
		}

		template<class Scalar>
		bool isSegmentInsideBox (const Segment<Scalar> &_segment, const Point<Scalar> &_bottomLeft, const Point<Scalar> &_topRight)
		{
			return isPointInsideBox (_segment.p1 (), _bottomLeft, _topRight)
				&& isPointInsideBox (_segment.p2 (), _bottomLeft, _topRight);
		}

		template<class Out, class In>
		const Point<Out> cast (const Point<In> &_in)
		{
			return { static_cast<Out>(_in.x ()), static_cast<Out>(_in.y ()) };
		}

		template<class Out, class In>
		const Segment<Out> cast (const Segment<In> &_in)
		{
			return { cast<Out> (_in.p1 ()), cast<Out> (_in.p2 ()) };
		}

		template<class InOut, class In>
		const Point<InOut> &cast (const Point<InOut> &_in)
		{
			return _in;
		}

		template<class InOut, class In>
		const Segment<InOut> &cast (const Segment<InOut> &_in)
		{
			return _in;
		}

	}

}

#endif