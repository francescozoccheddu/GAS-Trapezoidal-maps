#pragma once

#include "trapezoidal_map.hpp"
#include <stdexcept>

namespace GAS
{

	template<class Scalar>
	inline TrapezoidalMap<Scalar>::TrapezoidalMap ()
	{}

	template<class Scalar>
	TrapezoidalMap<Scalar>::TrapezoidalMap (const Point<Scalar> &_bottomLeft, const Point<Scalar> &_topRight)
	{
		setBounds (_bottomLeft, topRight);
		clear ();
	}

	template<class Scalar>
	const std::vector<Trapezoid<Scalar>> &TrapezoidalMap<Scalar>::getTrapezoids () const
	{
		return m_trapezoids;
	}

	template<class Scalar>
	const Point<Scalar> &TrapezoidalMap<Scalar>::getBottomLeft () const
	{
		return m_bottom.p1 ();
	}

	template<class Scalar>
	const Point<Scalar> &TrapezoidalMap<Scalar>::getBottomRight () const
	{
		return m_bottom.p2 ();
	}

	template<class Scalar>
	const Point<Scalar> &TrapezoidalMap<Scalar>::getTopLeft () const
	{
		return m_top.p1 ();
	}

	template<class Scalar>
	const Point<Scalar> &TrapezoidalMap<Scalar>::getTopRight () const
	{
		return m_top.p2 ();
	}

	template<class Scalar>
	const Segment<Scalar> &TrapezoidalMap<Scalar>::getTop () const
	{
		return m_top;
	}

	template<class Scalar>
	const Segment<Scalar> &TrapezoidalMap<Scalar>::getBottom () const
	{
		return m_bottom;
	}

	template<class Scalar>
	Scalar TrapezoidalMap<Scalar>::getLeftX () const
	{
		return m_bottom.p1 ().x ();
	}

	template<class Scalar>
	Scalar TrapezoidalMap<Scalar>::getRightX () const
	{
		return m_bottom.p2 ().x ();
	}

	template<class Scalar>
	Scalar TrapezoidalMap<Scalar>::getBottomY () const
	{
		return m_bottom.p1 ().y ();
	}

	template<class Scalar>
	Scalar TrapezoidalMap<Scalar>::getTopY () const
	{
		return m_top.p1 ().y ();
	}

	template<class Scalar>
	void TrapezoidalMap<Scalar>::setBounds (const Point<Scalar> &_bottomLeft, const Point<Scalar> &_topRight)
	{
		if (_topRight.x () <= _bottomLeft.x ())
		{
			throw std::domain_error ("Right x must be greater than left x");
		}
		if (_topRight.y () <= _bottomLeft.y ())
		{
			throw std::domain_error ("Top y must be greater than bottom y");
		}
		m_bottom.set (_bottomLeft, { _topRight.x (), _bottomLeft.y () });
		m_top.set ({ _bottomLeft.x (), _topRight.y () }, _topRight);
	}

	template<class Scalar>
	const std::vector<Segment<Scalar>> &TrapezoidalMap<Scalar>::getSegments () const
	{
		return m_segments;
	}

	template<class Scalar>
	void TrapezoidalMap<Scalar>::clear ()
	{
		m_segments.clear ();
		m_trapezoids.clear ();
		// Add bounding box trapezoid
		Trapezoid<Scalar> t;
		t.bottom = &m_bottom;
		t.top = &m_top;
		t.left = &getBottomLeft ();
		t.right = &getBottomRight ();
		m_trapezoids.push_back (t);
		m_dag = { &t };
	}

	template<class Scalar>
	void TrapezoidalMap<Scalar>::addSegment (const Segment<Scalar> &_segment)
	{
		// TODO
	}

}