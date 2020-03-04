#pragma once

#include "point.hpp"
#include "segment.hpp"

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

	public:

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
