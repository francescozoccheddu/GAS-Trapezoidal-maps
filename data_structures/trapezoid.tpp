#pragma once

#include "trapezoid.hpp"
#include <utils/geometry_utils.hpp>

namespace GAS
{

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