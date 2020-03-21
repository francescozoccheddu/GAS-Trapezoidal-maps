#pragma once

#include "trapezoidal_map.hpp"
#include <stdexcept>
#include <cassert>
#include <utils/geometry_utils.hpp>

namespace GAS
{

	template<class Scalar>
	typename TrapezoidalMap<Scalar>::ELeftWeldJointType TrapezoidalMap<Scalar>::getLeftWeldJointType (Pair _left)
	{
		assert (_left.isRightAligned ());
		if (!_left.isSplit ())
		{
			return ELeftWeldJointType::Compact;
		}
		else
		{
			assert (_left.isHorizontalSplit ());
			const Point &right { _left.bottom ().top ()->p2 () };
			if (right == _left.bottom ().bottom ()->p2 ())
			{
				return ELeftWeldJointType::JointBottom;
			}
			else if (right == _left.top ().top ()->p2 ())
			{
				return ELeftWeldJointType::JointTop;
			}
			else
			{
				return ELeftWeldJointType::Split;
			}
		}
	}

	template<class Scalar>
	typename TrapezoidalMap<Scalar>::ERightWeldFitness TrapezoidalMap<Scalar>::getRightWeldBottomFitness (const Trapezoid &_left, const Trapezoid &_right)
	{
		assert (_left.right ()->x () == _right.left ()->x ());
		if (_left.bottom () == _right.bottom () || _left.bottom ()->p2 () == _right.bottom ()->p1 ())
		{
			return ERightWeldFitness::Fit;
		}
		// TODO Do not rely on floating point computations
		else if (_left.bottomRight ().y () > _right.bottomLeft ().y ())
		{
			return ERightWeldFitness::Extended;
		}
		else
		{
			return ERightWeldFitness::Shrinked;
		}
	}

	template<class Scalar>
	typename TrapezoidalMap<Scalar>::ERightWeldFitness TrapezoidalMap<Scalar>::getRightWeldTopFitness (const Trapezoid &_left, const Trapezoid &_right)
	{
		assert (_left.right ()->x () == _right.left ()->x ());
		if (_left.top () == _right.top () || _left.top ()->p2 () == _right.top ()->p1 ())
		{
			return ERightWeldFitness::Fit;
		}
		// TODO Do not rely on floating point computations
		else if (_left.topRight ().y () < _right.topLeft ().y ())
		{
			return ERightWeldFitness::Extended;
		}
		else
		{
			return ERightWeldFitness::Shrinked;
		}
	}

	template<class Scalar>
	typename TrapezoidalMap<Scalar>::RightWeldConfiguration TrapezoidalMap<Scalar>::getRightWeldConfiguration (Pair _left, Pair _right)
	{
		assert (_left.isRightAligned () && _right.isLeftAligned ());
		RightWeldConfiguration configuration;
		configuration.split = _right.isSplit ();
		configuration.bottomFitness = getRightWeldBottomFitness (_left.bottom (), _right.bottom ());
		configuration.topFitness = getRightWeldTopFitness (_left.top (), _right.top ());
		return configuration;
	}

