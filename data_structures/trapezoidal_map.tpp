#pragma once

#include "trapezoidal_map.hpp"
#include <stdexcept>
#include <cassert>
#include <utils/geometry_utils.hpp>

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
	void TrapezoidalMap<Scalar>::destroy ()
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
		return TDAG::query (*m_root, _point);
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
		destroy ();
		initialize ();
	}

	template<class Scalar>
	const std::forward_list<Segment<Scalar>> &TrapezoidalMap<Scalar>::segments () const
	{
		return m_segments;
	}


}