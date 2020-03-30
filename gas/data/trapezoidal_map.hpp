/// GAS::TrapezoidalMap data structure for efficient point location querying.
/// \file
/// \author Francesco Zoccheddu

#ifndef GAS_DATA_TRAPEZOIDAL_MAP_INCLUDED
#define GAS_DATA_TRAPEZOIDAL_MAP_INCLUDED

#include <gas/data/point.hpp>
#include <gas/data/segment.hpp>
#include <gas/data/trapezoid.hpp>
#include <gas/data/trapezoidal_dag.hpp>
#include <forward_list>

namespace GAS
{

	/// Trapezoidal map data structure for efficient point location querying.
	/// \tparam Scalar
	/// The scalar type.
	template<class Scalar>
	class TrapezoidalMap final
	{

		using PointS = Point<Scalar>;
		using SegmentS = Segment<Scalar>;
		using Trapezoid = Trapezoid<Scalar>;
		using Node = TDAG::Node<Scalar>;
		using NodeData = TDAG::NodeData<Scalar>;
		using Graph = TDAG::Graph<Scalar>;

		/// %Pair of horizontally or vertically stacked Trapezoid.
		class Pair
		{

		protected:

			Trapezoid *m_a, *m_b;

			Pair (Trapezoid *leftOrBottom, Trapezoid *rightOrTop);

		public:

			Pair (Trapezoid &leftOrBottom, Trapezoid &rightOrTop);

			/// \return
			/// \c true if the two trapezoids are different, \c false otherwise.
			bool isSplit () const;

			/// \return
			/// \c true if the trapezoids share the same left point, \c false otherwise.
			bool isLeftAligned () const;

			/// \return
			/// \c true if the trapezoids share the same right point, \c false otherwise.
			bool isRightAligned () const;

			Trapezoid &left () const;
			Trapezoid &right () const;
			Trapezoid &bottom () const;
			Trapezoid &top () const;

		};

		/// %Pair of horizontally or vertically stacked Trapezoid or NullablePair::null.
		class NullablePair : public Pair
		{

		public:

			/// The null pair.
			static const NullablePair null;

			/// \param[in] leftOrBottom
			/// The left or the bottom trapezoid.
			/// \param[in] rightOrTop
			/// The right or the top trapezoid.
			/// \pre
			/// The arguments must both be non-null or null.
			/// \return
			/// A new pair if the arguments are non-null, #null otherwise.
			static NullablePair allOrNone (Trapezoid *leftOrBottom, Trapezoid *rightOrTop);

			using Pair::Pair;

			NullablePair &operator=(const NullablePair &) = default;
			NullablePair &operator=(const Pair &);

			/// \return
			/// \c true if the pair is not #null, \c false otherwise.
			operator bool () const;

		};

		/// Update trapezoid neighbors after a split operations has occurred.
		/// Update \p trapezoid neighbors to refer to \p neighbors if needed.
		/// \param[in] trapezoid
		/// The trapezoid to update.
		/// \param[in] neighbors
		/// The new neighbors to which \p trapezoid may refer.
		/// \param[in] right
		/// \c true if \p neighbors are located to the right of \p trapezoid, \c false otherwise.
		/// \pre
		/// \p neighbors must be a vertically stacked pair or a single trapezoid.
		static void weld (Trapezoid &trapezoid, Pair neighbors, bool right);

		/// Update trapezoids neighbors after a split operations has occurred.
		/// Update \p left trapezoids to refer to \p right trapezoids and vice versa if needed.
		/// \param[in] left
		/// The left pair.
		/// \param[in] right
		/// The right pair.
		/// \pre
		/// \p left and \p right must be vertically stacked pairs or single trapezoids.
		static void weld (Pair left, Pair right);

		/// \tparam ArithmeticScalar
		/// The scalar type to use to perform the arithmetic operations.
		/// \param[in] line
		/// Any non-degenerate segment the lies on the line to evaluate.
		/// \param[in] trapezoid
		/// The trapezoid.
		/// \return
		/// The y-coordinate of the intersection between \p line and the right edge of \p trapezoid.
		template<class ArithmeticScalar = Scalar>
		static ArithmeticScalar evalLineOnRightEdge (const SegmentS &line, const Trapezoid &trapezoid);

