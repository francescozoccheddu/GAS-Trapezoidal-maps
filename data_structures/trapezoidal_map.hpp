#pragma once

#include <type_traits>
#include <cg3/geometry/point2.h>
#include <cg3/geometry/segment2.h>
#include <vector>
#include "trapezoid.hpp"
#include "trapezoidal_dag.hpp"

namespace GAS
{

	template<class Scalar, typename = typename std::enable_if<std::is_arithmetic<Scalar>::value, Scalar>::type>
	class TrapezoidalMap
	{

	private:

		using Point = cg3::Point2<Scalar>;
		using Segment = cg3::Segment2<Scalar>;

		std::vector<Segment> m_segments;
		std::vector<Trapezoid> m_trapezoids;
		TrapezoidalDAG::Node m_dag;

	public:

		const std::vector<Segment>& getSegments () const;
		const std::vector<Trapezoid>& getTrapezoids () const;
		void clear ();
		void addSegment (const Segment& segment);

	};

}