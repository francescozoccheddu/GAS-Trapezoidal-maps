#pragma once

#include "point.hpp"
#include "segment.hpp"
#include "trapezoid.hpp"
#include "trapezoidal_dag.hpp"
#include <vector>

namespace GAS
{

	template<class Scalar>
	class TrapezoidalMap
	{

		std::vector<Segment<Scalar>> m_segments;
		std::vector<Trapezoid<Scalar>> m_trapezoids;
		TrapezoidalDAG::Node m_dag;

		Segment<Scalar> m_bottom {}, m_top {};

	public:

		TrapezoidalMap ();
		TrapezoidalMap (const Point<Scalar> &bottomLeft, const Point<Scalar> &topRight);
		virtual ~TrapezoidalMap = default;

		// Trapezoids
		const std::vector<Trapezoid> &getTrapezoids () const;

		// Bounds
		const Segment<Scalar> &getBottomLeft () const;
		const Segment<Scalar> &getBottomRight () const;
		const Segment<Scalar> &getTopLeft () const;
		const Segment<Scalar> &getTopRight () const;
		const Point<Scalar> &getTop () const;
		const Point<Scalar> &getBottom () const;
		Scalar getLeftX () const;
		Scalar getRightX () const;
		Scalar getBottomY () const;
		Scalar getTopY () const;
		void setBounds (const Point<Scalar> &bottomLeft, const Point<Scalar> &topRight);

		// Segments
		const std::vector<Segment<Scalar>> &getSegments () const;
		void clear ();
		void addSegment (const Segment<Scalar> &segment);

	};

}