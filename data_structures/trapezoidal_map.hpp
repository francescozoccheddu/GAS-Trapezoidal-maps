#pragma once

#include "point.hpp"
#include "segment.hpp"
#include "trapezoid.hpp"
#include "trapezoidal_dag.hpp"
#include <forward_list>
#include <deque>

namespace GAS
{

	template<class Scalar>
	class TrapezoidalMap
	{

	public:

		using Point = Point<Scalar>;
		using Segment = Segment<Scalar>;
		using Trapezoid = Trapezoid<Scalar>;
		using Node = TDAG::Node<Scalar>;

		class Iterator final
		{

			std::deque<Trapezoid *> m_deque;
			Trapezoid *m_current {};

			explicit Iterator (Trapezoid *node);

		public:

			static const Iterator end;
			static Iterator fromTrapezoidalMap (const TrapezoidalMap &trapezoidalMap);

			Iterator &operator++();
			const Trapezoid &operator*() const;
			const Trapezoid *operator->() const;

			bool operator!=(const Iterator &other) const;

			bool hasNext () const;

		};

	private:

		struct HorizontalSplit
		{
			Trapezoid &bottom, &top;
			HorizontalSplit (Trapezoid &bottom, Trapezoid &top);
		};

		std::forward_list<Segment> m_segments;
		Node *m_dag {};
		int m_trapezoidsCount;
		Trapezoid *m_leftmostTrapezoid {};

		Segment m_bottom, m_top;

		Node &splitVertically (Trapezoid &trapezoid, const Point &point);

		HorizontalSplit &splitHorizontallyAndLink (Trapezoid &trapezoid, const Segment &segment, HorizontalSplit leftNeighbors);

	public:

		TrapezoidalMap (const Point &bottomLeft, const Point &topRight);
		virtual ~TrapezoidalMap () = default;

		// Trapezoids
		int getTrapezoidsCount () const;
		Iterator begin () const;
		const Iterator &end () const;

		// Bounds
		const Point &getBottomLeft () const;
		const Point &getBottomRight () const;
		const Point &getTopLeft () const;
		const Point &getTopRight () const;
		const Segment &getTop () const;
		const Segment &getBottom () const;
		Scalar getLeftX () const;
		Scalar getRightX () const;
		Scalar getBottomY () const;
		Scalar getTopY () const;
		void setBounds (const Point &bottomLeft, const Point &topRight);

		// Segments
		const std::vector<Segment> &getSegments () const;
		void clear ();
		void addSegment (const Segment &segment);

	};

}

#include "trapezoidal_map.tpp"