#pragma once

#include "trapezoid.hpp"
#include <utils/geometry_utils.hpp>

namespace GAS
{

#ifdef GAS_DRAWING_ENABLE_TRAPEZOID_SERIAL

	template<class Scalar>
	int Trapezoid<Scalar>::s_serial {};

	template<class Scalar>
	int Trapezoid<Scalar>::getSerial () const
	{
		return m_serial;
	}

	template<class Scalar>
	Trapezoid<Scalar>::Trapezoid (const Trapezoid &_clone) :
		m_left { _clone.left }, m_right { _clone.right }, m_top { _clone.top }, m_bottom { _clone.bottom },
		m_lowerLeftNeighbor { _clone.m_lowerLeftNeighbor }, m_upperLeftNeighbor { _clone.m_upperLeftNeighbor },
		m_lowerRightNeighbor { _clone.m_lowerRightNeighbor }, m_upperRightNeighbor { _clone.m_upperRightNeighbor }
	{}

	template<class Scalar>
	Trapezoid<Scalar> &Trapezoid<Scalar>::operator= (const Trapezoid &_clone)
	{
		m_left = _clone.m_left;
		m_right = _clone.m_right;
		m_top = _clone.m_top;
		m_bottom = _clone.m_bottom;
		m_lowerLeftNeighbor = _clone.m_lowerLeftNeighbor;
		m_upperLeftNeighbor = _clone.m_upperLeftNeighbor;
		m_lowerRightNeighbor = _clone.m_lowerRightNeighbor;
		m_upperRightNeighbor = _clone.m_upperRightNeighbor;
		return *this;
	}

#endif

	template<class Scalar>
	const Point<Scalar> *Trapezoid<Scalar>::getLeft () const
	{
		return m_left;
	}

	template<class Scalar>
	const Point<Scalar> *Trapezoid<Scalar>::getRight () const
	{
		return m_right;
	}

	template<class Scalar>
	const Segment<Scalar> *Trapezoid<Scalar>::getBottom () const
	{
		return m_bottom;
	}

	template<class Scalar>
	const Segment<Scalar> *Trapezoid<Scalar>::getTop () const
	{
		return m_top;
	}

	template<class Scalar>
	const Trapezoid<Scalar> *Trapezoid<Scalar>::getLowerLeftNeighbor () const
	{
		return m_lowerLeftNeighbor;
	}

	template<class Scalar>
	const Trapezoid<Scalar> *Trapezoid<Scalar>::getUpperLeftNeighbor () const
	{
		return m_upperLeftNeighbor;
	}

	template<class Scalar>
	const Trapezoid<Scalar> *Trapezoid<Scalar>::getLowerRightNeighbor () const
	{
		return m_lowerRightNeighbor;
	}

	template<class Scalar>
	const Trapezoid<Scalar> *Trapezoid<Scalar>::getUpperRightNeighbor () const
	{
		return m_upperRightNeighbor;
	}

	template<class Scalar>
	const Point<Scalar> *&Trapezoid<Scalar>::left ()
	{
		return m_left;
	}

	template<class Scalar>
	const Point<Scalar> *&Trapezoid<Scalar>::right ()
	{
		return m_right;
	}

	template<class Scalar>
	const Segment<Scalar> *&Trapezoid<Scalar>::bottom ()
	{
		return m_bottom;
	}

	template<class Scalar>
	const Segment<Scalar> *&Trapezoid<Scalar>::top ()
	{
		return m_top;
	}

	template<class Scalar>
	Trapezoid<Scalar> *&Trapezoid<Scalar>::lowerLeftNeighbor ()
	{
		return m_lowerLeftNeighbor;
	}

	template<class Scalar>
	Trapezoid<Scalar> *&Trapezoid<Scalar>::upperLeftNeighbor ()
	{
		return m_upperLeftNeighbor;
	}

	template<class Scalar>
	Trapezoid<Scalar> *&Trapezoid<Scalar>::lowerRightNeighbor ()
	{
		return m_lowerRightNeighbor;
	}

	template<class Scalar>
	Trapezoid<Scalar> *&Trapezoid<Scalar>::upperRightNeighbor ()
	{
		return m_upperRightNeighbor;
	}

	template<class Scalar>
	Point<Scalar> Trapezoid<Scalar>::getBottomLeft () const
	{
		return { m_left->x (), Geometry::evalLine (*m_bottom, m_left->x ()) };
	}

	template<class Scalar>
	Point<Scalar> Trapezoid<Scalar>::getBottomRight () const
	{
		return { m_right->x (), Geometry::evalLine (*m_bottom, m_right->x ()) };
	}

	template<class Scalar>
	Point<Scalar> Trapezoid<Scalar>::getTopLeft () const
	{
		return { m_left->x (), Geometry::evalLine (*m_top, m_left->x ()) };
	}

	template<class Scalar>
	Point<Scalar> Trapezoid<Scalar>::getTopRight () const
	{
		return { m_right->x (), Geometry::evalLine (*m_top, m_right->x ()) };
	}

	template<class Scalar>
	Point<Scalar> Trapezoid<Scalar>::getCentroid () const
	{
		return (getBottomLeft () + getBottomRight () + getTopLeft () + getTopRight ()) / 4;
	}

	template<class Scalar>
	void Trapezoid<Scalar>::replaceInLeftNeighbors (Trapezoid *_replacement)
	{
		if (m_lowerLeftNeighbor)
		{
			m_lowerLeftNeighbor->replaceRightNeighbor (*this, _replacement);
		}
		if (m_upperLeftNeighbor)
		{
			m_upperLeftNeighbor->replaceRightNeighbor (*this, _replacement);
		}
	}

	template<class Scalar>
	void Trapezoid<Scalar>::replaceInRightNeighbors (Trapezoid *_replacement)
	{
		if (m_lowerRightNeighbor)
		{
			m_lowerRightNeighbor->replaceLeftNeighbor (*this, _replacement);
		}
		if (m_upperRightNeighbor)
		{
			m_upperRightNeighbor->replaceLeftNeighbor (*this, _replacement);
		}
	}

	template<class Scalar>
	void Trapezoid<Scalar>::replaceLeftNeighbor (const Trapezoid &_replaced, Trapezoid *_replacement)
	{
		if (m_lowerLeftNeighbor == &_replaced)
		{
			m_lowerLeftNeighbor = _replacement;
		}
		if (m_upperLeftNeighbor == &_replaced)
		{
			m_upperLeftNeighbor = _replacement;
		}
	}

	template<class Scalar>
	void Trapezoid<Scalar>::replaceRightNeighbor (const Trapezoid &_replaced, Trapezoid *_replacement)
	{
		if (m_lowerRightNeighbor == &_replaced)
		{
			m_lowerRightNeighbor = _replacement;
		}
		if (m_upperRightNeighbor == &_replaced)
		{
			m_upperRightNeighbor = _replacement;
		}
	}

}