	template<class Scalar>
	void TrapezoidalMap<Scalar>::weld (Pair _left, Pair _right)
	{
		const RightWeldConfiguration rightConfiguration { getRightWeldConfiguration (_left, _right) };
		switch (getLeftWeldJointType (_left))
		{
			case ELeftWeldJointType::Compact:
			{
				if (rightConfiguration.split)
				{
					if (rightConfiguration.bottomFitness == ERightWeldFitness::Fit)
					{
						if (rightConfiguration.topFitness == ERightWeldFitness::Fit)
						{
							_left.compact ().setRightNeighbors (&_right.bottom (), &_right.top ());
							_right.bottom ().setLeftNeighbors (&_left.compact ());
							_right.top ().setLeftNeighbors (&_left.compact ());
							return;
						}
						else if (rightConfiguration.topFitness == ERightWeldFitness::Extended)
						{
							_left.compact ().setRightNeighbors (&_right.bottom (), &_right.top ());
							_right.bottom ().setLeftNeighbors (&_left.compact ());
							_right.top ().setLeftNeighbors (&_left.compact ());
							return;
						}
					}
					else if (rightConfiguration.bottomFitness == ERightWeldFitness::Extended && rightConfiguration.topFitness == ERightWeldFitness::Fit)
					{
						_left.compact ().setRightNeighbors (&_right.bottom (), &_right.top ());
						_right.bottom ().upperLeftNeighbor () = &_left.compact ();
						_right.top ().setLeftNeighbors (&_left.compact ());
						return;
					}
				}
				else
				{
					if (rightConfiguration.bottomFitness == ERightWeldFitness::Fit && rightConfiguration.topFitness == ERightWeldFitness::Extended)
					{
						_left.compact ().setRightNeighbors (&_right.compact ());
						_right.compact ().lowerLeftNeighbor () = &_left.compact ();
						return;
					}
					else if (rightConfiguration.bottomFitness == ERightWeldFitness::Extended && rightConfiguration.topFitness == ERightWeldFitness::Fit)
					{
						_left.compact ().setRightNeighbors (&_right.compact ());
						_right.compact ().upperLeftNeighbor () = &_left.compact ();
						return;
					}
				}
			}
			break;
			case ELeftWeldJointType::Split:
			{
				if (rightConfiguration.split)
				{
					assert (_left.bottom ().top () == _right.bottom ().top () || _left.bottom ().right () == _right.bottom ().left ());
					if (rightConfiguration.bottomFitness == ERightWeldFitness::Fit)
					{
						if (rightConfiguration.topFitness == ERightWeldFitness::Fit)
						{
							Trapezoid::link (_left.bottom (), _right.bottom ());
							Trapezoid::link (_left.top (), _right.top ());
							return;
						}
						else if (rightConfiguration.topFitness == ERightWeldFitness::Shrinked)
						{
							Trapezoid::link (_left.bottom (), _right.bottom ());
							_left.top ().lowerRightNeighbor () = &_right.top ();
							_right.top ().setLeftNeighbors (&_left.top ());
							return;
						}
						else if (rightConfiguration.topFitness == ERightWeldFitness::Extended)
						{
							Trapezoid::link (_left.bottom (), _right.bottom ());
							_left.top ().setRightNeighbors (&_right.top ());
							_right.top ().lowerLeftNeighbor () = &_left.top ();
							return;
						}
					}
					else if (rightConfiguration.topFitness == ERightWeldFitness::Fit)
					{
						if (rightConfiguration.bottomFitness == ERightWeldFitness::Shrinked)
						{
							Trapezoid::link (_left.top (), _right.top ());
							_left.bottom ().upperRightNeighbor () = &_right.bottom ();
							_right.bottom ().setLeftNeighbors (&_left.bottom ());
							return;
						}
						else if (rightConfiguration.bottomFitness == ERightWeldFitness::Extended)
						{
							Trapezoid::link (_left.top (), _right.top ());
							_left.bottom ().setRightNeighbors (&_right.bottom ());
							_right.bottom ().upperLeftNeighbor () = &_left.bottom ();
							return;
						}
					}
				}
				else if (rightConfiguration.bottomFitness == ERightWeldFitness::Fit && rightConfiguration.topFitness == ERightWeldFitness::Fit)
				{
					_right.compact ().setLeftNeighbors (&_left.bottom (), &_left.top ());
					_left.bottom ().setRightNeighbors (&_right.compact ());
					_left.top ().setRightNeighbors (&_right.compact ());
					return;
				}
			}
			break;
			case ELeftWeldJointType::JointBottom:
			{
				if (!rightConfiguration.split && rightConfiguration.topFitness == ERightWeldFitness::Fit)
				{
					switch (rightConfiguration.bottomFitness)
					{
						case ERightWeldFitness::Fit:
						{
							_left.bottom ().setRightNeighbors (nullptr);
							Trapezoid::link (_left.top (), _right.compact ());
							return;
						}
						case ERightWeldFitness::Extended:
						{
							_left.bottom ().setRightNeighbors (nullptr);
							_left.top ().setRightNeighbors (&_right.top ());
							_right.compact ().upperLeftNeighbor () = &_left.top ();
							return;
						}
					}
				}
			}
			break;
			case ELeftWeldJointType::JointTop:
			{
				if (!rightConfiguration.split && rightConfiguration.bottomFitness == ERightWeldFitness::Fit)
				{
					switch (rightConfiguration.topFitness)
					{
						case ERightWeldFitness::Fit:
						{
							_left.top ().setRightNeighbors (nullptr);
							Trapezoid::link (_left.top (), _right.compact ());
							return;
						}
						case ERightWeldFitness::Extended:
						{
							_left.bottom ().setRightNeighbors (&_right.top ());
							_left.top ().setRightNeighbors (nullptr);
							_right.compact ().lowerLeftNeighbor () = &_left.bottom ();
							return;
						}
					}
				}
			}
			break;
		}
		assert (false);
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
	typename TrapezoidalMap<Scalar>::Pair TrapezoidalMap<Scalar>::incrementalSplitHorizontally (Trapezoid &_trapezoid, const Segment &_segment, Pair _previous)
	{
		assert (_previous.isRightAligned ());
		assert (Geometry::areSegmentPointsHorizzontallySorted (_segment));
		// Create new trapezoids
		Trapezoid &bottom { createTrapezoid (_trapezoid) }, &top { createTrapezoid (_trapezoid) };
		bottom.top () = top.bottom () = &_segment;
		// Replace old trapezoid
		_trapezoid.lowerLeftNeighbor ()->replaceRightNeighbor (&_trapezoid, &bottom);
		_trapezoid.upperLeftNeighbor ()->replaceRightNeighbor (&_trapezoid, &top);
		_trapezoid.lowerRightNeighbor ()->replaceLeftNeighbor (&_trapezoid, &bottom);
		_trapezoid.upperRightNeighbor ()->replaceLeftNeighbor (&_trapezoid, &top);
		weld (_previous, { bottom, top });
		// Update DAG
		splitTrapezoid (_trapezoid, _segment, top, bottom);
		return { bottom, top };
	}

	template<class Scalar>
	Trapezoid<Scalar> &TrapezoidalMap<Scalar>::mergeLeft (Trapezoid &_trapezoid)
	{

	}

}