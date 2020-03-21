#pragma once

#include "trapezoidal_map.hpp"
#include <stdexcept>
#include <cassert>
#include <utils/geometry_utils.hpp>

namespace GAS
{
	template<class Scalar>
	bool TrapezoidalMap<Scalar>::Pair::areAligned (const Pair &_left, const Pair &_right)
	{
		return (!_left.isSplit () || _left.isHorizontalSplit ())
			&& (!_right.isSplit () || _right.isHorizontalSplit ())
			&& *_left.bottom ().right () == *_right.bottom.left ();
	}

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
	bool TrapezoidalMap<Scalar>::Pair::isVerticalSplit () const
	{
		return isSplit () && m_a->right () == m_b->left ();
	}

	template<class Scalar>
	bool TrapezoidalMap<Scalar>::Pair::isHorizontalSplit () const
	{
		return isSplit () && m_a->top () == m_b->bottom ();
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
		m_root = nullptr;
		m_graph.clear ();
		m_segments.clear ();
	}

	template<class Scalar>
	void TrapezoidalMap<Scalar>::initialize ()
	{
		assert (!m_root && m_segments.empty () && m_graph.isEmpty ());
		Trapezoid trapezoid;
		trapezoid.left () = &bottomLeft ();
		trapezoid.right () = &bottomRight ();
		trapezoid.top () = &m_top;
		trapezoid.bottom () = &m_bottom;
		m_root = &getNode (createTrapezoid (trapezoid));
	}

	template<class Scalar>
	TDAG::Node<Scalar> &TrapezoidalMap<Scalar>::getNode (Trapezoid &_trapezoid) const
	{
		return Node::from (NodeData::from (_trapezoid));
	}

	template<class Scalar>
	Trapezoid<Scalar> &TrapezoidalMap<Scalar>::createTrapezoid (const Trapezoid &_copy)
	{
		return m_graph.createLeaf (_copy).data ().second ();
	}

	template<class Scalar>
	void TrapezoidalMap<Scalar>::splitTrapezoid (Trapezoid &_trapezoid, Scalar _x, Trapezoid &_left, Trapezoid &_right)
	{
		Node &node { getNode (_trapezoid) };
		m_graph.setInner (node, getNode (_left), getNode (_right));
		node.data () = _x;
	}

	template<class Scalar>
	void TrapezoidalMap<Scalar>::splitTrapezoid (Trapezoid &_trapezoid, Segment _segment, Trapezoid &_left, Trapezoid &_right)
	{
		Node &node { getNode (_trapezoid) };
		m_graph.setInner (node, getNode (_left), getNode (_right));
		node.data () = _segment;
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
		return m_graph.leafNodesCount ();
	}

	template<class Scalar>
	TDAG::Utils::ConstTrapezoidIterator<Scalar> TrapezoidalMap<Scalar>::begin () const
	{
		return TDAG::Utils::ConstTrapezoidIterator<Scalar> { m_graph.leafNodes ().begin () };
	}

	template<class Scalar>
	TDAG::Utils::ConstTrapezoidIterator<Scalar> TrapezoidalMap<Scalar>::end () const
	{
		return TDAG::Utils::ConstTrapezoidIterator<Scalar> { m_graph.leafNodes ().end () };
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
		assert (m_root && !m_graph.isEmpty ());
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
						// Decide whether to proceed splitting in the lower or the upper right neighbor before splitting
						const bool segmentAboveRight { Geometry::evalLine (segment, current->right ()->x ()) > current->right ()->y () };
						Trapezoid *next { segmentAboveRight ? current->upperRightNeighbor () : current->lowerRightNeighbor () };
						// Split
						previous = incrementalSplitHorizontally (*current, segment, previous);
						current = next;
					}
				}
				// Link last trapezoid
				weld (previous, { *previous.bottom ().lowerRightNeighbor (), *previous.bottom ().upperRightNeighbor () });
			}
		}
	}

}