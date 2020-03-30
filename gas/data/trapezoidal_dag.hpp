/// GAS::TDAG search structure for GAS::TrapezoidalMap.
/// \file
/// \author Francesco Zoccheddu

#ifndef GAS_DATA_TRAPEZOIDAL_DAG_INCLUDED
#define GAS_DATA_TRAPEZOIDAL_DAG_INCLUDED

#include <gas/data/point.hpp>
#include <gas/data/segment.hpp>
#include <gas/data/trapezoid.hpp>
#include <gas/data/binary_dag.hpp>
#include <gas/utils/geometry.hpp>
#include <gas/utils/bivariant.hpp>
#include <gas/utils/iterators.hpp>
#include <gas/utils/ignore.hpp>

namespace GAS
{

	/// Search structure for TrapezoidalMap based on BDAG.
	namespace TDAG
	{

		/// %Split node type.
		enum class ESplitType
		{
			Vertical, 		///< Vertical split (x-node).
			NonVertical		///< Non-vertical split (y-node).
		};

		/// %Split node information.
		/// \tparam Scalar
		/// The scalar type.
		template<class Scalar>
		class Split
		{

			ESplitType m_type;

			union
			{
				const Scalar *m_x;					///< Vertical split data.
				const Segment<Scalar> *m_segment;	///< Non-vertical split data.
			};

		public:

			/// Construct a vertical split.
			/// \param[in] x
			/// The vertical split line x-coordinate.
			/// \remark
			/// A reference to \p x will be stored, so its address must remain valid.
			Split (const Scalar &x);

			/// Construct a non-vertical split.
			/// \param[in] segment
			/// The split segment.
			/// \remark
			/// A reference to \p segment will be stored, so its address must remain valid.
			Split (const Segment<Scalar> &segment);

			/// \see ESplitType
			/// \return
			/// The split type.
			ESplitType type () const;

			/// \pre
			/// The split type must be vertical.
			/// \return
			/// The vertical split line x-coordinate.
			const Scalar &x () const;

			/// \pre
			/// The split type must be non-vertical.
			/// \return
			/// The split segment.
			const Segment<Scalar> &segment () const;

			/// Turn into a vertical split.
			/// \param[in] x
			/// The vertical split line x-coordinate.
			/// \remark
			/// A reference to \p x will be stored, so its address must remain valid.
			void setVertical (const Scalar &x);

			/// Turn into a non-vertical split.
			/// \param[in] segment
			/// The split segment.
			/// \remark
			/// A reference to \p segment will be stored, so its address must remain valid.
			void setNonVertical (const Segment<Scalar> &segment);

		};

		/// User data type to be used with BDAG::Node.
		/// \tparam Scalar
		/// The scalar type.
		template<class Scalar>
		using NodeData = GAS::Utils::BiVariant<Split<Scalar>, Trapezoid<Scalar>>;

		/// Trapezoidal DAG's node.
		/// Type alias for BDAG::Node (can be safely used with BDAG functions). 
		/// \tparam Scalar
		/// The scalar type.
		template<class Scalar>
		using Node = BDAG::Node<NodeData<Scalar>>;

		/// Trapezoidal DAG.
		/// Type alias for BDAG::Graph (can be safely used with BDAG functions). 
		/// \tparam Scalar
		/// The scalar type.
		template<class Scalar>
		using Graph = BDAG::Graph<NodeData<Scalar>>;

		/// Left or right child of a node.
		/// Type alias for BDAG::EChild (can be safely used with BDAG functions). 
		using EChild = BDAG::EChild;

		/// Find the trapezoid on which \p point lies.
		/// \tparam Scalar
		/// The scalar type.
		/// \tparam QueryScalar
		/// The scalar type to use to perform the arithmetic operations.
		/// \tparam Disambigutor
		/// Any type that can be called with a Split and a Point argument and returns an #EChild.
		/// \param[in] root
		/// The root node of the search structure.
		/// \param[in] point
		/// The query point.
		/// \param[in] disambiguator
		/// A callable object that decides whether to continue walking through the left or the right child when the query point lies exactly on a split line.
		/// \return
		/// The trapezoid on which \p point lies.
		template<class Scalar, class QueryScalar, class Disambiguator>
		inline const Trapezoid<Scalar> &query (const Node<Scalar> &root, const Point<QueryScalar> &point, Disambiguator disambiguator);

