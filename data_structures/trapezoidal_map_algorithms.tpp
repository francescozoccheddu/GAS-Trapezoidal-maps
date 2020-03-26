#pragma once

#include "trapezoidal_map.hpp"
#include <stdexcept>
#include <cassert>
#include <utils/geometry_utils.hpp>
#ifndef NDEBUG
#include <algorithm>
#endif

namespace GAS
{

	template<class Scalar>
	void TrapezoidalMap<Scalar>::weld (Pair _left, Pair _right)
	{
		weld (_left.bottom (), _right, true);
		weld (_right.bottom (), _left, false);
		if (_left.isSplit ())
		{
			weld (_left.top (), _right, true);
		}
		if (_right.isSplit ())
		{
			weld (_right.top (), _left, false);
		}
	}

	template<class Scalar>
	void TrapezoidalMap<Scalar>::weld (Trapezoid &_trapezoid, Pair _neighbors, bool _right)
	{
		if (_right)
		{
			assert (_neighbors.isLeftAligned ());
			assert (_trapezoid.right () == _neighbors.bottom ().left ());
		}
		else
		{
			assert (_neighbors.isRightAligned ());
			assert (_trapezoid.left () == _neighbors.bottom ().right ());
		}
		Trapezoid *&upper { _right ? _trapezoid.upperRightNeighbor () : _trapezoid.upperLeftNeighbor () };
		Trapezoid *&lower { _right ? _trapezoid.lowerRightNeighbor () : _trapezoid.lowerLeftNeighbor () };
		switch (_right ? _trapezoid.rightSource () : _trapezoid.leftSource ())
		{
			case ETrapezoidSideSource::Joint:
				upper = lower = nullptr;
				break;
			case ETrapezoidSideSource::Bottom:
				if (_trapezoid.top () == _neighbors.top ().top ())
				{
					upper = lower = &_neighbors.top ();
				}
				else if (_neighbors.isSplit () && _trapezoid.top () == _neighbors.bottom ().top ())
				{
					upper = lower = &_neighbors.bottom ();
				}
				break;
			case ETrapezoidSideSource::Top:
				if (_trapezoid.bottom () == _neighbors.bottom ().bottom ())
				{
					upper = lower = &_neighbors.bottom ();
				}
				else if (_neighbors.isSplit () && _trapezoid.bottom () == _neighbors.top ().bottom ())
				{
					upper = lower = &_neighbors.top ();
				}
				break;
			case ETrapezoidSideSource::External:
			{
				const Point &point { _right ? *_trapezoid.right () : *_trapezoid.left () };
				const Point &(Segment:: * endpoint)() const { _right ? &Segment::p1 : &Segment::p2 };
				bool (Trapezoid:: * isJoint)() const { _right ? &Trapezoid::isJointLeft : &Trapezoid::isJointRight };
				if (point == (_neighbors.bottom ().bottom ()->*endpoint) ())
				{
					if (!(_neighbors.bottom ().*isJoint) ())
					{
						upper = &_neighbors.bottom ();
					}
					else if (!(_neighbors.top ().*isJoint) ())
					{
						upper = &_neighbors.top ();
					}
				}
				else if (_neighbors.isSplit () && point == (_neighbors.top ().bottom ()->*endpoint) ())
				{
					if (!(_neighbors.bottom ().*isJoint) ())
					{
						lower = &_neighbors.bottom ();
					}
					if (!(_neighbors.top ().*isJoint) ())
					{
						upper = &_neighbors.top ();
					}
				}
				else if (point == (_neighbors.top ().top ()->*endpoint) ())
				{
					if (!(_neighbors.top ().*isJoint)())
					{
						lower = &_neighbors.top ();
					}
				}
			}
			break;
		}
	}

	template<class Scalar>
	Trapezoid<Scalar> &TrapezoidalMap<Scalar>::findLeftmostIntersectedTrapezoid (const Segment &_segment) const
	{
		assert (Geometry::areSegmentPointsHorizzontallySorted (_segment));
		const Point &left { _segment.p1 () }, &right { _segment.p2 () };
		return BDAG::walk (*m_root, [&](const TDAG::NodeData<Scalar> &_data) {
			const TDAG::Split<Scalar> &split { _data.first () };
			if (split.type () == TDAG::ESplitType::NonVertical)
			{
				const Segment &splitSegment { split.segment () };
				assert (Geometry::areSegmentPointsHorizzontallySorted (splitSegment));
				const Point &splitLeft { splitSegment.p1 () };
				// If two points share the same x-coordinate
				if (splitLeft.x () == left.x ())
				{
					if (splitLeft.y () == left.y ())
					{
						// If two segments share the same left point
						switch (Geometry::getPointSideWithSegment (splitSegment, right))
						{
							default:
								assert (false);
							case Geometry::ESide::Left:
								return TDAG::EChild::Left;
							case Geometry::ESide::Right:
								return TDAG::EChild::Right;
							case Geometry::ESide::Collinear:
								throw std::invalid_argument ("Overlapping segments are illegal");
						}
					}
					throw std::invalid_argument ("Points with the same x-coordinate are illegal");
				}
			}
			return TDAG::Utils::getPointQueryNextChild (split, left, TDAG::disambiguateAlwaysRight<Scalar>);
		}).data ().second ();
	}

