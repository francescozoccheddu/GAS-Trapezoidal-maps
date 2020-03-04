#pragma once

#include "trapezoid.hpp"
#include <utils/geometry_utils.hpp>

namespace GAS
{

	template<class Scalar>
	void Trapezoid<Scalar>::replaceLeft (const Trapezoid &_target, Trapezoid *_substitute)
	{
		if (lowerLeftNeighbor == &_target)
		{
			lowerLeftNeighbor = _substitute;
		}
		if (upperLeftNeighbor == &_target)
		{
			upperLeftNeighbor = _substitute;
		}
	}

	template<class Scalar>
	void Trapezoid<Scalar>::replaceRight (const Trapezoid &_target, Trapezoid *_substitute)
	{
		if (lowerRightNeighbor == &_target)
		{
			lowerRightNeighbor = _substitute;
		}
		if (upperRightNeighbor == &_target)
		{
			upperRightNeighbor = _substitute;
		}
	}

	template<class Scalar>
	void Trapezoid<Scalar>::link (Trapezoid &_bottomLeft, Trapezoid &_topLeft, Trapezoid &_bottomRight, Trapezoid &_topRight)
	{
		if (_bottomLeft.getTopRight ().y () < _bottomRight.getTopLeft ().y ())
		{
			_bottomRight.lowerLeftNeighbor = &_bottomLeft;
			_bottomRight.upperLeftNeighbor = &_topLeft;
			_bottomLeft.lowerRightNeighbor = _bottomLeft.upperRightNeighbor = &_bottomRight;
		}
		else
		{
			_bottomRight.lowerLeftNeighbor = _bottomRight.upperLeftNeighbor = &_bottomLeft;
			_bottomLeft.lowerRightNeighbor = &_bottomRight;
			_bottomLeft.upperRightNeighbor = &_topRight;
		}
		if (_topLeft.getBottomRight ().y () > _topRight.getBottomLeft ().y ())
		{
			_topRight.lowerLeftNeighbor = &_bottomLeft;
			_topRight.upperLeftNeighbor = &_topLeft;
			_topLeft.lowerRightNeighbor = _bottomLeft.upperRightNeighbor = &_topRight;
		}
		else
		{
			_topRight.lowerLeftNeighbor = _topRight.upperLeftNeighbor = &_topLeft;
			_topLeft.lowerRightNeighbor = &_bottomRight;
			_topLeft.upperRightNeighbor = &_topRight;
		}
	}

	template<class Scalar>
	void Trapezoid<Scalar>::replaceAndLinkLeft (Trapezoid &_old, Trapezoid &_bottomRight, Trapezoid &_topRight)
	{
		if (_old.lowerLeftNeighbor)
		{
			link (*_old.lowerLeftNeighbor, *_old.upperLeftNeighbor, _bottomRight, _topRight);
		}
		else
		{
			_bottomRight.lowerLeftNeighbor = _bottomRight.upperLeftNeighbor = _topRight.lowerLeftNeighbor = _topRight.upperLeftNeighbor = nullptr;
		}
	}

	template<class Scalar>
	void Trapezoid<Scalar>::replaceAndLinkRight (Trapezoid &_old, Trapezoid &_bottomRight, Trapezoid &_topRight)
	{
		if (_old.lowerRightNeighbor)
		{
			link (*_old.lowerRightNeighbor, *_old.upperRightNeighbor, _bottomRight, _topRight);
		}
		else
		{
			_bottomRight.lowerRightNeighbor = _bottomRight.upperRightNeighbor = _topRight.lowerRightNeighbor = _topRight.upperRightNeighbor = nullptr;
		}
	}

	template<class Scalar>
	Trapezoid<Scalar>::~Trapezoid ()
	{
		if (lowerLeftNeighbor)
		{
			lowerLeftNeighbor->replaceRight (*this, nullptr);
		}
		if (lowerRightNeighbor)
		{
			lowerRightNeighbor->replaceLeft (*this, nullptr);
		}
		if (upperLeftNeighbor)
		{
			upperLeftNeighbor->replaceRight (*this, nullptr);
		}
		if (upperRightNeighbor)
		{
			upperRightNeighbor->replaceLeft (*this, nullptr);
		}
	}

	template<class Scalar>
	Point<Scalar> Trapezoid<Scalar>::getBottomLeft () const
	{
		return { left->x (), Geometry::evalLine (*bottom, left->x ()) };
	}

	template<class Scalar>
	Point<Scalar> Trapezoid<Scalar>::getBottomRight () const
	{
		return { right->x (), Geometry::evalLine (*bottom, right->x ()) };
	}

	template<class Scalar>
	Point<Scalar> Trapezoid<Scalar>::getTopLeft () const
	{
		return { left->x (), Geometry::evalLine (*top, left->x ()) };
	}

	template<class Scalar>
	Point<Scalar> Trapezoid<Scalar>::getTopRight () const
	{
		return { right->x (), Geometry::evalLine (*top, right->x ()) };
	}

	template<class Scalar>
	Point<Scalar> Trapezoid<Scalar>::getCentroid () const
	{
		return (getBottomLeft () + getBottomRight () + getTopLeft () + getTopRight ()) / 4;
	}

}