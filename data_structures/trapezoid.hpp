#pragma once

#include "point.hpp"
#include "segment.hpp"

// For debugging purposes
#define GAS_DRAWING_ENABLE_TRAPEZOID_SERIAL

namespace GAS
{

	template<class Scalar>
	class TrapezoidalMap;

	template<class Scalar>
	class Trapezoid final
	{

		void replaceLeft (const Trapezoid &target, Trapezoid *substitute);
		void replaceRight (const Trapezoid &target, Trapezoid *substitute);
		static void link (Trapezoid &bottomLeft, Trapezoid &topLeft, Trapezoid &bottomRight, Trapezoid &topRight);
		static void replaceAndLinkLeft (Trapezoid &old, Trapezoid &bottom, Trapezoid &top);
		static void replaceAndLinkRight (Trapezoid &old, Trapezoid &bottom, Trapezoid &top);

		friend class TrapezoidalMap<Scalar>;

#ifdef GAS_DRAWING_ENABLE_TRAPEZOID_SERIAL
		static int s_serial;
		int m_serial { s_serial++ };
#endif

	public:

#ifdef GAS_DRAWING_ENABLE_TRAPEZOID_SERIAL
		int getSerial () const;
#endif

		const Point<Scalar> *left {}, *right {};
		const Segment<Scalar> *bottom {}, *top {};

		Trapezoid *lowerLeftNeighbor {}, *upperLeftNeighbor {}, *lowerRightNeighbor {}, *upperRightNeighbor {};
		~Trapezoid ();

		Point<Scalar> getBottomLeft () const;
		Point<Scalar> getBottomRight () const;
		Point<Scalar> getTopLeft () const;
		Point<Scalar> getTopRight () const;
		Point<Scalar> getCentroid () const;

	};

}

#include "trapezoid.tpp"
