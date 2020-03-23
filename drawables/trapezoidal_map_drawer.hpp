#pragma once

#include <data_structures/trapezoidal_map.hpp>
#include <data_structures/trapezoid.hpp>
#include <cg3/viewer/interfaces/drawable_object.h>
#include <cg3/geometry/point3.h>
#include "color.hpp"
#include "trapezoid_brushes.hpp"
#include "trapezoid_colorizers.hpp"

namespace GAS
{

	namespace Drawing
	{

		template<class Scalar>
		class TrapezoidalMapDrawer : public cg3::DrawableObject
		{

			const TrapezoidalMap<Scalar> *m_map {};
			TrapezoidColorizer<Scalar> *m_colorizer {};
			TrapezoidBrush<Scalar> *m_brush {};

		protected:

			virtual void draw () const override final;
			virtual cg3::Point3d sceneCenter () const override final;
			virtual double sceneRadius () const override final;

			void ensureValid () const;
			void ensureValidMap () const;

		public:

			TrapezoidalMapDrawer () = default;
			TrapezoidalMapDrawer (const TrapezoidalMap<Scalar> &map, TrapezoidColorizer<Scalar> &colorizer, TrapezoidBrush<Scalar> &brush);

			virtual ~TrapezoidalMapDrawer () = default;

			const TrapezoidalMap<Scalar> *trapezoidalMap () const;
			const TrapezoidalMap<Scalar> *&trapezoidalMap ();
			const TrapezoidColorizer<Scalar> *colorizer () const;
			TrapezoidColorizer<Scalar> *&colorizer ();
			const TrapezoidBrush<Scalar> *brush () const;
			TrapezoidBrush<Scalar> *&brush ();

		};

	}

}

#include "trapezoidal_map_drawer.tpp"
