#pragma once

#include "point.hpp"
#include "segment.hpp"
#include "trapezoid.hpp"
#include "binary_dag.hpp"
#include <utils/geometry_utils.hpp>
#include <utils/bivariant.hpp>
#include <utils/iterator_utils.hpp>

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
				const Scalar *m_x;
				const Segment<Scalar> *m_segment;
			};

		public:

			Split (const Scalar &x);
			Split (const Segment<Scalar> &segment);

			ESplitType type () const;
			const Scalar &x () const;
			const Segment<Scalar> &segment () const;

			void setVertical (const Scalar &x);
			void setNonVertical (const Segment<Scalar> &segment);

		};

		template<class Scalar>
		using NodeData = GAS::Utils::BiVariant<Split<Scalar>, Trapezoid<Scalar>>;

		template<class Scalar>
		using Node = BDAG::Node<NodeData<Scalar>>;

		template<class Scalar>
		using Graph = BDAG::Graph<NodeData<Scalar>>;

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

			template<class Scalar>
			const Trapezoid<Scalar> &getTrapezoid (const typename Graph<Scalar>::ConstLeafNodeIterator &iterator);

			template<class Scalar>
			using ConstTrapezoidIterator = GAS::Utils::IteratorAdapter <typename Graph<Scalar>::ConstLeafNodeIterator, const Trapezoid<Scalar>, getTrapezoid<Scalar>>;

		}

	}

}

#include "trapezoidal_dag.tpp"