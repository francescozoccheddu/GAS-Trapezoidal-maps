#pragma once

#include "trapezoidal_map.hpp"
#include <stdexcept>
#include <utils/geometry_utils.hpp>

namespace GAS
{


	template<class Scalar>
	TrapezoidalMap<Scalar>::Iterator::Iterator (Trapezoid *_node) : m_current { _node }
	{}

	template<class Scalar>
	typename TrapezoidalMap<Scalar>::Iterator TrapezoidalMap<Scalar>::Iterator::fromTrapezoidalMap (const TrapezoidalMap &_trapezoidalMap)
	{
		return Iterator { _trapezoidalMap.m_leftmostTrapezoid };
	}

	template<class Scalar>
	const typename TrapezoidalMap<Scalar>::Iterator TrapezoidalMap<Scalar>::Iterator::end { nullptr };

	template<class Scalar>
	typename TrapezoidalMap<Scalar>::Iterator &TrapezoidalMap<Scalar>::Iterator::operator++()
	{
		if (m_current->upperRightNeighbor () && m_current->upperRightNeighbor ()->top () == m_current->top ())
		{
			if (m_current->lowerRightNeighbor () && m_current->lowerRightNeighbor () != m_current->upperRightNeighbor ())
			{
				m_deque.push_back (m_current->lowerRightNeighbor ());
			}
			m_current = m_current->upperRightNeighbor ();
		}
		else if (m_current->lowerRightNeighbor () && m_current->lowerRightNeighbor ()->top () == m_current->top ())
		{
			m_current = m_current->lowerRightNeighbor ();
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
	const Trapezoid<Scalar> &TrapezoidalMap<Scalar>::Iterator::operator*() const
	{
		return *m_current;
	}

	template<class Scalar>
	const Trapezoid<Scalar> *TrapezoidalMap<Scalar>::Iterator::operator->() const
	{
		return m_current;
	}

	template<class Scalar>
	bool TrapezoidalMap<Scalar>::Iterator::operator!=(const Iterator &_other) const
	{
		return m_current != _other.m_current || m_deque != _other.m_deque;
	}

	template<class Scalar>
	bool TrapezoidalMap<Scalar>::Iterator::hasNext () const
	{
		return *this != end;
	}

	template<class Scalar>
	TrapezoidalMap<Scalar>::HorizontalSplit::HorizontalSplit (Trapezoid &_bottom, Trapezoid &_top) : bottom { _bottom }, top { _top }
	{}

	template<class Scalar>
	TDAG::Node<Scalar> &TrapezoidalMap<Scalar>::splitVertically (Trapezoid &_trapezoid, const Point &_point)
	{
		// Create new trapezoids
		Node &leftNode { *new Node { _trapezoid } }, &rightNode { *new Node { _trapezoid } };
		Trapezoid &left { leftNode.leafData () }, &right { rightNode.leafData () };
		left.right () = right.left () = &_point;
		left.lowerRightNeighbor () = left.upperRightNeighbor () = &right;
		right.lowerLeftNeighbor () = right.upperLeftNeighbor () = &left;
		// Replace old trapezoid
		_trapezoid.replaceInLeftNeighbors (&left);
		_trapezoid.replaceInRightNeighbors (&right);
		if (&_trapezoid == m_leftmostTrapezoid)
		{
			m_leftmostTrapezoid = &left;
		}
		// Update DAG
		Node &node { Node::getNode (_trapezoid) };
		node.setInner (_point.x (), leftNode, rightNode);
		// Update trapezoids count
		m_trapezoidsCount++;
		return node;
	}

	template<class Scalar>
	typename TrapezoidalMap<Scalar>::HorizontalSplit &TrapezoidalMap<Scalar>::splitHorizontallyAndLink (Trapezoid &_trapezoid, const Segment &_segment, HorizontalSplit _leftNeighbors)
	{
		return {};
	}

	template<class Scalar>
	TrapezoidalMap<Scalar>::TrapezoidalMap (const Point &_bottomLeft, const Point &_topRight)
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
		return Iterator::fromTrapezoidalMap (*this);
	}

	template<class Scalar>
	const typename TrapezoidalMap<Scalar>::Iterator &TrapezoidalMap<Scalar>::end () const
	{
		return Iterator::end;
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
	void TrapezoidalMap<Scalar>::setBounds (const Point &_bottomLeft, const Point &_topRight)
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
		// Delete DAG and segments
		if (m_dag)
		{
			Node::recursiveDelete (*m_dag, static_cast<size_t>(m_trapezoidsCount));
		}
		m_segments.clear ();
		// Create initial trapezoid
		Trapezoid trapezoid;
		trapezoid.left () = &getBottomLeft ();
		trapezoid.right () = &getBottomRight ();
		trapezoid.top () = &m_top;
		trapezoid.bottom () = &m_bottom;
		m_dag = new Node { trapezoid };
		m_leftmostTrapezoid = &m_dag->leafData ();
		m_trapezoidsCount = 1;
	}


	template<class Scalar>
	inline TDAG::Node<Scalar> &getNode (const Point<Scalar> &_point)
	{

	}

	template<class Scalar>
	void TrapezoidalMap<Scalar>::addSegment (const Segment &_segment)
	{
		m_segments.push_front (_segment.p1 ().x () < _segment.p2 ().x () ? _segment : Segment { _segment.p2 (), _segment.p1 () });
		const Segment &s { m_segments.front () };
		// Find leftmost intersected trapezoid
		// TODO Use DAG!!
		splitVertically (*m_leftmostTrapezoid, s.p1 ());
		// TODO Replace
	}

}