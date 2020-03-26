#ifndef GAS_DRAWING_TRAPEZOIDAL_MAP_DRAWER_IMPL_INCLUDED
#define GAS_DRAWING_TRAPEZOIDAL_MAP_DRAWER_IMPL_INCLUDED

#ifndef GAS_DRAWING_TRAPEZOIDAL_MAP_DRAWER_INCLUDED
#error 'gas/drawing/trapezoidal_map_drawer.tpp' should not be directly included
#endif

#include "trapezoidal_map_drawer.hpp"
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
			m_brush->beforeDraw (*m_map);

			for (const Trapezoid<Scalar> &t : *m_map)
			{
				Color color { m_colorizer->provideColor (*m_map, i, t) };
				m_brush->draw (*m_map, i, t, color);
				i++;
			}

			m_brush->afterDraw (*m_map);

#ifdef GAS_DRAWING_RESTORE_GL_STATE
			glPopAttrib ();
#endif

		}

		template<class Scalar>
		cg3::Point3d TrapezoidalMapDrawer<Scalar>::sceneCenter () const
		{
			ensureValidMap ();
			const Point<Scalar> center { (m_map->bottomLeft () + m_map->topRight ()) / 2.0 };
			return { center.x (), center.y (), 0.0 };
		}

		template<class Scalar>
		double TrapezoidalMapDrawer<Scalar>::sceneRadius () const
		{
			ensureValidMap ();
			return m_map->bottomLeft ().dist (m_map->topRight ()) / 2.0;
		}

		template<class Scalar>
		void TrapezoidalMapDrawer<Scalar>::ensureValid () const
		{
			ensureValidMap ();
			if (!m_brush)
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
		TrapezoidalMapDrawer<Scalar>::TrapezoidalMapDrawer (const TrapezoidalMap<Scalar> &_map, TrapezoidColorizer<Scalar> &_colorizer, TrapezoidBrush<Scalar> &_brush)
			: m_map { &_map }, m_colorizer { &_colorizer }, m_brush { &_brush }
		{}

		template<class Scalar>
		const TrapezoidalMap<Scalar> *TrapezoidalMapDrawer<Scalar>::trapezoidalMap () const
		{
			return m_trapezoidalMap;
		}

		template<class Scalar>
		const TrapezoidalMap<Scalar> *&TrapezoidalMapDrawer<Scalar>::trapezoidalMap ()
		{
			return m_trapezoidalMap;
		}

		template<class Scalar>
		const typename TrapezoidColorizer<Scalar> *TrapezoidalMapDrawer<Scalar>::colorizer () const
		{
			return m_colorizer;
		}

		template<class Scalar>
		typename TrapezoidColorizer<Scalar> *&TrapezoidalMapDrawer<Scalar>::colorizer ()
		{
			return m_colorizer;
		}

		template<class Scalar>
		const typename TrapezoidBrush<Scalar> *TrapezoidalMapDrawer<Scalar>::brush () const
		{
			return m_brush;
		}

		template<class Scalar>
		typename TrapezoidBrush<Scalar> *&TrapezoidalMapDrawer<Scalar>::brush ()
		{
			return m_brush;
		}

	}

}

#endif