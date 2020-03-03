#pragma once

#include "trapezoidal_map.hpp"
#include <stdexcept>
#include <utils/geometry_utils.hpp>

namespace GAS
{

	template<class Scalar>
	TrapezoidalMap<Scalar>::Iterator::Iterator (const Trapezoid<Scalar> *_trapezoid) : m_current { _trapezoid }
	{}

	template<class Scalar>
	typename TrapezoidalMap<Scalar>::Iterator &TrapezoidalMap<Scalar>::Iterator::operator++ ()
	{
		if (m_current->upperRightNeighbor && m_current->upperRightNeighbor->top == m_current->top)
		{
			if (m_current->lowerRightNeighbor)
			{
				m_deque.push_back (m_current->lowerRightNeighbor);
			}
			m_current = m_current->upperRightNeighbor;
		}
		else if (m_current->lowerRightNeighbor)
		{
			m_current = m_current->lowerRightNeighbor;
		}
		else if (!m_deque.empty ())
		{
			m_current = m_deque.back ();
			m_deque.pop_back ();
		}
		else
		{
			m_current = nullptr;
		}
		return *this;
	}

	template<class Scalar>
	const Trapezoid<Scalar> &TrapezoidalMap<Scalar>::Iterator::operator* () const
	{
		return *m_current;
	}

	template<class Scalar>
	const Trapezoid<Scalar> *TrapezoidalMap<Scalar>::Iterator::operator-> () const
	{
		return m_current;
	}

	template<class Scalar>
	bool TrapezoidalMap<Scalar>::Iterator::operator!= (const Iterator &_other) const
	{
		return m_current != _other.m_current || m_deque != _other.m_deque;
	}

	template<class Scalar>
	void TrapezoidalMap<Scalar>::splitFive (Trapezoid<Scalar> &trapezoid, const Segment<Scalar> segment)
	{}

	template<class Scalar>
	void TrapezoidalMap<Scalar>::splitLeft (Trapezoid<Scalar> &_trapezoid, const Segment<Scalar> _segment)
	{}

	template<class Scalar>
	void TrapezoidalMap<Scalar>::splitHalf (Trapezoid<Scalar> &_trapezoid, const Segment<Scalar> _segment)
	{}

	template<class Scalar>
	void TrapezoidalMap<Scalar>::splitRight (Trapezoid<Scalar> &_trapezoid, const Segment<Scalar> _segment)
	{}

	template<class Scalar>
	void TrapezoidalMap<Scalar>::mergeLeft (Trapezoid<Scalar> &_trapezoid)
	{}

	template<class Scalar>
	TrapezoidalMap<Scalar>::TrapezoidalMap (const Point<Scalar> &_bottomLeft, const Point<Scalar> &_topRight)
	{
		setBounds (_bottomLeft, _topRight);
		clear ();
	}

	template<class Scalar>
	int TrapezoidalMap<Scalar>::getTrapezoidsCount () const
	{
		return m_trapezoidsCount;
	}

	template<class Scalar>
	typename TrapezoidalMap<Scalar>::Iterator TrapezoidalMap<Scalar>::begin () const
	{
		return Iterator (m_leftmostTrapezoid);
	}

	template<class Scalar>
	const typename TrapezoidalMap<Scalar>::Iterator &TrapezoidalMap<Scalar>::end () const
	{
		return m_end;
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
		{
			Iterator it { m_leftmostTrapezoid };
			while (it != m_end)
			{
				Trapezoid<Scalar> *const t { const_cast<Trapezoid<Scalar> *> (it.m_current) };
				++it;
				delete t;
			}
		}
		m_segments.clear ();
		// Add bounding box trapezoid
		m_leftmostTrapezoid = new Trapezoid<Scalar>;
		m_leftmostTrapezoid->bottom = &m_bottom;
		m_leftmostTrapezoid->top = &m_top;
		m_leftmostTrapezoid->left = &getBottomLeft ();
		m_leftmostTrapezoid->right = &getBottomRight ();
		m_dag = m_leftmostTrapezoid;
		m_trapezoidsCount = 1;
	}

	template<class Scalar>
	void TrapezoidalMap<Scalar>::addSegment (const Segment<Scalar> &_segment)
	{
		m_segments.push_back (_segment.p1 ().x () < _segment.p2 ().x () ? _segment : Segment<Scalar> { _segment.p2 (), _segment.p1 () });
		const Segment<Scalar> &s { m_segments[m_segments.size () - 1] };
		// Find leftmost intersected trapezoid
		// TODO Use DAG!!
		Trapezoid<Scalar> *t;
		{
			const Point<Scalar> &lp { _segment.p1 () };
			for (const Trapezoid<Scalar> &ti : *this)
			{
				if (lp.x () >= ti.left->x () && lp.x () < ti.right->x () && Geometry::getPointSideWithRespectToSegment (*ti.bottom, lp) != Geometry::getPointSideWithRespectToSegment (*ti.top, lp))
				{
					t = const_cast<Trapezoid<Scalar> *>(&ti);
					break;
				}
			}
		}
		// Start replacing
		while (t)
		{
			Trapezoid<Scalar> &ct { *t };
			const Scalar segLeft { s.p1 ().x () }, segRight { s.p2 ().x () };
			const Scalar trapLeft { ct.left->x () }, trapRight { ct.right->x () };
			t = Geometry::evalLine (s, trapRight) > ct.right->y () ? ct.upperRightNeighbor : ct.lowerRightNeighbor;
			if (segLeft > trapLeft)
			{
				if (segRight < trapRight)
				{
					splitFive (ct, s);
				}
				else
				{
					splitLeft (ct, s);
				}
			}
			else
			{
				if (segRight < trapRight)
				{
					if (segRight <= trapLeft)
					{
						t = nullptr;
					}
					else
					{
						splitRight (ct, s);
					}
				}
				else
				{
					splitHalf (ct, s);
				}
			}
		}
	}

}