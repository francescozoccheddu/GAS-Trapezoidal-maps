#ifndef GAS_DATA_TRAPEZOIDAL_MAP_IMPL_INCLUDED
#define GAS_DATA_TRAPEZOIDAL_MAP_IMPL_INCLUDED

#ifndef GAS_DATA_TRAPEZOIDAL_MAP_INCLUDED
#error 'gas/data/trapezoidal_map.tpp' should not be directly included
#endif

#include "trapezoidal_map.hpp"

#include <stdexcept>
#include <cassert>
#include <utility>
#include <gas/utils/geometry.hpp>

namespace GAS
{
	template<class Scalar>
	TrapezoidalMap<Scalar>::Pair::Pair (Trapezoid *_leftOrBottom, Trapezoid *_rightOrTop) : m_a { _leftOrBottom }, m_b { _rightOrTop }
	{}

	template<class Scalar>
	TrapezoidalMap<Scalar>::Pair::Pair (Trapezoid &_leftOrBottom, Trapezoid &_rightOrTop) : Pair { &_leftOrBottom, &_rightOrTop }
	{}

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
	Trapezoid<Scalar> &TrapezoidalMap<Scalar>::Pair::left () const
	{
		return *m_a;
	}

	template<class Scalar>
	Trapezoid<Scalar> &TrapezoidalMap<Scalar>::Pair::right () const
	{
		return *m_b;
	}

	template<class Scalar>
	Trapezoid<Scalar> &TrapezoidalMap<Scalar>::Pair::bottom () const
	{
		return *m_a;
	}

	template<class Scalar>
	Trapezoid<Scalar> &TrapezoidalMap<Scalar>::Pair::top () const
	{
		return *m_b;
	}

	template<class Scalar>
	const typename TrapezoidalMap<Scalar>::NullablePair TrapezoidalMap<Scalar>::NullablePair::null { nullptr, nullptr };

	template<class Scalar>
	typename TrapezoidalMap<Scalar>::NullablePair TrapezoidalMap<Scalar>::NullablePair::allOrNone (Trapezoid *_leftOrBottom, Trapezoid *_rightOrTop)
	{
		assert (((bool) _leftOrBottom) == ((bool) _rightOrTop));
		return { _leftOrBottom, _rightOrTop };
	}

	template<class Scalar>
	typename TrapezoidalMap<Scalar>::NullablePair &TrapezoidalMap<Scalar>::NullablePair::operator=(const Pair &_copy)
	{
		Pair::operator=(_copy);
		return *this;
	}

	template<class Scalar>
	TrapezoidalMap<Scalar>::NullablePair::operator bool () const
	{
		return m_a;
	}

	template<class Scalar>
	TDAG::Node<Scalar> &TrapezoidalMap<Scalar>::root ()
	{
		return *m_graph.nodes ().begin ();
	}

	template<class Scalar>
	void TrapezoidalMap<Scalar>::destroy ()
	{
		m_graph.clear ();
		m_segments.clear ();
	}

	template<class Scalar>
	void TrapezoidalMap<Scalar>::initialize ()
	{
		assert (m_graph.isEmpty ());
		Trapezoid trapezoid;
		trapezoid.left () = &bottomLeft ();
		trapezoid.right () = &bottomRight ();
		trapezoid.top () = &m_top;
		trapezoid.bottom () = &m_bottom;
		createTrapezoid (trapezoid);
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
	void TrapezoidalMap<Scalar>::splitTrapezoid (Trapezoid &_trapezoid, const Scalar &_x, Trapezoid &_left, Trapezoid &_right)
	{
		Node &node { getNode (_trapezoid) };
		m_graph.setInner (node, getNode (_left), getNode (_right));
		node.data () = _x;
	}

	template<class Scalar>
	void TrapezoidalMap<Scalar>::splitTrapezoid (Trapezoid &_trapezoid, const Segment &_segment, Trapezoid &_left, Trapezoid &_right)
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
	TrapezoidalMap<Scalar>::TrapezoidalMap (TrapezoidalMap &&_moved)
		: m_bottom { _moved.m_bottom }, m_top { _moved.m_top }, m_graph { std::move (_moved.m_graph) }, m_segments { std::move (_moved.m_segments) }
	{
		_moved.clear ();
	}

	template<class Scalar>
	TrapezoidalMap<Scalar> &TrapezoidalMap<Scalar>::operator=(TrapezoidalMap &&_moved)
	{
		clear ();
		m_bottom = _moved.m_bottom;
		m_top = _moved.m_top;
		m_graph = std::move (_moved.m_graph);
		m_segments = std::move (_moved.m_segments);
		_moved.clear ();
	}

	template<class Scalar>
	const TDAG::Node<Scalar> &TrapezoidalMap<Scalar>::root () const
	{
		return *m_graph.nodes ().begin ();
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
	const Trapezoid<Scalar> &TrapezoidalMap<Scalar>::query (const Point &_point) const
	{
		if (!isPointInsideBounds (_point))
		{
			throw std::invalid_argument ("Point is outside bounds");
		}
		return TDAG::query (root (), _point);
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
			throw std::invalid_argument ("Right x must be greater than left x");
		}
		if (_topRight.y () <= _bottomLeft.y ())
		{
			throw std::invalid_argument ("Top y must be greater than bottom y");
		}
		for (const Segment &segment : segments ())
		{
			if (!Geometry::isSegmentInsideBox (segment, _bottomLeft, _topRight))
			{
				throw std::invalid_argument ("Not all segments are inside the new bounds");
			}
		}
		m_bottom.set (_bottomLeft, { _topRight.x (), _bottomLeft.y () });
		m_top.set ({ _bottomLeft.x (), _topRight.y () }, _topRight);
	}

	template<class Scalar>
	bool TrapezoidalMap<Scalar>::isSegmentInsideBounds (const Segment &_segment) const
	{
		return Geometry::isSegmentInsideBox (_segment, bottomLeft (), topRight ());
	}

	template<class Scalar>
	bool TrapezoidalMap<Scalar>::isPointInsideBounds (const Point &_point) const
	{
		return Geometry::isPointInsideBox (_point, bottomLeft (), topRight ());
	}

	template<class Scalar>
	const std::forward_list<Segment<Scalar>> &TrapezoidalMap<Scalar>::segments () const
	{
		return m_segments;
	}

	template<class Scalar>
	void TrapezoidalMap<Scalar>::addSegment (const Segment &_segment)
	{
		assert (!m_graph.isEmpty ());
		if (_segment.p1 ().x () == _segment.p2 ().x ())
		{
			if (_segment.p1 ().y () == _segment.p2 ().y ())
			{
				throw std::invalid_argument ("Degenerate segments are illegal");
			}
			else
			{
				throw std::invalid_argument ("Vertical segments are illegal");
			}
		}
		if (!isSegmentInsideBounds (_segment))
		{
			throw std::invalid_argument ("Segment is not completely inside bounds");
		}
		addValidSegment (Geometry::sortSegmentPointsHorizontally (_segment));
	}

	template<class Scalar>
	void TrapezoidalMap<Scalar>::clear ()
	{
		destroy ();
		initialize ();
	}

}

#endif