#pragma once

#include "trapezoidal_map.hpp"
#include <stdexcept>
#include <cassert>
#include <utils/geometry_utils.hpp>

namespace GAS
{

	template<class Scalar>
	TrapezoidalMap<Scalar>::Pair::Pair (Trapezoid &_leftOrBottom, Trapezoid &_rightOrTop) : m_a { &_leftOrBottom }, m_b { &_rightOrTop }
	{
	}

	template<class Scalar>
	bool TrapezoidalMap<Scalar>::Pair::isSplit () const
	{
		return m_a != m_b;
	}

	template<class Scalar>
	bool TrapezoidalMap<Scalar>::Pair::isLeftAligned () const
	{
		return m_a->left () == m_b->left ();
	}

	template<class Scalar>
	bool TrapezoidalMap<Scalar>::Pair::isRightAligned () const
	{
		return m_a->right () == m_b->right ();
	}

	template<class Scalar>
	bool TrapezoidalMap<Scalar>::Pair::isHorizontal () const
	{
		return m_a->right () == m_b->left ();
	}

	template<class Scalar>
	bool TrapezoidalMap<Scalar>::Pair::isVertical () const
	{
		return m_a->top () == m_b->bottom ();
	}

	template<class Scalar>
	Trapezoid<Scalar> &TrapezoidalMap<Scalar>::Pair::compact () const
	{
		assert (!isSplit ());
		return *m_a;
	}

	template<class Scalar>
	Trapezoid<Scalar> &TrapezoidalMap<Scalar>::Pair::bottom () const
	{
		return *m_a;
	}

	template<class Scalar>
	Trapezoid<Scalar> &TrapezoidalMap<Scalar>::Pair::left () const
	{
		return *m_a;
	}

	template<class Scalar>
	Trapezoid<Scalar> &TrapezoidalMap<Scalar>::Pair::top () const
	{
		return *m_b;
	}

	template<class Scalar>
	Trapezoid<Scalar> &TrapezoidalMap<Scalar>::Pair::right () const
	{
		return *m_b;
	}

	template<class Scalar>
	void TrapezoidalMap<Scalar>::deleteAll ()
	{
		if (m_dag)
		{
			BDAG::deleteGraph (*m_dag, trapezoidsCount () * 2);
			m_dag = nullptr;
		}
		m_segments.clear ();
		m_trapezoids.clear ();
	}

	template<class Scalar>
	void TrapezoidalMap<Scalar>::initialize ()
	{
		assert (!m_dag && m_segments.empty () && m_trapezoids.empty ());
		Trapezoid trapezoid;
		trapezoid.left () = &bottomLeft ();
		trapezoid.right () = &bottomRight ();
		trapezoid.top () = &m_top;
		trapezoid.bottom () = &m_bottom;
		m_dag = &getNode (createTrapezoid (trapezoid));
	}

	template<class Scalar>
	TDAG::Node<Scalar> &TrapezoidalMap<Scalar>::getNode (Trapezoid &_trapezoid) const
	{
		return Node::from (TDAG::NodeData<Scalar>::from (_trapezoid));
	}

	template<class Scalar>
	Trapezoid<Scalar> &TrapezoidalMap<Scalar>::createTrapezoid (const Trapezoid &_copy)
	{
		Node &node { *new Node { _copy } };
		Trapezoid &trapezoid { node.data ().trapezoid };
		m_trapezoids.insert (&trapezoid);
		return trapezoid;
	}

	template<class Scalar>
	Trapezoid<Scalar> &TrapezoidalMap<Scalar>::createTrapezoid (Trapezoid &&_moved)
	{
		Node &node { *new Node { _moved } };
		Trapezoid &trapezoid { node.data ().trapezoid };
		m_trapezoids.insert (&trapezoid);
		return trapezoid;
	}

	template<class Scalar>
	void TrapezoidalMap<Scalar>::splitTrapezoid (Trapezoid &_trapezoid, Scalar _x, Trapezoid &_left, Trapezoid &_right)
	{
		m_trapezoids.erase (&_trapezoid);
		Node &node { getNode (_trapezoid) };
		node.data () = _x;
		node.setInner (getNode (_left), getNode (_right));
	}

	template<class Scalar>
	void TrapezoidalMap<Scalar>::splitTrapezoid (Trapezoid &_trapezoid, Segment _segment, Trapezoid &_left, Trapezoid &_right)
	{
		m_trapezoids.erase (&_trapezoid);
		Node &node { getNode (_trapezoid) };
		node.data () = _segment;
		node.setInner (getNode (_left), getNode (_right));
	}

	template<class Scalar>
	TrapezoidalMap<Scalar>::TrapezoidalMap (const Point &_bottomLeft, const Point &_topRight)
	{
		setBounds (_bottomLeft, _topRight);
		initialize ();
	}

