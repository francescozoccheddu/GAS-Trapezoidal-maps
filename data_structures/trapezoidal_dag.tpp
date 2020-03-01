#pragma once

#include "trapezoidal_dag.hpp"
#include <cassert>

namespace GAS
{

	namespace TrapezoidalDAG
	{

		template<class Scalar>
		Node<Scalar>::Node (Trapezoid<Scalar> *_trapezoid) : type { ENodeType::Trapezoid }
		{
			data.trapezoid = _trapezoid;
		}

		template<class Scalar>
		Node<Scalar>::Node (const Scalar *_splitX, Node<Scalar> *_left, Node<Scalar> *_right) : type { ENodeType::VerticalSplit }
		{
			data.split.splitter.x = _splitX;
			data.split.left = _left;
			data.split.right = _right;
		}

		template<class Scalar>
		Node<Scalar>::Node (const Segment<Scalar> *_splitSegment, Node<Scalar> *_left, Node<Scalar> *_right) : type { ENodeType::NonVerticalSplit }
		{
			data.split.splitter.segment = _splitSegment;
			data.split.left = _left;
			data.split.right = _right;
		}

		template<class Scalar>
		EPlane disambiguateAlwaysRight (ENodeType _type, const Splitter<Scalar> &_splitter, const Point<Scalar> &_point)
		{
			return EPlane::Right;
		}

		namespace Internals
		{

			template<class Scalar>
			inline Scalar getPointSide (const Segment<Scalar> &_segment, const Point<Scalar> &_point)
			{
				const Point<Scalar> &a { _segment.p1 () }, &b { _segment.p2 () };
				return (b.x () - a.x ()) * (_point.y () - a.y ()) - (b.y () - a.y ()) * (_point.x () - a.x ());
			}

			template<class Scalar>
			inline Scalar getPointSide (ENodeType _type, const Splitter<Scalar> &_splitter, const Point<Scalar> &_point)
			{
				switch (_type)
				{
					case ENodeType::VerticalSplit:
						return _point.x () - *_splitter.x;
					case ENodeType::NonVerticalSplit:
						return getPointSide (*_splitter.segment, _point);
					default:
						assert (false);
				}
			}

			template<class Scalar>
			inline EPlane getPointSide (ENodeType _type, const Splitter<Scalar> &_splitter, const Point<Scalar> &_point, QueryDisambiguator<Scalar> _disambiguator)
			{
				const Scalar side { getPointSide (_type, _splitter, _point) };
				return side > 0 ? EPlane::Right : side < 0 ? EPlane::Left : _disambiguator (_type, _splitter, _point);
			}

		}

		template<class Scalar>
		Trapezoid<Scalar> *query (const Node<Scalar> &_dag, const Point<Scalar> &_point, QueryDisambiguator<Scalar> _disambiguator)
		{
			const Node<Scalar> *node { &_dag };
			while (node->type != ENodeType::Trapezoid)
			{
				const EPlane plane { Internals::getPointSide (node->type, node->data.split.splitter, _point, _disambiguator) };
				node = plane == EPlane::Right ? node->data.split.right : node->data.split.left;
			}
			return node->data.trapezoid;
		}

	}

}