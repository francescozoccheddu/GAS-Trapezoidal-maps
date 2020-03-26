#pragma once

#include <data_structures/point.hpp>
#include <data_structures/segment.hpp>

// For debugging purposes
#define GAS_DRAWING_ENABLE_TRAPEZOID_SERIAL

#ifdef GAS_DRAWING_ENABLE_TRAPEZOID_SERIAL
#include <utils/serial.hpp>
#endif


namespace GAS
{

	enum class ETrapezoidSideSource
	{
		Joint, Bottom, Top, External
	};

	template<class Scalar>
	class Trapezoid final
	{

	public:

		using Point = Point<Scalar>;
		using Segment = Segment<Scalar>;

	private:

#ifdef GAS_DRAWING_ENABLE_TRAPEZOID_SERIAL
		Utils::Serial m_serial {};
	public:
		const Utils::Serial &serial () const;
	private:
#endif

		const Point *m_left {}, *m_right {};
		const Segment *m_bottom {}, *m_top {};
		Trapezoid *m_lowerLeftNeighbor {}, *m_upperLeftNeighbor {}, *m_lowerRightNeighbor {}, *m_upperRightNeighbor {};

	public:

		static void link (Trapezoid &left, Trapezoid &right);

		const Point *left () const;
		const Point *right () const;
		const Segment *bottom () const;
		const Segment *top () const;
		const Scalar leftX () const;
		const Scalar rightX () const;

		const Trapezoid *lowerLeftNeighbor () const;
		const Trapezoid *upperLeftNeighbor () const;
		const Trapezoid *lowerRightNeighbor () const;
		const Trapezoid *upperRightNeighbor () const;

		const Point *&left ();
		const Point *&right ();
		const Segment *&bottom ();
		const Segment *&top ();

		Trapezoid *&lowerLeftNeighbor ();
		Trapezoid *&upperLeftNeighbor ();
		Trapezoid *&lowerRightNeighbor ();
		Trapezoid *&upperRightNeighbor ();

		Point bottomLeft () const;
		Point bottomRight () const;
		Point topLeft () const;
		Point topRight () const;
		Point centroid () const;

		bool contains (const Point &point) const;

		bool isJointRight () const;
		bool isJointLeft () const;

		ETrapezoidSideSource leftSource () const;
		ETrapezoidSideSource rightSource () const;

		void setLeftNeighbors (Trapezoid *trapezoid);
		void setRightNeighbors (Trapezoid *trapezoid);
		void setLeftNeighbors (Trapezoid *lower, Trapezoid *upper);
		void setRightNeighbors (Trapezoid *lower, Trapezoid *upper);
		void replaceInNeighbors (Trapezoid *replacement);
		void replaceInLeftNeighbors (Trapezoid *replacement);
		void replaceInRightNeighbors (Trapezoid *replacement);
		void replaceLeftNeighbor (const Trapezoid *replaced, Trapezoid *replacement);
		void replaceRightNeighbor (const Trapezoid *replaced, Trapezoid *replacement);

	};

}

#include "trapezoid.tpp"