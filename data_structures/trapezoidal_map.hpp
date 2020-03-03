#pragma once

#include "point.hpp"
#include "segment.hpp"
#include "trapezoid.hpp"
#include "trapezoidal_dag.hpp"
#include <vector>
#include <deque>

namespace GAS
{

	template<class Scalar>
	class TrapezoidalMap
	{

	public:

		class Iterator
		{

			friend class TrapezoidalMap;

			Iterator (const Trapezoid<Scalar> *trapezoid);

			std::deque<const Trapezoid<Scalar> *> m_deque;
			const Trapezoid<Scalar> *m_current;

		public:

			Iterator &operator++();
			const Trapezoid<Scalar> &operator*() const;
			const Trapezoid<Scalar> *operator->() const;

			bool operator!=(const Iterator &other) const;

		};

	private:

		std::vector<Segment<Scalar>> m_segments;
		TrapezoidalDAG::Node<Scalar> m_dag;
		int m_trapezoidsCount;
		Trapezoid<Scalar> *m_leftmostTrapezoid {};
		const Iterator m_end { nullptr };

		Segment<Scalar> m_bottom, m_top;

		void splitFive (Trapezoid<Scalar> &trapezoid, const Segment<Scalar> segment);
		void splitLeft (Trapezoid<Scalar> &trapezoid, const Segment<Scalar> segment);
		void splitHalf (Trapezoid<Scalar> &trapezoid, const Segment<Scalar> segment);
		void splitRight (Trapezoid<Scalar> &trapezoid, const Segment<Scalar> segment);
		void mergeLeft (Trapezoid<Scalar> &trapezoid);

	public:

		TrapezoidalMap (const Point<Scalar> &bottomLeft, const Point<Scalar> &topRight);
		virtual ~TrapezoidalMap () = default;

		// Trapezoids
		int getTrapezoidsCount () const;
		Iterator begin () const;
		const Iterator &end () const;

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