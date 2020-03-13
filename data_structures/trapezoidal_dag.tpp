#pragma once

#include "trapezoidal_dag.hpp"

#include <cassert>


namespace GAS
{

	namespace TDAG
	{

		template<class Scalar>
		Split<Scalar>::Split (const Scalar &_x)
		{
			setVertical (_x);
		}

		template<class Scalar>
		Split<Scalar>::Split (const Segment<Scalar> &_segment)
		{
			setSegment (_segment);
		}

		template<class Scalar>
		ESplitType Split<Scalar>::getType () const
		{
			return m_type;
		}

		template<class Scalar>
		const Scalar &Split<Scalar>::getX () const
		{
			assert (m_type == ESplitType::Vertical);
			return *m_data.x;
		}

		template<class Scalar>
		const Segment<Scalar> &Split<Scalar>::getSegment () const
		{
			assert (m_type == ESplitType::NonVertical);
			return *m_data.segment;
		}

		template<class Scalar>
		void Split<Scalar>::setVertical (const Scalar &_x)
		{
			m_type = ESplitType::Vertical;
			m_data.x = &_x;
		}

		template<class Scalar>
		void Split<Scalar>::setSegment (const Segment<Scalar> &_segment)
		{
			m_type = ESplitType::NonVertical;
			m_data.segment = &_segment;
		}

	}

}