#ifndef GAS_DRAWING_TRAPEZOIDAL_MAP_DRAWER_IMPL_INCLUDED
#define GAS_DRAWING_TRAPEZOIDAL_MAP_DRAWER_IMPL_INCLUDED

#ifndef GAS_DRAWING_TRAPEZOIDAL_MAP_DRAWER_INCLUDED
#error 'gas/drawing/trapezoidal_map_drawer.tpp' should not be directly included
#endif

#include "trapezoidal_map_drawer.hpp"
#include <gas/utils/geometry.hpp>
#include <stdexcept>

namespace GAS
{

	namespace Drawing
	{

		template<class Scalar>
		void TrapezoidalMapDrawer<Scalar>::draw () const
		{
			ensureValid ();

#ifdef GAS_DRAWING_RESTORE_GL_STATE
			glPushAttrib (GL_COLOR_BUFFER_BIT);
#endif

			if (m_colorizer->requiresBlending ())
			{
				glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable (GL_BLEND);
			}
			else
			{
				glDisable (GL_BLEND);
			}

			int i {};
			m_renderer->beforeDraw (*m_map);

			for (const Trapezoid<Scalar> &t : *m_map)
			{
				Color color { m_colorizer->provideColor (*m_map, i, t) };
				m_renderer->draw (*m_map, i, t, color);
				i++;
			}

			m_renderer->afterDraw (*m_map);

#ifdef GAS_DRAWING_RESTORE_GL_STATE
			glPopAttrib ();
#endif

		}

		template<class Scalar>
		cg3::Point3d TrapezoidalMapDrawer<Scalar>::sceneCenter () const
		{
			ensureValidMap ();
			const Point<double> center { Geometry::cast<double> (m_map->bottomLeft () + m_map->topRight ()) / 2.0 };
			return { center.x (), center.y (), 0.0 };
		}

		template<class Scalar>
		double TrapezoidalMapDrawer<Scalar>::sceneRadius () const
		{
			ensureValidMap ();
			return Geometry::cast<double> (m_map->bottomLeft ()).dist (Geometry::cast<double> (m_map->topRight ())) / 2.0;
		}

		template<class Scalar>
		void TrapezoidalMapDrawer<Scalar>::ensureValid () const
		{
			ensureValidMap ();
			if (!m_renderer)
			{
				throw std::domain_error ("Painter cannot be null");
			}
			if (!m_colorizer)
			{
				throw std::domain_error ("Colorizer cannot be null");
			}
		}

		template<class Scalar>
		void TrapezoidalMapDrawer<Scalar>::ensureValidMap () const
		{
			if (!m_map)
			{
				throw std::domain_error ("Trapezoidal map cannot be null");
			}
		}

		template<class Scalar>
		TrapezoidalMapDrawer<Scalar>::TrapezoidalMapDrawer (const TrapezoidalMap<Scalar> &_map, TrapezoidColorizer<Scalar> &_colorizer, TrapezoidRenderer<Scalar> &_renderer)
			: m_map { &_map }, m_colorizer { &_colorizer }, m_renderer { &_renderer }
		{}

		template<class Scalar>
		const TrapezoidalMap<Scalar> *TrapezoidalMapDrawer<Scalar>::trapezoidalMap () const
		{
			return m_map;
		}

		template<class Scalar>
		const TrapezoidalMap<Scalar> *&TrapezoidalMapDrawer<Scalar>::trapezoidalMap ()
		{
			return m_map;
		}

		template<class Scalar>
		const TrapezoidColorizer<Scalar> *TrapezoidalMapDrawer<Scalar>::colorizer () const
		{
			return m_colorizer;
		}

		template<class Scalar>
		TrapezoidColorizer<Scalar> *&TrapezoidalMapDrawer<Scalar>::colorizer ()
		{
			return m_colorizer;
		}

		template<class Scalar>
		const TrapezoidRenderer<Scalar> *TrapezoidalMapDrawer<Scalar>::renderer () const
		{
			return m_renderer;
		}

		template<class Scalar>
		TrapezoidRenderer<Scalar> *&TrapezoidalMapDrawer<Scalar>::renderer ()
		{
			return m_renderer;
		}

	}

}

#endif