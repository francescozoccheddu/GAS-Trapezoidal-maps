#pragma once

#include "trapezoid.hpp"
#include <utils/geometry_utils.hpp>

namespace GAS
{

	template<class Scalar>
	Point<Scalar> Trapezoid<Scalar>::getBottomLeft () const
	{
		return { *left, Geometry::evalLine (*bottom, *left) };
	}

	template<class Scalar>
	Point<Scalar> Trapezoid<Scalar>::getBottomRight () const
	{
		return { *left, Geometry::evalLine (*bottom, *right) };
	}

	template<class Scalar>
	Point<Scalar> Trapezoid<Scalar>::getTopLeft () const
	{
		return { *left, Geometry::evalLine (*top, *left) };
	}

	template<class Scalar>
	Point<Scalar> Trapezoid<Scalar>::getTopRight () const
	{
		return { *left, Geometry::evalLine (*top, *right) };
	}

	template<class Scalar>
	Point<Scalar> Trapezoid<Scalar>::getCentroid () const
	{
		return (getBottomLeft () + getBottomRight () + getTopLeft () + getTopRight ()) / 4;
	}

}