	template<class Scalar>
	typename TrapezoidalMap<Scalar>::Pair TrapezoidalMap<Scalar>::splitVertically (Trapezoid &_trapezoid, const Point &_point)
	{
		assert (_point.x () > _trapezoid.leftX () && _point.x () < _trapezoid.rightX ());
		// Create new trapezoids
		Trapezoid &left { createTrapezoid (_trapezoid) }, &right { createTrapezoid (_trapezoid) };
		left.right () = right.left () = &_point;
		Trapezoid::link (left, right);
		// Replace old trapezoid
		_trapezoid.replaceInLeftNeighbors (&left);
		_trapezoid.replaceInRightNeighbors (&right);
		// Update DAG
		splitTrapezoid (_trapezoid, _point.x (), left, right);
		return { left, right };
	}

	template<class Scalar>
	typename TrapezoidalMap<Scalar>::Pair TrapezoidalMap<Scalar>::incrementalSplitHorizontally (Trapezoid &_trapezoid, const Segment &_segment, NullablePair _previous)
	{
		assert (!_previous || _previous.isRightAligned ());
		assert (Geometry::areSegmentPointsHorizzontallySorted (_segment));
		// Create new trapezoids or merge left
		const bool mergeBottom { _previous && _trapezoid.bottom () == _previous.bottom ().bottom () && &_segment == _previous.bottom ().top () };
		const bool mergeTop { _previous && _trapezoid.top () == _previous.top ().top () && &_segment == _previous.top ().bottom () };
		Trapezoid &bottom { mergeBottom ? _previous.bottom () : createTrapezoid (_trapezoid) };
		Trapezoid &top { mergeTop ? _previous.top () : createTrapezoid (_trapezoid) };
		bottom.top () = top.bottom () = &_segment;
		bottom.lowerRightNeighbor () = top.lowerRightNeighbor () = _trapezoid.lowerRightNeighbor ();
		bottom.upperRightNeighbor () = top.upperRightNeighbor () = _trapezoid.upperRightNeighbor ();
		bottom.right () = top.right () = _trapezoid.right ();
		// Replace old trapezoid
		if (_trapezoid.lowerLeftNeighbor ())
			_trapezoid.lowerLeftNeighbor ()->replaceRightNeighbor (&_trapezoid, &bottom);
		if (_trapezoid.upperLeftNeighbor ())
			_trapezoid.upperLeftNeighbor ()->replaceRightNeighbor (&_trapezoid, &top);
		if (_trapezoid.lowerRightNeighbor ())
			_trapezoid.lowerRightNeighbor ()->replaceLeftNeighbor (&_trapezoid, &bottom);
		if (_trapezoid.upperRightNeighbor ())
			_trapezoid.upperRightNeighbor ()->replaceLeftNeighbor (&_trapezoid, &top);
		assert (!mergeBottom || !mergeTop);
		if (!mergeBottom && !mergeTop)
		{
			if (_previous)
			{
				weld (_previous, { bottom, top });
			}
		}
		else if (mergeTop)
		{
			weld ({ _previous.bottom (), _previous.bottom () }, { bottom, bottom });
		}
		else if (mergeBottom)
		{
			weld ({ _previous.top (), _previous.top () }, { top, top });
		}
		// Update DAG
		splitTrapezoid (_trapezoid, _segment, top, bottom);
		return { bottom, top };
	}

	template<class Scalar>
	void TrapezoidalMap<Scalar>::addSegment (const Segment &_segment)
	{
		assert (m_root && !m_graph.isEmpty ());
		assert (std::find (m_segments.begin (), m_segments.end (), Geometry::sortSegmentPointsHorizontally (_segment)) == m_segments.end ());
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
				NullablePair previous { NullablePair::allOrNone (current->lowerLeftNeighbor (), current->upperLeftNeighbor ()) };
				NullablePair next { NullablePair::allOrNone (current->lowerRightNeighbor (), current->upperRightNeighbor ()) };
				while (current && right.x () > current->left ()->x ())
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
						next = NullablePair::allOrNone (current->lowerRightNeighbor (), current->upperRightNeighbor ());
						// Split
						previous = incrementalSplitHorizontally (*current, segment, previous);
						current = next ? segmentAboveRight ? &next.top () : &next.bottom () : nullptr;
					}
				}
				// Link last trapezoid
				if (next)
				{
					weld (previous, next);
				}
			}
		}
	}

}