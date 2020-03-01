#pragma once

#include <type_traits>
#include <cg3/geometry/point2.h>
#include <cg3/geometry/segment2.h>
#include "trapezoid.hpp"

namespace GAS
{

	namespace TrapezoidalDAG
	{

		template<class Scalar>
		using Point = cg3::Point2<Scalar>;

		template<class Scalar>
		using Segment = cg3::Segment2<Scalar>;

		enum class ENodeType
		{
			VerticalSplit, NonVerticalSplit, Trapezoid
		};

		enum class EPlane
		{
			Left, Right
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
		using QueryDisambiguator = EPlane (*)(ENodeType type, const Splitter<Scalar> & splitter, const Point<Scalar> & point);

		template<class Scalar>
		EPlane disambiguateAlwaysRight (ENodeType type, const Splitter<Scalar> &splitter, const Point<Scalar> &point);

		template<class Scalar>
		Trapezoid<Scalar> *query (const Node<Scalar> &dag, const Point<Scalar> &point, QueryDisambiguator<Scalar> disambiguator = disambiguateAlwaysRight<Scalar>);


	}

}

#include "trapezoidal_dag.tpp"