#pragma once

#include <type_traits>
#include <cg3/geometry/point2.h>
#include <cg3/geometry/segment2.h>
#include <vector>
#include "trapezoid.hpp"
#include "trapezoidal_dag.hpp"

namespace GAS
{

	template<class Scalar>
	class TrapezoidalMap
	{

	private:

		using Point = cg3::Point2<Scalar>;
		using Segment = cg3::Segment2<Scalar>;
		using Trapezoid = Trapezoid<Scalar>;

		std::vector<Segment> m_segments;
		std::vector<Trapezoid> m_trapezoids;
		TrapezoidalDAG::Node m_dag;

		Segment m_bottom {}, m_top {};

	public:

		TrapezoidalMap ();
		virtual ~TrapezoidalMap = default;

		// Trapezoids
		const std::vector<Trapezoid> &getTrapezoids () const;

		// Bounds
		const Segment &getBottomLeft () const;
		const Segment &getBottomRight () const;
		const Segment &getTopLeft () const;
		const Segment &getTopRight () const;
		const Point &getTop () const;
		const Point &getBottom () const;
		Scalar getLeftX () const;
		Scalar getRightX () const;
		Scalar getBottomY () const;
		Scalar getTopY () const;
		void setBounds (Scalar leftX, Scalar rightX, Scalar bottomY, Scalar topY);
		void setBounds (const Point &bottomLeft, const Point &topRight);

		// Segments
		const std::vector<Segment> &getSegments () const;
		void clear ();
		void addSegment (const Segment &segment);

	};

}