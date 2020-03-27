#ifndef GAS_DATA_TRAPEZOIDAL_MAP_INCLUDED
#define GAS_DATA_TRAPEZOIDAL_MAP_INCLUDED

#include <gas/data/point.hpp>
#include <gas/data/segment.hpp>
#include <gas/data/trapezoid.hpp>
#include <gas/data/trapezoidal_dag.hpp>
#include <forward_list>

namespace GAS
{

	template<class Scalar>
	class TrapezoidalMap final
	{

		using Point = Point<Scalar>;
		using Segment = Segment<Scalar>;
		using Trapezoid = Trapezoid<Scalar>;
		using Node = TDAG::Node<Scalar>;
		using NodeData = TDAG::NodeData<Scalar>;
		using Graph = TDAG::Graph<Scalar>;

		class Pair
		{

		protected:

			Trapezoid *m_a, *m_b;

			Pair (Trapezoid *leftOrBottom, Trapezoid *rightOrTop);

		public:

			Pair (Trapezoid &leftOrBottom, Trapezoid &rightOrTop);

			bool isSplit () const;
			bool isLeftAligned () const;
			bool isRightAligned () const;

			Trapezoid &left () const;
			Trapezoid &right () const;
			Trapezoid &bottom () const;
			Trapezoid &top () const;

		};

		class NullablePair : public Pair
		{

		public:

			static const NullablePair null;

			static NullablePair allOrNone (Trapezoid *leftOrBottom, Trapezoid *rightOrTop);

			using Pair::Pair;

			NullablePair &operator=(const NullablePair &) = default;
			NullablePair &operator=(const Pair &);

			operator bool () const;

		};

		static void weld (Pair left, Pair right);
		static void weld (Trapezoid &trapezoid, Pair neighbors, bool right);

		std::forward_list<Segment> m_segments; // for stable references
		Graph m_graph;

		/*
			I could have used cg3::BoundingBox2 but i needed this two segments to be referenceable.
		*/
		Segment m_bottom, m_top;

		Node &root ();

		void destroy ();
		void initialize ();

		Node &getNode (Trapezoid &trapezoid) const;
		Trapezoid &createTrapezoid (const Trapezoid &copy = {});
		void splitTrapezoid (Trapezoid &trapezoid, const Scalar &x, Trapezoid &left, Trapezoid &right);
		void splitTrapezoid (Trapezoid &trapezoid, const Segment &segment, Trapezoid &left, Trapezoid &right);

		Trapezoid &findLeftmostIntersectedTrapezoid (const Segment &segment);

		Pair splitVertically (Trapezoid &trapezoid, const Point &point);
		Pair incrementalSplitHorizontally (Trapezoid &trapezoid, const Segment &segment, NullablePair previous);
		void addValidSegment (const Segment &segment);

	public:

		TrapezoidalMap (const Point &bottomLeft, const Point &topRight);

		TrapezoidalMap (const TrapezoidalMap &copy) = default;
		TrapezoidalMap (TrapezoidalMap &&moved);

		TrapezoidalMap &operator =(const TrapezoidalMap &copy) = default;
		TrapezoidalMap &operator =(TrapezoidalMap &&moved);

		const Node &root () const;

		// Trapezoids
		int trapezoidsCount () const;
		TDAG::Utils::ConstTrapezoidIterator<Scalar> begin () const;
		TDAG::Utils::ConstTrapezoidIterator<Scalar> end () const;
		const Trapezoid &query (const Point &point) const;

		// Bounds
		const Point &bottomLeft () const;
		const Point &bottomRight () const;
		const Point &topLeft () const;
		const Point &topRight () const;
		const Segment &top () const;
		const Segment &bottom () const;
		Scalar leftX () const;
		Scalar rightX () const;
		Scalar bottomY () const;
		Scalar topY () const;
		void setBounds (const Point &bottomLeft, const Point &topRight);
		bool isSegmentInsideBounds (const Segment &segment) const;
		bool isPointInsideBounds (const Point &point) const;

		// Segments
		const std::forward_list<Segment> &segments () const;
		void addSegment (const Segment &segment);
		void clear ();

	};

}

#include "trapezoidal_map.tpp"
#include "trapezoidal_map_algorithms.tpp"

#endif