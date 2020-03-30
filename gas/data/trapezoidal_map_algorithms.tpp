#ifndef GAS_DATA_TRAPEZOIDAL_MAP_ALGORITHMS_IMPL_INCLUDED
#define GAS_DATA_TRAPEZOIDAL_MAP_ALGORITHMS_IMPL_INCLUDED

#ifndef GAS_DATA_TRAPEZOIDAL_MAP_INCLUDED
#error 'gas/data/trapezoidal_map_algorithms.tpp' should not be directly included
#endif

#include "trapezoidal_map.hpp"

#include <stdexcept>
#include <cassert>
#include <gas/utils/geometry.hpp>

namespace GAS
{

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
			case ETrapezoidPointSource::Joint:
				upper = lower = nullptr;
				break;
			case ETrapezoidPointSource::Bottom:
				if (_trapezoid.top () == _neighbors.top ().top ())
				{
					upper = lower = &_neighbors.top ();
				}
				else if (_neighbors.isSplit () && _trapezoid.top () == _neighbors.bottom ().top ())
				{
					upper = lower = &_neighbors.bottom ();
				}
				break;
			case ETrapezoidPointSource::Top:
				if (_trapezoid.bottom () == _neighbors.bottom ().bottom ())
				{
					upper = lower = &_neighbors.bottom ();
				}
				else if (_neighbors.isSplit () && _trapezoid.bottom () == _neighbors.top ().bottom ())
				{
					upper = lower = &_neighbors.top ();
				}
				break;
			case ETrapezoidPointSource::External:
			{
				const PointS &point { _right ? *_trapezoid.right () : *_trapezoid.left () };
				const PointS &(SegmentS:: * endpoint)() const { _right ? &SegmentS::p1 : &SegmentS::p2 };
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
	template<class ArithmeticScalar>
	ArithmeticScalar TrapezoidalMap<Scalar>::evalLineOnRightEdge (const SegmentS &_line, const Trapezoid &_trapezoid)
	{
		const ArithmeticScalar &x { static_cast<ArithmeticScalar> (_trapezoid.rightX ()) };
		return Geometry::evalLine (Geometry::cast<ArithmeticScalar> (_line), x);
	}

	template<class Scalar>
	template<class ArithmeticScalar>
	Trapezoid<Scalar> &TrapezoidalMap<Scalar>::findLeftmostIntersectedTrapezoid (const SegmentS &_segment)
	{
		assert (Geometry::areSegmentPointsHorizzontallySorted (_segment));
		const PointS &left { _segment.p1 () }, &right { _segment.p2 () };
		return BDAG::walk (root (), [&](const TDAG::NodeData<Scalar> &_data) {
			const TDAG::Split<Scalar> &split { _data.first () };
			if (split.type () == TDAG::ESplitType::NonVertical)
			{
				const SegmentS &splitSegment { split.segment () };
				assert (Geometry::areSegmentPointsHorizzontallySorted (splitSegment));
				const PointS &splitLeft { splitSegment.p1 () };
				// If two points share the same x-coordinate
				if (splitLeft.x () == left.x ())
				{
					if (splitLeft.y () == left.y ())
					{
						// If two segments share the same left point
						if (_segment == splitSegment)
						{
							throw std::invalid_argument ("Duplicate segments are illegal");
						}
						switch (Geometry::getPointSideWithSegment (Geometry::cast<ArithmeticScalar> (splitSegment), Geometry::cast<ArithmeticScalar> (right)))
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
			return TDAG::Utils::getPointQueryNextChild (split, Geometry::cast<ArithmeticScalar> (left), TDAG::Utils::disambiguateAlwaysRight);
		}).data ().second ();
	}

	template<class Scalar>
	typename TrapezoidalMap<Scalar>::Pair TrapezoidalMap<Scalar>::splitVertically (Trapezoid &_trapezoid, const PointS &_point)
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
	typename TrapezoidalMap<Scalar>::Pair TrapezoidalMap<Scalar>::incrementalSplitHorizontally (Trapezoid &_trapezoid, const SegmentS &_segment, NullablePair _previous)
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
	template<class ArithmeticScalar>
	void TrapezoidalMap<Scalar>::updateForNewSegment (const SegmentS &_segment, Trapezoid &_leftmost)
	{
		assert (Geometry::areSegmentPointsHorizzontallySorted (_segment));
		Trapezoid *current;
		// Left vertical split
		{
			const PointS &left { _segment.p1 () };
			assert (left.x () >= _leftmost.left ()->x ());
			if (left.x () > _leftmost.left ()->x ())
			{
				Pair split { splitVertically (_leftmost, left) };
				current = &split.right ();
			}
			else
			{
				assert (left.y () == _leftmost.left ()->y ());
				current = &_leftmost;
			}
		}
		// Horizontal split
		{
			const PointS &right { _segment.p2 () };
			NullablePair previous { NullablePair::allOrNone (current->lowerLeftNeighbor (), current->upperLeftNeighbor ()) };
			NullablePair next { NullablePair::allOrNone (current->lowerRightNeighbor (), current->upperRightNeighbor ()) };
			while (current && right.x () > current->left ()->x ())
			{
				// Split vertically if _segment ends inside current trapezoid
				if (right.x () < current->right ()->x ())
				{
					Pair split { splitVertically (*current, right) };
					current = &split.left ();
				}
				// Split horizontally and find the next trapezoid
				{
					// Decide whether to proceed splitting in the lower or the upper right neighbor before splitting
					const bool segmentAboveRight { evalLineOnRightEdge (_segment, *current) > static_cast<ArithmeticScalar>(current->right ()->y ()) };
					next = NullablePair::allOrNone (current->lowerRightNeighbor (), current->upperRightNeighbor ());
					// Split
					previous = incrementalSplitHorizontally (*current, _segment, previous);
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

	template<class Scalar>
	template<class ArithmeticScalar>
	bool TrapezoidalMap<Scalar>::doesSegmentIntersect (const SegmentS &_segment, const Trapezoid &_leftmost) const
	{
		assert (Geometry::areSegmentPointsHorizzontallySorted (_segment));
		assert (isSegmentInsideBounds (_segment));
		const PointS &right { _segment.p2 () };
		const Trapezoid *current { &_leftmost };
		while (right.x () > current->rightX ())
		{
			const ArithmeticScalar y { evalLineOnRightEdge<ArithmeticScalar> (_segment, *current) };
			if (y <= current->template bottomRight<ArithmeticScalar> ().y () ||
				y == static_cast<ArithmeticScalar> (current->right ()->y ()) ||
				y >= current->template topRight<ArithmeticScalar> ().y ())
			{
				return true;
			}
			const bool segmentAboveRight { y > static_cast<ArithmeticScalar>(current->right ()->y ()) };
			current = segmentAboveRight ? current->upperRightNeighbor () : current->lowerRightNeighbor ();
		}
		if (right.x () < current->rightX ())
		{
			if (!current->contains (Geometry::cast<ArithmeticScalar> (right)))
			{
				return true;
			}
		}
		else if (right != current->bottom ()->p2 ()
			&& right != current->top ()->p2 ()
			&& right.y () != current->right ()->y ())
		{
			throw std::invalid_argument ("Points with the same x-coordinate are illegal");
		}
		return false;
	}

}

#endif