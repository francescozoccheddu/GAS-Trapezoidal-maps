/// Geometry utils.
/// \file
/// \author Francesco Zoccheddu

#ifndef GAS_UTILS_GEOMETRY_INCLUDED
#define GAS_UTILS_GEOMETRY_INCLUDED

#include <gas/data/point.hpp>
#include <gas/data/segment.hpp>

namespace GAS
{

	/// Geometry utility functions.
	namespace Geometry
	{

		/// Point side with respect to a line.
		enum class ESide
		{
			Left, Right, Collinear
		};

		/// Get the point side with respect to a vertical line.
		/// \tparam Scalar
		/// The scalar type.
		/// \param[in] lineX
		/// The x-coordinate of the line.
		/// \param[in] point
		/// The point to test.
		/// \return
		/// The side of the point with respect to the specified line.
		template<class Scalar>
		ESide getPointSideWithVerticalLine (const Scalar &lineX, const Point<Scalar> &point);

		/// Get the point side with respect to a segment.
		/// \tparam Scalar
		/// The scalar type.
		/// \param[in] segment
		/// The segment.
		/// \param[in] point
		/// The point to test.
		/// \return
		/// The side of the point with respect to the specified segment.
		/// \note
		/// I could have used cg3::internal::positionOfPointWithRespectToSegment but it looks like a private API.
		template<class Scalar>
		ESide getPointSideWithSegment (const Segment<Scalar> &segment, const Point<Scalar> &point);

		/// Get the y-coordinate of the point lying on a line at a given x-coordinate.
		/// \tparam Scalar
		/// The scalar type.
		/// \param[in] line
		/// Any segment that lies on the line to evaluate.
		/// \param[in] x
		/// The x-coordinate of the point.
		/// \return
		/// The y-coordinate of the line evaluated at \p x.
		template<class Scalar>
		Scalar evalLine (const Segment<Scalar> &line, Scalar x);

		/// \tparam Scalar
		/// The scalar type.
		/// \param[in] segment
		/// The segment.
		/// \return
		/// \c true if \c segment.p1() is the leftmost endpoint of \p segment, \c false otherwise.
		template<class Scalar>
		bool areSegmentPointsHorizzontallySorted (const Segment<Scalar> &segment);

		/// \tparam Scalar
		/// The scalar type.
		/// \param[in] segment
		/// The segment.
		/// \pre
		/// Segment must not be degenerate.
		/// \return
		/// \c true if \p segment is vertical, \c false otherwise.
		template<class Scalar>
		bool isSegmentVertical (const Segment<Scalar> &segment);

		/// \tparam Scalar
		/// The scalar type.
		/// \param[in] segment
		/// The segment.
		/// \return
		/// A segment where the same endpoints of \p segment are sorted on their x-coordinates.
		template<class Scalar>
		Segment<Scalar> sortSegmentPointsHorizontally (const Segment<Scalar> &segment);

		/// \tparam Scalar
		/// The scalar type.
		/// \param[in] point
		/// The point to test.
		/// \param[in] bottomLeft
		/// The bottom left point of the rectangle.
		/// \param[in] topRight
		/// The top right point of the rectangle.
		/// \return
		/// \c true if \p point is inside the rectangle, \c false otherwise.
		template<class Scalar>
		bool isPointInsideBox (const Point<Scalar> &point, const Point<Scalar> &bottomLeft, const Point<Scalar> &topRight);

		/// \tparam Scalar
		/// The scalar type.
		/// \param[in] segment
		/// The segment to test.
		/// \param[in] bottomLeft
		/// The bottom left point of the rectangle.
		/// \param[in] topRight
		/// The top right point of the rectangle.
		/// \return
		/// \c true if \p segment is completely inside the rectangle, \c false otherwise.
		template<class Scalar>
		bool isSegmentInsideBox (const Segment<Scalar> &segment, const Point<Scalar> &bottomLeft, const Point<Scalar> &topRight);

	}

}

#include "geometry.tpp"

#endif