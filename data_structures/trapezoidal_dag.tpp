#pragma once

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
		ESplitType Split<Scalar>::getType () const
		{
			return m_type;
		}

		template<class Scalar>
		const Scalar &Split<Scalar>::getX () const
		{
			assert (m_type == ESplitType::Vertical);
			return *m_data.x;
		}

		template<class Scalar>
		const Segment<Scalar> &Split<Scalar>::getSegment () const
		{
			assert (m_type == ESplitType::NonVertical);
			return *m_data.segment;
		}

		template<class Scalar>
		void Split<Scalar>::setVertical (const Scalar &_x)
		{
			m_type = ESplitType::Vertical;
			m_data.x = &_x;
		}

		template<class Scalar>
		void Split<Scalar>::setNonVertical (const Segment<Scalar> &_segment)
		{
			m_type = ESplitType::NonVertical;
			m_data.segment = &_segment;
		}

		template<class Scalar>
		EChild disambiguateAlwaysRight (const Split<Scalar> &_split, const Point<Scalar> &_point)
		{
			return EChild::Right;
		}

		template<class Scalar, class Disambiguator>
		const Trapezoid<Scalar> &query (const Node<Scalar> &_root, const Point<Scalar> &_point, Disambiguator _disambiguator)
		{
			return BDAG::walk (_root, [](const Split<Scalar> &_split) {
				return Utils::getPointQueryNextChild (_split, _point, _disambiguator);
			}).getLeafData ();
		}

		template<class Scalar, class Disambiguator>
		Trapezoid<Scalar> &query (Node<Scalar> &_root, const Point<Scalar> &_point, Disambiguator _disambiguator)
		{
			return BDAG::walk (_root, [](const Split<Scalar> &_split) {
				return Utils::getPointQueryNextChild (_split, _point, _disambiguator);
			}).leafData ();
		}

		template<class Scalar>
		const Trapezoid<Scalar> &query (const Node<Scalar> &_root, const Point<Scalar> &_point)
		{
			return query (_root, _point, disambiguateAlwaysRight);
		}

		template<class Scalar>
		Trapezoid<Scalar> &query (Node<Scalar> &_root, const Point<Scalar> &_point)
		{
			return query (_root, _point, disambiguateAlwaysRight);
		}

		namespace Utils
		{

			template<class Scalar>
			Geometry::ESide getPointSide (const Split<Scalar> &_split, const Point<Scalar> &_point)
			{
				switch (_split.getType ())
				{
					default:
						assert (false);
					case ESplitType::Vertical:
						return Geometry::getPointSideWithVerticalLine (_split.getX (), _point);
					case ESplitType::NonVertical:
						return Geometry::getPointSideWithSegment (_split.getSegment (), _point);
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

		}

	}

}