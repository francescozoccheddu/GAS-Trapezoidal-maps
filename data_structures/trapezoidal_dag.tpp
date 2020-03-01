#pragma once

#include "trapezoidal_dag.hpp"
#include <stdexcept>

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
		Geometry::EPolarizedSide disambiguateAlwaysRight (ENodeType _type, const Splitter<Scalar> &_splitter, const Point<Scalar> &_point)
		{
			return Geometry::EPolarizedSide::Right;
		}


		template<class Scalar>
		Trapezoid<Scalar> *query (const Node<Scalar> &_dag, const Point<Scalar> &_point, QueryDisambiguator<Scalar> _disambiguator)
		{
			const Node<Scalar> *node { &_dag };
			while (node->type != ENodeType::Trapezoid)
			{
				const Geometry::EPolarizedSide side { Internals::getPointPolarizedSide (node->type, node->data.split.splitter, _point, _disambiguator) };
				node = side == Geometry::EPolarizedSide::Right ? node->data.split.right : node->data.split.left;
			}
			return node->data.trapezoid;
		}

		namespace Internals
		{

			template<class Scalar>
			inline Geometry::ESide getPointSide (ENodeType _type, const Splitter<Scalar> &_splitter, const Point<Scalar> &_point)
			{
				switch (_type)
				{
					case ENodeType::VerticalSplit:
						return Geometry::getPointSideWithRespectToVerticalLine (*_splitter.x, _point);
					case ENodeType::NonVerticalSplit:
						return Geometry::getPointSideWithRespectToSegment (*_splitter.segment, _point);
					default:
						throw std::domain_error ("Unexpected node type");
				}
			}

			template<class Scalar>
			inline Geometry::EPolarizedSide getPointPolarizedSide (ENodeType _type, const Splitter<Scalar> &_splitter, const Point<Scalar> &_point, QueryDisambiguator<Scalar> _disambiguator)
			{
				switch (getPointSide (_type, _splitter, _point))
				{
					case Geometry::ESide::Left:
						return Geometry::EPolarizedSide::Left;
					case Geometry::ESide::Right:
						return Geometry::EPolarizedSide::Right;
					case Geometry::ESide::Collinear:
						return _disambiguator (_type, _splitter, _point);
					default:
						throw std::domain_error ("Unexpected point side");
				}
			}

		}

	}

}