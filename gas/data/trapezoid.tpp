#ifndef GAS_DATA_TRAPEZOID_IMPL_INCLUDED
#define GAS_DATA_TRAPEZOID_IMPL_INCLUDED

#ifndef GAS_DATA_TRAPEZOID_INCLUDED
#error 'gas/data/trapezoid.tpp' should not be directly included
#endif

#include "trapezoid.hpp"

#include <gas/utils/geometry.hpp>

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
	template<class OutputScalar>
	Point<OutputScalar> Trapezoid<Scalar>::bottomLeft () const
	{
		const OutputScalar &x { static_cast<OutputScalar>(m_left->x ()) };
		return { x, Geometry::evalLine (Geometry::cast<OutputScalar> (*m_bottom), x) };
	}

	template<class Scalar>
	template<class OutputScalar>
	Point<OutputScalar> Trapezoid<Scalar>::bottomRight () const
	{
		const OutputScalar &x { static_cast<OutputScalar>(m_right->x ()) };
		return { x, Geometry::evalLine (Geometry::cast<OutputScalar> (*m_bottom), x) };
	}

	template<class Scalar>
	template<class OutputScalar>
	Point<OutputScalar> Trapezoid<Scalar>::topLeft () const
	{
		const OutputScalar &x { static_cast<OutputScalar>(m_left->x ()) };
		return { x, Geometry::evalLine (Geometry::cast<OutputScalar> (*m_top), x) };
	}

	template<class Scalar>
	template<class OutputScalar>
	Point<OutputScalar> Trapezoid<Scalar>::topRight () const
	{
		const OutputScalar &x { static_cast<OutputScalar>(m_right->x ()) };
		return { x, Geometry::evalLine (Geometry::cast<OutputScalar> (*m_top), x) };
	}

	template<class Scalar>
	template<class OutputScalar>
	Point<OutputScalar> Trapezoid<Scalar>::centroid () const
	{
		return (bottomLeft<OutputScalar> () + bottomRight<OutputScalar> () + topLeft<OutputScalar> () + topRight<OutputScalar> ()) / OutputScalar { 4 };
	}

	template<class Scalar>
	Scalar Trapezoid<Scalar>::width () const
	{
		return rightX () - leftX ();
	}

	template<class Scalar>
	template<class InputScalar>
	bool Trapezoid<Scalar>::contains (const Point<InputScalar> &_point) const
	{
		return _point.x () > static_cast<InputScalar>(leftX ())
			&& _point.x () < static_cast<InputScalar>(rightX ())
			&& Geometry::getPointSideWithSegment (Geometry::cast<InputScalar> (*m_top), _point) == Geometry::ESide::Right
			&& Geometry::getPointSideWithSegment (Geometry::cast<InputScalar> (*m_bottom), _point) == Geometry::ESide::Left;
	}

	template<class Scalar>
	bool Trapezoid<Scalar>::isJointLeft () const
	{
		return m_bottom->p1 () == m_top->p1 () && m_bottom->p1 () == *m_left;
	}

	template<class Scalar>
	bool Trapezoid<Scalar>::isJointRight () const
	{
		return m_bottom->p2 () == m_top->p2 () && m_bottom->p2 () == *m_right;
	}

	template<class Scalar>
	ETrapezoidPointSource Trapezoid<Scalar>::leftSource () const
	{
		if (isJointLeft ())
		{
			return ETrapezoidPointSource::Joint;
		}
		else if (*m_left == m_bottom->p1 ())
		{
			return ETrapezoidPointSource::Bottom;
		}
		else if (*m_left == m_top->p1 ())
		{
			return ETrapezoidPointSource::Top;
		}
		else
		{
			return ETrapezoidPointSource::External;
		}
	}

	template<class Scalar>
	ETrapezoidPointSource Trapezoid<Scalar>::rightSource () const
	{
		if (isJointRight ())
		{
			return ETrapezoidPointSource::Joint;
		}
		else if (*m_right == m_bottom->p2 ())
		{
			return ETrapezoidPointSource::Bottom;
		}
		else if (*m_right == m_top->p2 ())
		{
			return ETrapezoidPointSource::Top;
		}
		else
		{
			return ETrapezoidPointSource::External;
		}
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
	void Trapezoid<Scalar>::replaceInNeighbors (Trapezoid *_replacement)
	{
		replaceInLeftNeighbors (_replacement);
		replaceInRightNeighbors (_replacement);
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

#endif