		/// List of inserted segments providing stable references.
		std::forward_list<SegmentS> m_segments;

		/// Trapezoid search structure.
		Graph m_graph;

		/// Bounding box segments.
		/// \note
		/// I could have used \c cg3::BoundingBox2 but I needed this two segments to be referenceable.
		SegmentS m_bottom, m_top;

		/// \remark
		/// The root node changes only if destroy() or initialize() are called.
		/// \pre
		/// The search structure must not be empty.
		/// \return
		/// The root node of the search structure.
		Node &root ();

		/// Clear the search structure and the segments list.
		/// \remark
		/// All the references to nodes and trapezoids will be invalidated.
		void destroy ();

		/// Initialize the search structure with the first Trapezoid.
		/// \pre
		/// The search structure must be empty.
		void initialize ();

		/// \param[in] trapezoid
		/// The trapezoid.
		/// \return
		/// The node in the search structure that holds \p trapezoid.
		Node &getNode (Trapezoid &trapezoid) const;

		/// Create a new trapezoid node in the search structure.
		/// \param[in] copy
		/// The trapezoid to clone.
		/// \return
		/// The created trapezoid.
		Trapezoid &createTrapezoid (const Trapezoid &copy = {});

		/// Turn a trapezoid node into a vertical split node in the search structure.
		/// \param[in] trapezoid
		/// The trapezoid contained in the node to split.
		/// \param[in] x
		/// The x-coordinate of the vertical split line.
		/// \param[in] left
		/// The left child of the split node.
		/// \param[in] right
		/// The right child of the split node.
		/// \pre
		/// \p trapezoid must be a contained in valid trapezoid node created using createTrapezoid().
		/// \remark
		/// \p trapezoid will be invalidated.
		/// \remark
		/// A reference to \p x will be stored, so its address must remain valid.
		void splitTrapezoid (Trapezoid &trapezoid, const Scalar &x, Trapezoid &left, Trapezoid &right);

		/// Turn a trapezoid node into a non-vertical split node in the search structure.
		/// \param[in] trapezoid
		/// The trapezoid contained in the node to split.
		/// \param[in] segment
		/// The split segment.
		/// \param[in] left
		/// The left child of the split node.
		/// \param[in] right
		/// The right child of the split node.
		/// \pre
		/// \p trapezoid must be a contained in valid trapezoid node created using createTrapezoid().
		/// \remark
		/// \p trapezoid will be invalidated.
		/// \remark
		/// A reference to \p segment will be stored, so its address must remain valid.
		void splitTrapezoid (Trapezoid &trapezoid, const SegmentS &segment, Trapezoid &left, Trapezoid &right);

		/// Find the leftmost trapezoid intersecting with a segment.
		/// \tparam ArithmeticScalar
		/// The scalar type to use to perform the arithmetic operations.
		/// \param[in] segment
		/// The test segment.
		/// \pre
		/// The segment endpoints must be sorted on their x-coordinates.
		/// \return
		/// The leftmost trapezoid intersecting with \p segment.
		/// \exception std::invalid_argument
		/// If \p segment is duplicated, overlapping or shares the x-coordinate (but not the y-coordinate) of the left endpoint with another segment.
		template<class ArithmeticScalar = Scalar>
		Trapezoid &findLeftmostIntersectedTrapezoid (const SegmentS &segment);

		/// Split a trapezoid along a vertical line.
		/// \param[in] trapezoid
		/// The trapezoid to split.
		/// \param[in] point
		/// Defines the x-coordinate of the split line. Becomes the right point of the left half trapezoid and the left point of the right one.
		/// \pre
		/// \p trapezoid must be a contained in valid trapezoid node created using createTrapezoid().
		/// \pre
		/// \p point must be contained inside \p trapezoid.
		/// \remark
		/// A reference to \p point will be stored, so its address must remain valid.
		/// \return
		/// A pair of horizontally stacked trapezoids.
		Pair splitVertically (Trapezoid &trapezoid, const PointS &point);

