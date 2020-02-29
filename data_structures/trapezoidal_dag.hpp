#pragma once

#include <type_traits>
#include <cg3/geometry/point2.h>
#include <cg3/geometry/segment2.h>
#include "trapezoid.hpp"

namespace GAS
{

	namespace TrapezoidalDAG
	{

		template<class Scalar, typename = typename std::enable_if<std::is_arithmetic<Scalar>::value, Scalar>::type>
		using Point = cg3::Point2<Scalar>;

		enum class ENodeType
		{
			VerticalSplit, NonVerticalSplit, Trapezoid
		};

		enum class EPlane
		{
			Left, Right
		};

		template<class Scalar, typename = typename std::enable_if<std::is_arithmetic<Scalar>::value, Scalar>::type>
		struct Node final
		{

			using Segment = cg3::Segment2<Scalar>;

			ENodeType type { ENodeType::Trapezoid };

			union
			{
				Trapezoid *trapezoid;
				struct
				{
					Node *left;
					Node *right;
					union
					{
						const Scalar *x;
						const Segment *segment;
					} splitter;
				} split;
			} data {};

		};

		template<class Scalar, typename = typename std::enable_if<std::is_arithmetic<Scalar>::value, Scalar>::type>
		using QueryDisambiguator = EPlane (*)(const Node<Scalar> & node, const Point<Scalar> & point);

		template<class Scalar, typename = typename std::enable_if<std::is_arithmetic<Scalar>::value, Scalar>::type>
		EPlane disambiguateAlwaysRight (const Node<Scalar> & node, const Point<Scalar> & point);

		template<class Scalar, typename = typename std::enable_if<std::is_arithmetic<Scalar>::value, Scalar>::type>
		Trapezoid<Scalar>* query (const Node<Scalar>& dag, const Point<Scalar>& point, QueryDisambiguator<Scalar> disambiguator = disambiguateAlwaysRight<Scalar>);

	}

}