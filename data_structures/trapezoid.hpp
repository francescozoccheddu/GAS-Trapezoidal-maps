#pragma once

#include "point.hpp"
#include "segment.hpp"

// For debugging purposes
#define GAS_DRAWING_ENABLE_TRAPEZOID_SERIAL

namespace GAS
{

	template<class Scalar>
	class Trapezoid final
	{

	public:

		using Point = Point<Scalar>;
		using Segment = Segment<Scalar>;

	private:

#ifdef GAS_DRAWING_ENABLE_TRAPEZOID_SERIAL
		static int s_serial;
		int m_serial { s_serial++ };
#endif

		const Point *m_left {}, *m_right {};
		const Segment *m_bottom {}, *m_top {};
		Trapezoid *m_lowerLeftNeighbor {}, *m_upperLeftNeighbor {}, *m_lowerRightNeighbor {}, *m_upperRightNeighbor {};

	public:

		Trapezoid () = default;

#ifdef GAS_DRAWING_ENABLE_TRAPEZOID_SERIAL
		int getSerial () const;

		Trapezoid (const Trapezoid &clone);
		Trapezoid &operator=(const Trapezoid &clone);
#else
		Trapezoid (const Trapezoid &clone) = default;
#endif

		const Point *getLeft () const;
		const Point *getRight () const;
		const Segment *getBottom () const;
		const Segment *getTop () const;

		const Trapezoid *getLowerLeftNeighbor () const;
		const Trapezoid *getUpperLeftNeighbor () const;
		const Trapezoid *getLowerRightNeighbor () const;
		const Trapezoid *getUpperRightNeighbor () const;

		const Point *&left ();
		const Point *&right ();
		const Segment *&bottom ();
		const Segment *&top ();


		Trapezoid *&lowerLeftNeighbor ();
		Trapezoid *&upperLeftNeighbor ();
		Trapezoid *&lowerRightNeighbor ();
		Trapezoid *&upperRightNeighbor ();

		Point getBottomLeft () const;
		Point getBottomRight () const;
		Point getTopLeft () const;
		Point getTopRight () const;
		Point getCentroid () const;

		void replaceInLeftNeighbors (Trapezoid *replacement);
		void replaceInRightNeighbors (Trapezoid *replacement);
		void replaceLeftNeighbor (const Trapezoid &replaced, Trapezoid *replacement);
		void replaceRightNeighbor (const Trapezoid &replaced, Trapezoid *replacement);

	};

}

#include "trapezoid.tpp"