		/// Split a trapezoid along a non-vertical segment.
		/// \param[in] trapezoid
		/// The trapezoid to split.
		/// \param[in] segment
		/// The split segment. Becomes the top segment of the lower half trapezoid and the bottom segment of the upper one.
		/// \param[in] previous
		/// The left neighbors to weld(). Can be the return value of the last call to this method for chained splits.
		/// \pre
		/// \p trapezoid must be a contained in valid trapezoid node created using createTrapezoid().
		/// \pre
		/// \p segment must cross \p trapezoid.
		/// \remark
		/// weld() will be called only for the left neighbors, so the right links should be manually updated or they may not be valid.
		/// \remark
		/// A reference to \p segment will be stored, so its address must remain valid.
		/// \return
		/// A pair of vertically stacked trapezoids.
		Pair incrementalSplitHorizontally (Trapezoid &trapezoid, const SegmentS &segment, NullablePair previous);

		/// Update the trapezoidal map after a segment has been added to the list of segments.
		/// Split the trapezoids intersected by \p segment, starting from \p leftmost.
		/// \tparam ArithmeticScalar
		/// The scalar type to use to perform the arithmetic operations.
		/// \param[in] segment
		/// The new segment in the list.
		/// \param[in] leftmost
		/// The leftmost intersected trapezoid by \p segment.
		/// \pre
		/// The segment endpoints must be sorted on their x-coordinates.
		/// \remark
		/// The segment reference will be stored in the intersected trapezoids, so its address must be stable.
		/// \remark
		/// No checks for the segment validity will be made.
		template<class ArithmeticScalar = Scalar>
		void updateForNewSegment (const SegmentS &segment, Trapezoid &leftmost);

		/// Check if a segment intersects some other segment in the map.
		/// \tparam ArithmeticScalar
		/// The scalar type to use to perform the arithmetic operations.
		/// \param[in] segment
		/// The segment to test.
		/// \param[in] leftmost
		/// The leftmost intersected trapezoid by \p segment.
		/// \pre
		/// The segment endpoints must be sorted on their x-coordinates.
		/// \pre
		/// The segment must be contained inside the map bounds.
		/// \return
		/// \c true if \p segment intersects some other segment in the map, \c false otherwise.
		/// \exception std::invalid_argument
		/// If \p segment shares the x-coordinate (but not the y-coordinate) of the right endpoint with another segment.
		template<class ArithmeticScalar = Scalar>
		bool doesSegmentIntersect (const SegmentS &segment, const Trapezoid &leftmost) const;

	public:

		/// Construct an empty trapezoidal map.
		/// \param[in] bottomLeft
		/// The bottom left point of the bounding box.
		/// \param[in] topRight
		/// The top right point of the bounding box.
		/// \remark
		/// An empty trapezoidal map still has a single trapezoid.
		/// \exception std::invalid_argument
		/// If the two points are inverted or equal.
		TrapezoidalMap (const PointS &bottomLeft, const PointS &topRight);

		/// Construct a trapezoidal map by cloning \p copy.
		/// \param[in] copy
		/// The trapezoidal map to clone.
		TrapezoidalMap (const TrapezoidalMap &copy) = default;

		/// Construct a trapezoidal map by moving \p moved.
		/// \param[in] moved
		/// The trapezoidal map to move.
		/// \remark
		/// After calling this constructor \p moved will be empty and valid.
		TrapezoidalMap (TrapezoidalMap &&moved);

		/// \see clear()
		~TrapezoidalMap () = default;

		/// Clear the map and clone \p copy.
		/// \param[in] copy
		/// The trapezoidal map to clone.
		TrapezoidalMap &operator =(const TrapezoidalMap &copy) = default;

		/// Clear the map and move \p moved.
		/// \param[in] moved
		/// The trapezoidal map to move.
		/// \remark
		/// After calling this assignment operator \p moved will be empty and valid.
		TrapezoidalMap &operator =(TrapezoidalMap &&moved);

