#pragma once

#include "point.hpp"
#include "segment.hpp"
#include "trapezoid.hpp"
#include "trapezoidal_dag.hpp"
#include <forward_list>
#include <deque>
#include <unordered_set>

namespace GAS
{

	template<class Scalar>
	class TrapezoidalMap final
	{

	public:

		using Point = Point<Scalar>;
		using Segment = Segment<Scalar>;
		using Trapezoid = Trapezoid<Scalar>;
		using Node = TDAG::Node<Scalar>;
		using TrapezoidIterator = typename std::unordered_set<Trapezoid *>::const_iterator;

	private:

		class Pair
		{

		private:

			Trapezoid *m_a, *m_b;

		public:

			Pair (Trapezoid &leftOrBottom, Trapezoid &rightOrTop);

			bool isSplit () const;
			bool isLeftAligned () const;
			bool isRightAligned () const;
			bool isHorizontal () const;
			bool isVertical () const;

			Trapezoid &compact () const;
			Trapezoid &bottom () const;
			Trapezoid &left () const;
			Trapezoid &top () const;
			Trapezoid &right () const;

		};

		enum class ELeftWeldJointType
		{
			Compact, Split, JointBottom, JointTop
		};

		enum class ERightWeldFitness
		{
			Fit, Extended, Shrinked
		};

		struct RightWeldConfiguration
		{
			bool split;
			ERightWeldFitness bottomFitness, topFitness;
		};

		static ELeftWeldJointType getLeftWeldJointType (Pair left);
		static ERightWeldFitness getRightWeldBottomFitness (Pair left, Pair right);
		static ERightWeldFitness getRightWeldTopFitness (Pair left, Pair right);
		static RightWeldConfiguration getRightWeldConfiguration (Pair left, Pair right);
		static void weld (Pair left, Pair right);

		std::forward_list<Segment> m_segments;
		Node *m_dag {};
		std::unordered_set<Trapezoid *> m_trapezoids;

		Segment m_bottom, m_top;

		void deleteAll ();
		void initialize ();

		Node &getNode (Trapezoid &trapezoid) const;
		Trapezoid &createTrapezoid (const Trapezoid &copy = {});
		Trapezoid &createTrapezoid (Trapezoid &&moved);
		void splitTrapezoid (Trapezoid &trapezoid, Scalar x, Trapezoid &left, Trapezoid &right);
		void splitTrapezoid (Trapezoid &trapezoid, Segment segment, Trapezoid &left, Trapezoid &right);

		Trapezoid &findLeftmostIntersectedTrapezoid (const Segment &segment) const;

		Pair splitVertically (Trapezoid &trapezoid, const Point &point);
		Pair incrementalSplitHorizontally (Trapezoid &trapezoid, const Segment &segment, Pair previous);

	public:

		TrapezoidalMap (const Point &bottomLeft, const Point &topRight);

		// Trapezoids
		int trapezoidsCount () const;
		TrapezoidIterator begin () const;
		TrapezoidIterator end () const;

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
		void clear ();
		void addSegment (const Segment &segment);

	};

}

#include "trapezoidal_map.tpp"
#include "trapezoidal_map_algorithms.tpp"
