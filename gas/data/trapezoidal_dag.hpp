/// GAS::TrapezoidalMap search structure based on GAS::BDAG.
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

namespace GAS
{

	/// TrapezoidalMap search structure based on BDAG.
	namespace TDAG
	{

		/// %Split Node type.
		enum class ESplitType
		{
			Vertical, 		///< Vertical split (x-node).
			NonVertical		///< Non-vertical split (y-node).
		};

		/// %Split Node information.
		/// \tparam Scalar
		/// The scalar type for the split information.
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
		/// The scalar type for the Split information and the Trapezoid.
		template<class Scalar>
		using NodeData = GAS::Utils::BiVariant<Split<Scalar>, Trapezoid<Scalar>>;

		/// Trapezoidal DAG's node.
		/// Type alias for BDAG::Node (can be safely used with BDAG functions). 
		/// \tparam Scalar
		/// The scalar type for the Split information and the Trapezoid.
		template<class Scalar>
		using Node = BDAG::Node<NodeData<Scalar>>;

		/// Trapezoidal DAG.
		/// Type alias for BDAG::Graph (can be safely used with BDAG functions). 
		/// \tparam Scalar
		/// The scalar type for the Split information and the Trapezoid.
		template<class Scalar>
		using Graph = BDAG::Graph<NodeData<Scalar>>;

		/// Left or right children of a node.
		/// Type alias for BDAG::EChild (can be safely used with BDAG functions). 
		using EChild = BDAG::EChild;

		/// Find the trapezoid on which \p point lies.
		/// \tparam Scalar
		/// The scalar type for the TDAG.
		/// \tparam Disambigutor
		/// Any type that can be called with a Split argument and returns an #EChild.
		/// \param[in] root
		/// The root node of the search structure.
		/// \param[in] point
		/// The query point.
		/// \param[in] disambiguator
		/// A callable object that decides whether to continue walking through the left or the right child when the query point lies exactly on a split line.
		/// \return
		/// The trapezoid on which \p point lies.
		template<class Scalar, class Disambiguator>
		inline const Trapezoid<Scalar> &query (const Node<Scalar> &root, const Point<Scalar> &point, Disambiguator disambiguator);

		/// \copydoc query
		template<class Scalar, class Disambiguator>
		inline Trapezoid<Scalar> &query (Node<Scalar> &root, const Point<Scalar> &point, Disambiguator disambiguator);

		/// \copybrief query
		/// \tparam Scalar
		/// The scalar type for the TDAG.
		/// \param[in] root
		/// The root node of the search structure.
		/// \param[in] point
		/// The query point.
		/// \remark
		/// If \p point lies on a split line, the search will continue on its right side.
		/// \return
		/// The trapezoid on which \p point lies.
		template<class Scalar>
		inline const Trapezoid<Scalar> &query (const Node<Scalar> &root, const Point<Scalar> &point);

		/// \copydoc query(const Node<Scalar> &root, const Point<Scalar> &point)
		template<class Scalar>
		inline Trapezoid<Scalar> &query (Node<Scalar> &root, const Point<Scalar> &point);

		/// Utility functions for the TDAG.
		namespace Utils
		{
			
			/// Convenience \c Disambiguator for query() functions that continues always on the right child.
			/// \tparam Scalar
			/// The scalar type for TDAG.
			/// \param[in] split
			/// The split information.
			/// \param[in] point
			/// The query point that lies on the split line of \p split.
			/// \return
			/// Always EChild::Right.
			template<class Scalar>
			EChild disambiguateAlwaysRight (const Split<Scalar> &split, const Point<Scalar> &point);

			/// Get the side of a point with respect to a split line.
			/// \tparam Scalar
			/// The scalar type for the Split information and the Point.
			/// \param[in] split
			/// The split information.
			/// \param[in] point
			/// The point.
			/// \return
			/// The side of \p point with respect to the split line of \p split.
			template<class Scalar>
			Geometry::ESide getPointSide (const Split<Scalar> &split, const Point<Scalar> &point);

			/// Decide whether a point query should continue through the left or the right child of a split node.
			/// \tparam Scalar
			/// The scalar type for the TDAG.
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
			template<class Scalar, class Disambiguator>
			inline EChild getPointQueryNextChild (const Split<Scalar> &split, const Point<Scalar> &point, Disambiguator disambiguator);

			/// Convenience function for getting the Trapezoid from a Graph::ConstLeafNodeIterator.
			/// \tparam Scalar
			/// The scalar type for the TDAG.
			/// \param[in] iterator
			/// The iterator.
			/// \return
			/// The current trapezoid referenced by \p iterator.
			template<class Scalar>
			const Trapezoid<Scalar> &getTrapezoid (const typename Graph<Scalar>::ConstLeafNodeIterator &iterator);

			/// Convenience GAS::Utils::IteratorAdapter for iterating through all the Trapezoid in a TDAG::Graph.
			/// \tparam Scalar
			/// The scalar type for the TDAG.
			template<class Scalar>
			using ConstTrapezoidIterator = GAS::Utils::IteratorAdapter <typename Graph<Scalar>::ConstLeafNodeIterator, const Trapezoid<Scalar>, getTrapezoid<Scalar>>;

		}

	}

}

#include "trapezoidal_dag.tpp"

#endif