	template<class Scalar>
	int TrapezoidalMap<Scalar>::trapezoidsCount () const
	{
		return m_trapezoids.size ();
	}

	template<class Scalar>
	typename TrapezoidalMap<Scalar>::TrapezoidIterator TrapezoidalMap<Scalar>::begin () const
	{
		return m_trapezoids.cbegin ();
	}

	template<class Scalar>
	typename TrapezoidalMap<Scalar>::TrapezoidIterator TrapezoidalMap<Scalar>::end () const
	{
		return m_trapezoids.cend ();
	}

	template<class Scalar>
	const Point<Scalar> &TrapezoidalMap<Scalar>::bottomLeft () const
	{
		return m_bottom.p1 ();
	}

	template<class Scalar>
	const Point<Scalar> &TrapezoidalMap<Scalar>::bottomRight () const
	{
		return m_bottom.p2 ();
	}

	template<class Scalar>
	const Point<Scalar> &TrapezoidalMap<Scalar>::topLeft () const
	{
		return m_top.p1 ();
	}

	template<class Scalar>
	const Point<Scalar> &TrapezoidalMap<Scalar>::topRight () const
	{
		return m_top.p2 ();
	}

	template<class Scalar>
	const Segment<Scalar> &TrapezoidalMap<Scalar>::top () const
	{
		return m_top;
	}

	template<class Scalar>
	const Segment<Scalar> &TrapezoidalMap<Scalar>::bottom () const
	{
		return m_bottom;
	}

	template<class Scalar>
	Scalar TrapezoidalMap<Scalar>::leftX () const
	{
		return m_bottom.p1 ().x ();
	}

	template<class Scalar>
	Scalar TrapezoidalMap<Scalar>::rightX () const
	{
		return m_bottom.p2 ().x ();
	}

	template<class Scalar>
	Scalar TrapezoidalMap<Scalar>::bottomY () const
	{
		return m_bottom.p1 ().y ();
	}

	template<class Scalar>
	Scalar TrapezoidalMap<Scalar>::topY () const
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
	bool TrapezoidalMap<Scalar>::isSegmentInsideBounds (const Segment &_segment) const
	{
		return isPointInsideBounds (_segment.p1 ()) && isPointInsideBounds (_segment.p2 ());
	}

	template<class Scalar>
	bool TrapezoidalMap<Scalar>::isPointInsideBounds (const Point &_point) const
	{
		return _point.x () > leftX () && _point.x () < rightX () && _point.y () < topY () && _point.y () > bottomY ();
	}

	template<class Scalar>
	void TrapezoidalMap<Scalar>::clear ()
	{
		deleteAll ();
		initialize ();
	}

	template<class Scalar>
	void TrapezoidalMap<Scalar>::addSegment (const Segment &_segment)
	{
		assert (m_dag && !m_trapezoids.empty ());
		if (_segment.p1 () == _segment.p2 ())
		{
			throw std::invalid_argument ("Degenerate segment");
		}
		if (!isSegmentInsideBounds (_segment))
		{
			throw std::invalid_argument ("Segment is not completely inside bounds");
		}
		// Store segment
		m_segments.push_front (Geometry::sortSegmentPointsHorizontally (_segment));
		const Segment &segment { m_segments.front () };
		// Replace
		{
			Trapezoid *current;
			// Left vertical split
			{
				Trapezoid &leftmost { findLeftmostIntersectedTrapezoid (segment) };
				const Point &left { segment.p1 () };
				assert (left.x () >= leftmost.left ()->x ());
				if (left.x () > leftmost.left ()->x ())
				{
					Pair split { splitVertically (leftmost, left) };
					current = &split.right ();
				}
				else
				{
					assert (left.y () == leftmost.left ()->y ());
					current = &leftmost;
				}
			}
			// Horizontal split
			{
				const Point &right { segment.p2 () };
				Pair previous { *current->lowerLeftNeighbor (), *current->upperLeftNeighbor () };
				while (right.x () > current->left ()->x ())
				{
					// Split vertically if segment ends inside current trapezoid
					if (right.x () < current->right ()->x ())
					{
						Pair split { splitVertically (*current, right) };
						current = &split.left ();
					}
					// Split horizontally, link trapezoids and find next trapezoid
					{
						const Point &rightPoint { *current->right () };
						previous = incrementalSplitHorizontally (*current, segment, previous);
						// Decide whether to proceed splitting in the lower or the upper right neighbor
						const bool segmentAboveRight { Geometry::evalLine (segment, rightPoint.x ()) > rightPoint.y () };
						current = segmentAboveRight ? previous.top ().lowerRightNeighbor () : previous.bottom ().upperRightNeighbor ();
					}
				}
				// Link last trapezoid
				weld (previous, { *previous.bottom ().lowerRightNeighbor (), *previous.bottom ().upperRightNeighbor () });
			}
		}
	}

}