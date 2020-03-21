#pragma once

#include "trapezoid.hpp"
#include <utils/geometry_utils.hpp>

namespace GAS
{

	template<class Scalar>
	void Trapezoid<Scalar>::link (Trapezoid &_left, Trapezoid &_right)
	{
		_left.setRightNeighbors (&_right);
		_right.setLeftNeighbors (&_left);
	}


#ifdef GAS_DRAWING_ENABLE_TRAPEZOID_SERIAL

	template<class Scalar>
	const Utils::Serial &Trapezoid<Scalar>::serial () const
	{
		return m_serial;
	}

#endif

	template<class Scalar>
	const Point<Scalar> *Trapezoid<Scalar>::left () const
	{
		return m_left;
	}

	template<class Scalar>
	const Point<Scalar> *Trapezoid<Scalar>::right () const
	{
		return m_right;
	}

	template<class Scalar>
	const Segment<Scalar> *Trapezoid<Scalar>::bottom () const
	{
		return m_bottom;
	}

	template<class Scalar>
	const Segment<Scalar> *Trapezoid<Scalar>::top () const
	{
		return m_top;
	}

	template<class Scalar>
	const Scalar Trapezoid<Scalar>::leftX () const
	{
		return m_left->x ();
	}

	template<class Scalar>
	const Scalar Trapezoid<Scalar>::rightX () const
	{
		return m_right->x ();
	}

	template<class Scalar>
	const Trapezoid<Scalar> *Trapezoid<Scalar>::lowerLeftNeighbor () const
	{
		return m_lowerLeftNeighbor;
	}

	template<class Scalar>
	const Trapezoid<Scalar> *Trapezoid<Scalar>::upperLeftNeighbor () const
	{
		return m_upperLeftNeighbor;
	}

	template<class Scalar>
	const Trapezoid<Scalar> *Trapezoid<Scalar>::lowerRightNeighbor () const
	{
		return m_lowerRightNeighbor;
	}

	template<class Scalar>
	const Trapezoid<Scalar> *Trapezoid<Scalar>::upperRightNeighbor () const
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
	Point<Scalar> Trapezoid<Scalar>::bottomLeft () const
	{
		return { m_left->x (), Geometry::evalLine (*m_bottom, m_left->x ()) };
	}

	template<class Scalar>
	Point<Scalar> Trapezoid<Scalar>::bottomRight () const
	{
		return { m_right->x (), Geometry::evalLine (*m_bottom, m_right->x ()) };
	}

	template<class Scalar>
	Point<Scalar> Trapezoid<Scalar>::topLeft () const
	{
		return { m_left->x (), Geometry::evalLine (*m_top, m_left->x ()) };
	}

	template<class Scalar>
	Point<Scalar> Trapezoid<Scalar>::topRight () const
	{
		return { m_right->x (), Geometry::evalLine (*m_top, m_right->x ()) };
	}

	template<class Scalar>
	Point<Scalar> Trapezoid<Scalar>::centroid () const
	{
		return (bottomLeft () + bottomRight () + topLeft () + topRight ()) / 4;
	}

	template<class Scalar>
	bool Trapezoid<Scalar>::contains (const Point &_point) const
	{
		return _point.x () >= leftX () && _point.x () < rightX () &&
			Geometry::getPointSideWithSegment (*m_top, _point) == Geometry::ESide::Right &&
			Geometry::getPointSideWithSegment (*m_bottom, _point) == Geometry::ESide::Left;
	}

	template<class Scalar>
	void Trapezoid<Scalar>::setLeftNeighbors (Trapezoid *_trapezoid)
	{
		m_lowerLeftNeighbor = m_upperLeftNeighbor = _trapezoid;
	}

	template<class Scalar>
	void Trapezoid<Scalar>::setRightNeighbors (Trapezoid *_trapezoid)
	{
		m_lowerRightNeighbor = m_upperRightNeighbor = _trapezoid;
	}

	template<class Scalar>
	void Trapezoid<Scalar>::setLeftNeighbors (Trapezoid *_bottom, Trapezoid *_top)
	{
		m_lowerLeftNeighbor = _bottom;
		m_upperLeftNeighbor = _top;
	}

	template<class Scalar>
	void Trapezoid<Scalar>::setRightNeighbors (Trapezoid *_bottom, Trapezoid *_top)
	{
		m_lowerRightNeighbor = _bottom;
		m_upperRightNeighbor = _top;
	}

	template<class Scalar>
	void Trapezoid<Scalar>::replaceInNeighbors (Trapezoid *_replacement)
	{
		replaceInLeftNeighbors (_replacement);
		replaceInRightNeighbors (_replacement);
	}

	template<class Scalar>
	void Trapezoid<Scalar>::replaceInLeftNeighbors (Trapezoid *_replacement)
	{
		if (m_lowerLeftNeighbor)
		{
			m_lowerLeftNeighbor->replaceRightNeighbor (this, _replacement);
		}
		if (m_upperLeftNeighbor)
		{
			m_upperLeftNeighbor->replaceRightNeighbor (this, _replacement);
		}
	}

	template<class Scalar>
	void Trapezoid<Scalar>::replaceInRightNeighbors (Trapezoid *_replacement)
	{
		if (m_lowerRightNeighbor)
		{
			m_lowerRightNeighbor->replaceLeftNeighbor (this, _replacement);
		}
		if (m_upperRightNeighbor)
		{
			m_upperRightNeighbor->replaceLeftNeighbor (this, _replacement);
		}
	}

	template<class Scalar>
	void Trapezoid<Scalar>::replaceLeftNeighbor (const Trapezoid *_replaced, Trapezoid *_replacement)
	{
		if (m_lowerLeftNeighbor == _replaced)
		{
			m_lowerLeftNeighbor = _replacement;
		}
		if (m_upperLeftNeighbor == _replaced)
		{
			m_upperLeftNeighbor = _replacement;
		}
	}

	template<class Scalar>
	void Trapezoid<Scalar>::replaceRightNeighbor (const Trapezoid *_replaced, Trapezoid *_replacement)
	{
		if (m_lowerRightNeighbor == _replaced)
		{
			m_lowerRightNeighbor = _replacement;
		}
		if (m_upperRightNeighbor == _replaced)
		{
			m_upperRightNeighbor = _replacement;
		}
	}

}