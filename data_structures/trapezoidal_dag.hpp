#pragma once

#include "point.hpp"
#include "segment.hpp"
#include "trapezoid.hpp"
#include "binary_dag.hpp"
#include <utils/geometry_utils.hpp>

namespace GAS
{

	namespace TDAG
	{

		enum class ESplitType
		{
			Vertical, NonVertical
		};


		template<class Scalar>
		class Split
		{

			ESplitType m_type;

			union
			{
				const Scalar *x;
				const Segment<Scalar> *segment;
			} m_data;

		public:

			Split (const Scalar &x);
			Split (const Segment<Scalar> &segment);

			ESplitType getType () const;

			const Scalar &getX () const;
			const Segment<Scalar> &getSegment () const;

			void setVertical (const Scalar &x);
			void setSegment (const Segment<Scalar> &segment);

		};

		template<class Scalar>
		using Node = BDAG::Node<Trapezoid<Scalar>, Split<Scalar>>;

	}

}

#include "trapezoidal_dag.tpp"