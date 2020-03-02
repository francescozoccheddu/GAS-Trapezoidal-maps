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
		TrapezoidalDAG::Node<Scalar> m_dag;

		Segment<Scalar> m_bottom, m_top;

	public:

		TrapezoidalMap (const Point<Scalar> &bottomLeft, const Point<Scalar> &topRight);
		virtual ~TrapezoidalMap () = default;

		// Trapezoids
		const std::vector<Trapezoid<Scalar>> &getTrapezoids () const;

		// Bounds
		const Point<Scalar> &getBottomLeft () const;
		const Point<Scalar> &getBottomRight () const;
		const Point<Scalar> &getTopLeft () const;
		const Point<Scalar> &getTopRight () const;
		const Segment<Scalar> &getTop () const;
		const Segment<Scalar> &getBottom () const;
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

#include "trapezoidal_map.tpp"
