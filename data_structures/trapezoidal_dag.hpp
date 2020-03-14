#pragma once

#include "point.hpp"
#include "segment.hpp"
#include "trapezoid.hpp"
#include "binary_dag.hpp"
#include <utils/geometry_utils.hpp>

namespace GAS
{

	namespace TDAG
	{

		enum class ESplitType
		{
			Vertical, NonVertical
		};

		template<class Scalar>
		class Split
		{

			ESplitType m_type;

			union
			{
				const Scalar *x;
				const Segment<Scalar> *segment;
			} m_data;

		public:

			Split (const Scalar &x);
			Split (const Segment<Scalar> &segment);

			ESplitType getType () const;

			const Scalar &getX () const;
			const Segment<Scalar> &getSegment () const;

			void setVertical (const Scalar &x);
			void setNonVertical (const Segment<Scalar> &segment);

		};

		template<class Scalar>
		using Node = BDAG::Node<Trapezoid<Scalar>, Split<Scalar>>;

		using EChild = BDAG::EChild;

		template<class Scalar>
		EChild disambiguateAlwaysRight (const Split<Scalar> &split, const Point<Scalar> &point);

		template<class Scalar, class Disambiguator>
		inline const Trapezoid<Scalar> &query (const Node<Scalar> &root, const Point<Scalar> &point, Disambiguator disambiguator);

		template<class Scalar, class Disambiguator>
		inline Trapezoid<Scalar> &query (Node<Scalar> &root, const Point<Scalar> &point, Disambiguator disambiguator);

		template<class Scalar>
		inline const Trapezoid<Scalar> &query (const Node<Scalar> &root, const Point<Scalar> &point);

		template<class Scalar>
		inline Trapezoid<Scalar> &query (Node<Scalar> &root, const Point<Scalar> &point);

		namespace Utils
		{

			template<class Scalar>
			Geometry::ESide getPointSide (const Split<Scalar> &split, const Point<Scalar> &point);

			template<class Scalar, class Disambiguator>
			inline EChild getPointQueryNextChild (const Split<Scalar> &split, const Point<Scalar> &point, Disambiguator disambiguator);

		}

	}

}

#include "trapezoidal_dag.tpp"