		/// Get the root node of the search structure.
		/// \see TDAG
		/// \remark
		/// The root node changes only when the map is cleared (when the map is moved or assigned or clear() is called).
		/// \return
		/// The root node of the search structure.
		const Node &root () const;

		/// Get the number of trapezoids in the map.
		/// \return
		/// The number of trapezoids.
		int trapezoidsCount () const;

		/// Get the \c begin iterator for iterating through all the trapezoids.
		/// The iteration follows the order of creation of the trapezoids (note that splitting a trapezoid creates two new trapezoids).
		/// \return
		/// The \c begin iterator.
		TDAG::Utils::ConstTrapezoidIterator<Scalar> begin () const;

		/// Get the \c end iterator for iterating through all the trapezoids.
		/// \see begin()
		/// \remark
		/// The \c end iterator does refer to any trapezoid thus should not be dereferenced.
		/// \remark
		/// The \c end iterator does not depend on the state of the map and can also be obtained through the default constructor TDAG::Utils::ConstTrapezoidIterator().
		/// \return 
		/// The \c end iterator.
		TDAG::Utils::ConstTrapezoidIterator<Scalar> end () const;

		/// Find the trapezoid in the map that contains the point \p point.
		/// \return
		/// The trapezoid that contains \p point.
		/// \exception std::invalid_argument
		/// If \p point is outside the bounding box.
		const Trapezoid &query (const PointS &point) const;

		/// \return
		/// The bottom left point of the bounding box.
		const PointS &bottomLeft () const;

		/// \return
		/// The bottom right point of the bounding box.
		const PointS &bottomRight () const;

		/// \return
		/// The top left point of the bounding box.
		const PointS &topLeft () const;

		/// \return
		/// The top right point of the bounding box.
		const PointS &topRight () const;

		/// \return
		/// The x-coordinate of the left edge of the bounding box.
		Scalar leftX () const;

		/// \return
		/// The x-coordinate of the right edge of the bounding box.
		Scalar rightX () const;

		/// \return
		/// The y-coordinate of the bottom edge of the bounding box.
		Scalar bottomY () const;

		/// \return
		/// The y-coordinate of the top edge of the bounding box.
		Scalar topY () const;

		/// Set the bounding box.
		/// \param[in] bottomLeft
		/// The bottom left point of the bounding box.
		/// \param[in] topRight
		/// The top right point of the bounding box.
		/// \exception std::invalid_argument
		/// If the two points are inverted or equal or if the new bounds do not contain all the segments in the map.
		void setBounds (const PointS &bottomLeft, const PointS &topRight);

		/// Check if a segment is inside the map bounds.
		/// \return
		/// \c true if \p segment is inside bounds, \c false otherwise.
		bool isSegmentInsideBounds (const SegmentS &segment) const;

		/// Check if a point is inside the map bounds.
		/// \return
		/// \c true if \p point is inside bounds, \c false otherwise.
		bool isPointInsideBounds (const PointS &point) const;

		/// Get the list of all the segments in the map.
		/// The list follows the inverse order of insertion of the segments.
		/// \return
		/// The list of the segments.
		const std::forward_list<SegmentS> &segments () const;

		/// Add a segment to the list of the segments and update the map accordingly.
		/// \tparam ArithmeticScalar
		/// The scalar type to use when performing the arithmetic operations needed to update the map.
		/// \param[in] segment
		/// The segment to add.
		/// \exception std::invalid_argument
		/// If \p segment is not inside the bounds, degenerate, duplicate, vertical, overlapping, intersecting or shares 
		/// the x-coordinate (but not the y-coordinate) of one of its endpoints with another segment in the map.
		template<class ArithmeticScalar = Scalar>
		void addSegment (const SegmentS &segment);

		/// Clear the map.
		/// \remark
		/// The root node obtained through root() const and all the trapezoids in the map will be invalidated.
		void clear ();

	};

}

#include "trapezoidal_map.tpp"
#include "trapezoidal_map_algorithms.tpp"

#endif