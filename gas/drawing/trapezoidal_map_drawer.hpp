/// GAS::Drawing::TrapezoidalMapDrawer class for GAS::TrapezoidalMap rendering.
/// \file
/// \author Francesco Zoccheddu

#ifndef GAS_DRAWING_TRAPEZOIDAL_MAP_DRAWER_INCLUDED
#define GAS_DRAWING_TRAPEZOIDAL_MAP_DRAWER_INCLUDED

#include <gas/data/trapezoidal_map.hpp>
#include <gas/data/trapezoid.hpp>
#include <cg3/viewer/interfaces/drawable_object.h>
#include <cg3/geometry/point3.h>
#include <gas/drawing/color.hpp>
#include <gas/drawing/trapezoid_colorizers.hpp>
#include <gas/drawing/trapezoid_renderers.hpp>

namespace GAS
{

	/// TrapezoidalMap rendering system.
	namespace Drawing
	{

		/// \c cg3::DrawableObject implementation that draws TrapezoidalMap objects.
		/// The render style is defined through a TrapezoidRenderer component and the trapezoids color through a TrapezoidColorizer component.
		/// \tparam Scalar
		/// The scalar type.
		template<class Scalar>
		class TrapezoidalMapDrawer : public cg3::DrawableObject
		{

			const TrapezoidalMap<Scalar> *m_map {};
			TrapezoidColorizer<Scalar> *m_colorizer {};
			TrapezoidRenderer<Scalar> *m_renderer {};

		protected:

			virtual void draw () const override final;
			virtual cg3::Point3d sceneCenter () const override final;
			virtual double sceneRadius () const override final;

			/// Throw if the drawing components or the trapezoidal map are nullptr.
			void ensureValid () const;

			/// Throw if the trapezoidal map is nullptr.
			void ensureValidMap () const;

		public:

			/// Construct a drawer.
			/// \remark
			/// The drawing components (TrapezoidColorizer and TrapezoidRenderer) must be set before start drawing.
			TrapezoidalMapDrawer () = default;

			/// Construct a drawer with the specified components to draw the specified map.
			/// \param[in] map
			/// The trapezoidal map to draw.
			/// \param[in] colorizer
			/// The colorizer component to use for drawing.
			/// \param[in] renderer
			/// The renderer component to use for drawing.
			TrapezoidalMapDrawer (const TrapezoidalMap<Scalar> &map, TrapezoidColorizer<Scalar> &colorizer, TrapezoidRenderer<Scalar> &renderer);

			virtual ~TrapezoidalMapDrawer () = default;

			/// \return
			/// The trapezoidal map to draw.
			const TrapezoidalMap<Scalar> *trapezoidalMap () const;

			/// \copydoc trapezoidalMap
			const TrapezoidalMap<Scalar> *&trapezoidalMap ();

			/// \return
			/// The colorizer component to use for drawing.
			const TrapezoidColorizer<Scalar> *colorizer () const;

			/// \copydoc colorizer
			TrapezoidColorizer<Scalar> *&colorizer ();

			/// \return
			/// The renderer component to use for drawing.
			const TrapezoidRenderer<Scalar> *renderer () const;

			/// \copydoc renderer
			TrapezoidRenderer<Scalar> *&renderer ();

		};

	}

}

#include "trapezoidal_map_drawer.tpp"

#endif