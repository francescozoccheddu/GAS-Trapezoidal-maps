/// GAS::Trapezoid data structure for GAS::TrapezoidalMap.
/// \file
/// \author Francesco Zoccheddu

#ifndef GAS_DATA_TRAPEZOID_INCLUDED
#define GAS_DATA_TRAPEZOID_INCLUDED

#include <gas/data/point.hpp>
#include <gas/data/segment.hpp>

/// Enable an unique GAS::Trapezoid identifier for debugging purposes.
/// If defined, an unique GAS::Utils::Serial member is added to each GAS::Trapezoid object.
#define GAS_DRAWING_ENABLE_TRAPEZOID_SERIAL

#ifdef GAS_DRAWING_ENABLE_TRAPEZOID_SERIAL
#include <gas/utils/serial.hpp>
#endif

/// Root namespace for the project.
namespace GAS
{

	/// Source of the left or right point of a Trapezoid.
	enum class ETrapezoidPointSource
	{
		Bottom, 	///< The point is an endpoint of the bottom segment.
		Top, 		///< The point is an endpoint of the top  segment.
		Joint, 		///< The point is a common endpoint of both the top and the bottom  segment.
		External	///< The point is not an endpoint of either segment.
	};

	/// %Trapezoid data structure to be used in a TrapezoidalMap.
	/// Stores pointers to the left and right points, bottom and top segments and the eventual four neighbors.
	/// \tparam Scalar
	/// The scalar type.
	template<class Scalar>
	class Trapezoid final
	{

		using PointS = Point<Scalar>;
		using SegmentS = Segment<Scalar>;

	private:

#ifdef GAS_DRAWING_ENABLE_TRAPEZOID_SERIAL
		Utils::Serial m_serial {};
	public:
		/// \return 
		/// An unique serial for debugging purposes.
		const Utils::Serial &serial () const;
	private:
#endif

		const PointS *m_left {}, *m_right {};
		const SegmentS *m_bottom {}, *m_top {};
		Trapezoid *m_lowerLeftNeighbor {}, *m_upperLeftNeighbor {}, *m_lowerRightNeighbor {}, *m_upperRightNeighbor {};

	public:

		/// Set \p left to be the only left neighbor of \p right and vice versa.
		/// \param[in] left
		/// The left trapezoid.
		/// \param[in] right
		/// The right trapezoid.
		static void link (Trapezoid &left, Trapezoid &right);

		/// \return
		/// The left point.
		const PointS *left () const;

		/// \return
		/// The right point.
		const PointS *right () const;

		/// \return
		/// The bottom segment.
		const SegmentS *bottom () const;

		/// \return
		/// The top segment.
		const SegmentS *top () const;

		/// \return
		/// The x-coordinate of the left point.
		const Scalar leftX () const;

		/// \return
		/// The x-coordinate of the right point.
		const Scalar rightX () const;

		/// \remarks
		/// If the trapezoid has a single left neighbor it will be equivalent to upperLeftNeighbor().
		/// \return
		/// The lower left neighbor.
		const Trapezoid *lowerLeftNeighbor () const;

		/// \remarks
		/// If the trapezoid has a single left neighbor it will be equivalent to lowerLeftNeighbor().
		/// \return
		/// The upper left neighbor.
		const Trapezoid *upperLeftNeighbor () const;

		/// \remarks
		/// If the trapezoid has a single right neighbor it will be equivalent to upperRightNeighbor().
		/// \return
		/// The lower right neighbor.
		const Trapezoid *lowerRightNeighbor () const;

		/// \remarks
		/// If the trapezoid has a single right neighbor it will be equivalent to lowerRightNeighbor().
		/// \return
		/// The upper right neighbor.
		const Trapezoid *upperRightNeighbor () const;

		/// \copydoc left
		const PointS *&left ();

		/// \copydoc right
		const PointS *&right ();

		/// \copydoc bottom
		const SegmentS *&bottom ();

		/// \copydoc top
		const SegmentS *&top ();

		/// \copydoc lowerLeftNeighbor
		Trapezoid *&lowerLeftNeighbor ();

		/// \copydoc upperLeftNeighbor
		Trapezoid *&upperLeftNeighbor ();