		/// \copydoc query
		template<class Scalar, class QueryScalar, class Disambiguator>
		inline Trapezoid<Scalar> &query (Node<Scalar> &root, const Point<QueryScalar> &point, Disambiguator disambiguator);

		/// \copybrief query
		/// \tparam Scalar
		/// The scalar type.
		/// \tparam QueryScalar
		/// The scalar type to use to perform the arithmetic operations.
		/// \param[in] root
		/// The root node of the search structure.
		/// \param[in] point
		/// The query point.
		/// \remark
		/// If \p point lies on a split line, the search will continue on its right side.
		/// \return
		/// The trapezoid on which \p point lies.
		template<class Scalar, class QueryScalar = Scalar>
		inline const Trapezoid<Scalar> &query (const Node<Scalar> &root, const Point<QueryScalar> &point);

		/// \copydoc query(const Node<Scalar> &, const Point<Scalar> &)
		template<class Scalar, class QueryScalar = Scalar>
		inline Trapezoid<Scalar> &query (Node<Scalar> &root, const Point<QueryScalar> &point);

		/// Utility functions for the TDAG.
		namespace Utils
		{

			/// Convenience \c Disambiguator for query() functions that continues always on the right child.
			/// \param[in] split
			/// Ignored.
			/// \param[in] point
			/// Ignored.
			/// \return
			/// Always BDAG::EChild::Right.
			inline EChild disambiguateAlwaysRight (GAS::Utils::Ignore split, GAS::Utils::Ignore point);

			/// Get the side of a point with respect to a split line.
			/// \tparam Scalar
			/// The scalar type.
			/// \tparam QueryScalar
			/// The scalar type to use to perform the arithmetic operations.
			/// \param[in] split
			/// The split information.
			/// \param[in] point
			/// The point.
			/// \return
			/// The side of \p point with respect to the split line of \p split.
			template<class Scalar, class QueryScalar = Scalar>
			Geometry::ESide getPointSide (const Split<Scalar> &split, const Point<QueryScalar> &point);

			/// Decide whether a point query should continue through the left or the right child of a split node.
			/// \tparam Scalar
			/// The scalar type.
			/// \tparam QueryScalar
			/// The scalar type to use to perform the arithmetic operations.
			/// \tparam Disambigutor
			/// Any type that can be called with a Split argument and returns an #EChild.
			/// \param[in] split
			/// The split node information.
			/// \param[in] point
			/// The query point.
			/// \param[in] disambiguator
			/// A callable object that decides whether to continue walking through the left or the right child when the query point lies exactly on a split line.
			/// \return
			/// The child through which the query for \p point should continue.
			template<class Scalar, class QueryScalar, class Disambiguator>
			inline EChild getPointQueryNextChild (const Split<Scalar> &split, const Point<QueryScalar> &point, Disambiguator disambiguator);

			/// Convenience function for retrieving the Trapezoid referenced by a Graph::ConstLeafNodeIterator.
			/// \tparam Scalar
			/// The scalar type.
			/// \param[in] iterator
			/// The iterator.
			/// \return
			/// The current trapezoid referenced by \p iterator.
			template<class Scalar>
			const Trapezoid<Scalar> &getTrapezoid (const typename Graph<Scalar>::ConstLeafNodeIterator &iterator);

			/// Convenience Utils::IteratorAdapter for iterating through all the Trapezoid in a TDAG::Graph.
			/// \tparam Scalar
			/// The scalar type.
			template<class Scalar>
			using ConstTrapezoidIterator = GAS::Utils::IteratorAdapter <typename Graph<Scalar>::ConstLeafNodeIterator, const Trapezoid<Scalar>, getTrapezoid<Scalar>>;

		}

	}

}

#include "trapezoidal_dag.tpp"

#endif