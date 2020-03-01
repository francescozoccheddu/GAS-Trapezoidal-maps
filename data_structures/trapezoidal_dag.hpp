#pragma once

#include "point.hpp"
#include "segment.hpp"
#include "trapezoid.hpp"
#include <utils/geometry_utils.hpp>

namespace GAS
{

	namespace TrapezoidalDAG
	{

		enum class ENodeType
		{
			VerticalSplit, NonVerticalSplit, Trapezoid
		};

		template<class Scalar>
		union Splitter
		{
			const Scalar *x;
			const Segment<Scalar> *segment;
		};

		template<class Scalar>
		struct Node final
		{

			Node () = default;
			Node (Trapezoid<Scalar> *trapezoid);
			Node (const Scalar *splitX, Node<Scalar> *left = nullptr, Node<Scalar> *right = nullptr);
			Node (const Segment<Scalar> *splitSegment, Node<Scalar> *left = nullptr, Node<Scalar> *right = nullptr);

			ENodeType type { ENodeType::Trapezoid };

			union
			{
				Trapezoid<Scalar> *trapezoid {};
				struct
				{
					Node *left;
					Node *right;
					Splitter<Scalar> splitter;
				} split;
			} data {};

		};

		template<class Scalar>
		using QueryDisambiguator = Geometry::EPolarizedSide (*)(ENodeType type, const Splitter<Scalar> & splitter, const Point<Scalar> & point);

		template<class Scalar>
		Geometry::EPolarizedSide disambiguateAlwaysRight (ENodeType type, const Splitter<Scalar> &splitter, const Point<Scalar> &point);

		template<class Scalar>
		Trapezoid<Scalar> *query (const Node<Scalar> &dag, const Point<Scalar> &point, QueryDisambiguator<Scalar> disambiguator = disambiguateAlwaysRight<Scalar>);

		namespace Internals
		{
			template<class Scalar>
			inline Geometry::ESide getPointSide (ENodeType type, const Splitter<Scalar> &splitter, const Point<Scalar> &point);

			template<class Scalar>
			inline Geometry::EPolarizedSide getPointPolarizedSide (ENodeType _type, const Splitter<Scalar> &splitter, const Point<Scalar> &point, QueryDisambiguator<Scalar> disambiguator);
		}

	}

}

#include "trapezoidal_dag.tpp"