		/// \copydoc lowerRightNeighbor
		Trapezoid *&lowerRightNeighbor ();

		/// \copydoc upperRightNeighbor
		Trapezoid *&upperRightNeighbor ();

		/// \tparam OutputScalar
		/// The scalar type to use to perform the arithmetic operations.
		/// \return
		/// The bottom left point.
		template<class OutputScalar = Scalar>
		Point<OutputScalar> bottomLeft () const;

		/// \tparam OutputScalar
		/// The scalar type to use to perform the arithmetic operations.
		/// \return
		/// The bottom right point.
		template<class OutputScalar = Scalar>
		Point<OutputScalar> bottomRight () const;

		/// \tparam OutputScalar
		/// The scalar type to use to perform the arithmetic operations.
		/// \return
		/// The top left point.
		template<class OutputScalar = Scalar>
		Point<OutputScalar> topLeft () const;

		/// \tparam OutputScalar
		/// The scalar type to use to perform the arithmetic operations.
		/// \return
		/// The top right point.
		template<class OutputScalar = Scalar>
		Point<OutputScalar> topRight () const;

		/// \tparam OutputScalar
		/// The scalar type to use to perform the arithmetic operations.
		/// \return
		/// The centroid.
		template<class OutputScalar = Scalar>
		Point<OutputScalar> centroid () const;

		/// \return
		/// The width.
		Scalar width () const;

		/// \tparam InputScalar
		/// The scalar type to use to perform the arithmetic operations.
		/// \return
		/// \c true if the trapezoid contains \p point, \c false otherwise.
		template<class InputScalar = Scalar>
		bool contains (const Point<InputScalar> &point) const;

		/// \return
		/// \c true if bottomLeft() equals topLeft(), \c false otherwise.
		bool isJointLeft () const;

		/// \return
		/// \c true if bottomRight() equals topRight(), \c false otherwise.
		bool isJointRight () const;

		/// \see ETrapezoidPointSource
		/// \return
		/// The source of the left point.
		ETrapezoidPointSource leftSource () const;

		/// \see ETrapezoidPointSource
		/// \return
		/// The source of the right point.
		ETrapezoidPointSource rightSource () const;

		/// Convenience method to set both the left neighbors.
		/// \param[in] trapezoid
		/// The left neighbor.
		void setLeftNeighbors (Trapezoid *trapezoid);

		/// Convenience method to set both the right neighbors.
		/// \param[in] trapezoid
		/// The right neighbor.
		void setRightNeighbors (Trapezoid *trapezoid);

		/// Convenience method to set both the left neighbors.
		/// \param[in] lower
		/// The lower left neighbor.
		/// \param[in] upper
		/// The upper left neighbor.
		void setLeftNeighbors (Trapezoid *lower, Trapezoid *upper);

		/// Convenience method to set both the right neighbors.
		/// \param[in] lower
		/// The lower right neighbor.
		/// \param[in] upper
		/// The upper right neighbor.
		void setRightNeighbors (Trapezoid *lower, Trapezoid *upper);

		/// Replace the references to this trapezoid in the left neighbors with \p replacement.
		/// \param[in] replacement
		/// The replacement.
		void replaceInLeftNeighbors (Trapezoid *replacement);

		/// Replace the references to this trapezoid in the right neighbors with \p replacement.
		/// \param[in] replacement
		/// The replacement.
		void replaceInRightNeighbors (Trapezoid *replacement);

		/// Replace the references to this trapezoid in the neighbors with \p replacement.
		/// \param[in] replacement
		/// The replacement.
		void replaceInNeighbors (Trapezoid *replacement);

		/// Replace the references to some left neighbor \p replaced with \p replacement.
		/// \param[in] replaced
		/// The reference to replace.
		/// \param[in] replacement
		/// The replacement.
		void replaceLeftNeighbor (const Trapezoid *replaced, Trapezoid *replacement);

		/// Replace the references to some right neighbor \p replaced with \p replacement.
		/// \param[in] replaced
		/// The reference to replace.
		/// \param[in] replacement
		/// The replacement.
		void replaceRightNeighbor (const Trapezoid *replaced, Trapezoid *replacement);

	};

}

#include "trapezoid.tpp"

#endif