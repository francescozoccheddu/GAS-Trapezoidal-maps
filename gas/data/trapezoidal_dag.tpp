#ifndef GAS_DATA_TRAPEZOIDAL_DAG_IMPL_INCLUDED
#define GAS_DATA_TRAPEZOIDAL_DAG_IMPL_INCLUDED

#ifndef GAS_DATA_TRAPEZOIDAL_DAG_INCLUDED
#error __FILE__ should not be directly included
#endif

#include "trapezoidal_dag.hpp"

#include <cassert>

namespace GAS
{

	namespace TDAG
	{

		template<class Scalar>
		Split<Scalar>::Split (const Scalar &_x)
		{
			setVertical (_x);
		}

		template<class Scalar>
		Split<Scalar>::Split (const Segment<Scalar> &_segment)
		{
			setNonVertical (_segment);
		}

		template<class Scalar>
		ESplitType Split<Scalar>::type () const
		{
			return m_type;
		}

		template<class Scalar>
		const Scalar &Split<Scalar>::x () const
		{
			assert (m_type == ESplitType::Vertical);
			return *m_x;
		}

		template<class Scalar>
		const Segment<Scalar> &Split<Scalar>::segment () const
		{
			assert (m_type == ESplitType::NonVertical);
			return *m_segment;
		}

		template<class Scalar>
		void Split<Scalar>::setVertical (const Scalar &_x)
		{
			m_type = ESplitType::Vertical;
			m_x = &_x;
		}

		template<class Scalar>
		void Split<Scalar>::setNonVertical (const Segment<Scalar> &_segment)
		{
			m_type = ESplitType::NonVertical;
			m_segment = &_segment;
		}


		template<class Scalar, class Disambiguator>
		const Trapezoid<Scalar> &query (const Node<Scalar> &_root, const Point<Scalar> &_point, Disambiguator _disambiguator)
		{
			return BDAG::walk (_root, [&](const NodeData<Scalar> &_node) {
				return Utils::getPointQueryNextChild (_node.first (), _point, _disambiguator);
			}).data ().second ();
		}

		template<class Scalar, class Disambiguator>
		Trapezoid<Scalar> &query (Node<Scalar> &_root, const Point<Scalar> &_point, Disambiguator _disambiguator)
		{
			return BDAG::walk (_root, [&](const NodeData<Scalar> &_node) {
				return Utils::getPointQueryNextChild (_node.first (), _point, _disambiguator);
			}).data ().second ();
		}

		template<class Scalar>
		const Trapezoid<Scalar> &query (const Node<Scalar> &_root, const Point<Scalar> &_point)
		{
			return query (_root, _point, Utils::disambiguateAlwaysRight<Scalar>);
		}

		template<class Scalar>
		Trapezoid<Scalar> &query (Node<Scalar> &_root, const Point<Scalar> &_point)
		{
			return query (_root, _point, Utils::disambiguateAlwaysRight<Scalar>);
		}

		namespace Utils
		{

			template<class Scalar>
			EChild disambiguateAlwaysRight (const Split<Scalar> &/*_split*/, const Point<Scalar> &/*_point*/)
			{
				return EChild::Right;
			}

			template<class Scalar>
			Geometry::ESide getPointSide (const Split<Scalar> &_split, const Point<Scalar> &_point)
			{
				switch (_split.type ())
				{
					default:
						assert (false);
					case ESplitType::Vertical:
						return Geometry::getPointSideWithVerticalLine (_split.x (), _point);
					case ESplitType::NonVertical:
						return Geometry::getPointSideWithSegment (_split.segment (), _point);
				}
			}

			template<class Scalar, class Disambiguator>
			EChild getPointQueryNextChild (const Split<Scalar> &_split, const Point<Scalar> &_point, Disambiguator _disambiguator)
			{
				switch (getPointSide (_split, _point))
				{
					case Geometry::ESide::Left:
						return EChild::Left;
					case Geometry::ESide::Right:
						return EChild::Right;
					default:
						return _disambiguator (_split, _point);
				}
			}

			template<class Scalar>
			const Trapezoid<Scalar> &getTrapezoid (const typename Graph<Scalar>::ConstLeafNodeIterator &_iterator)
			{
				return _iterator->data ().second ();
			}

		}

	}

}

#endif