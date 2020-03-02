#pragma once

#include "trapezoidal_map_drawer.hpp"
#include <stdexcept>
#include <cg3/viewer/opengl_objects/opengl_objects2.h>

namespace GAS
{

	namespace Drawing
	{

		namespace Internals
		{

			template<class Scalar>
			void drawTrapezoid (const Trapezoid<Scalar> &_trapezoid, cg3::Color _color, int _thickness, bool _fill)
			{
				// TODO Implement
			}

		}

		template<class Scalar>
		void TrapezoidalMapDrawer<Scalar>::draw () const
		{
			ensureValid ();
			const std::vector<Trapezoid<Scalar>> &trapezoids { m_trapezoidalMap->getTrapezoids () };
			for (int i { 0 }; i < trapezoids.size; i++)
			{
				cg3::Color color { m_colorizer->provideColor (*m_trapezoidalMap, i) };
				m_painter->draw (*m_trapezoidalMap, i, color);
			}
		}

		template<class Scalar>
		cg3::Point3d TrapezoidalMapDrawer<Scalar>::sceneCenter () const
		{
			ensureValidMap ();
			return (m_trapezoidalMap->getBottomLeft () + m_trapezoidalMap->getTopRight ()) / 2.0;
		}

		template<class Scalar>
		double TrapezoidalMapDrawer<Scalar>::sceneRadius () const
		{
			ensureValidMap ();
			return m_trapezoidalMap->getBottomLeft ().dist (m_trapezoidalMap->getTopRight ()) / 2.0;
		}

		template<class Scalar>
		void TrapezoidalMapDrawer<Scalar>::ensureValid () const
		{
			ensureValidMap ();
			if (!m_painter)
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
			if (!m_trapezoidalMap)
			{
				throw std::domain_error ("Trapezoidal map cannot be null");
			}
		}

		template<class Scalar>
		const TrapezoidalMap<Scalar> *TrapezoidalMapDrawer<Scalar>::getTrapezoidalMap () const
		{
			return m_trapezoidalMap;
		}

		template<class Scalar>
		void TrapezoidalMapDrawer<Scalar>::setTrapezoidalMap (const TrapezoidalMap<Scalar> *_trapezoidalMap)
		{
			m_trapezoidalMap = _trapezoidalMap;
		}

		template<class Scalar>
		const TrapezoidalMapDrawer<Scalar>::Colorizer *TrapezoidalMapDrawer<Scalar>::getColorizer () const
		{
			return m_colorizer;
		}

		template<class Scalar>
		TrapezoidalMapDrawer<Scalar>::Colorizer *TrapezoidalMapDrawer<Scalar>::getColorizer ()
		{
			return m_colorizer;
		}

		template<class Scalar>
		void TrapezoidalMapDrawer<Scalar>::setColorizer (Colorizer *_colorizer)
		{
			m_colorizer = _colorizer;
		}

		template<class Scalar>
		const TrapezoidalMapDrawer<Scalar>::Painter *TrapezoidalMapDrawer<Scalar>::getPainter () const
		{
			return m_painter;
		}

		template<class Scalar>
		TrapezoidalMapDrawer<Scalar>::Painter *TrapezoidalMapDrawer<Scalar>::getPainter ()
		{
			return m_painter;
		}

		template<class Scalar>
		void TrapezoidalMapDrawer<Scalar>::setPainter (Painter *_painter)
		{
			m_painter = _painter;
		}

		template<class Scalar>
		const cg3::Color &TrapezoidConstantColorizer<Scalar>::provideColor (const TrapezoidalMap<Scalar> &_trapezoidalMap, int _index) const
		{
			return color;
		}

		template<class Scalar>
		const cg3::Color &TrapezoidFancyColorizer<Scalar>::provideColor (const TrapezoidalMap<Scalar> &_trapezoidalMap, int _index) const
		{
			cg3::Color color;
			color.setHsvF (_index / (float) _trapezoidalMap.getTrapezoids ().size, m_saturation, m_value);
			color.setAlphaF (m_alpha);
			return color;
		}

		template<class Scalar>
		float TrapezoidFancyColorizer<Scalar>::getSaturation () const
		{
			return m_saturation;
		}

		template<class Scalar>
		void TrapezoidFancyColorizer<Scalar>::setSaturation (float _saturation) const
		{
			if (_saturation < 0.0f || _saturation > 1.0f)
			{
				throw std::domain_error ("Saturation must be in range [0,1]");
			}
			m_saturation = _saturation;
		}

		template<class Scalar>
		float TrapezoidFancyColorizer<Scalar>::getValue () const
		{
			return m_value;
		}

		template<class Scalar>
		void TrapezoidFancyColorizer<Scalar>::setValue (float _value)
		{
			if (_value < 0.0f || _value > 1.0f)
			{
				throw std::domain_error ("Value must be in range [0,1]");
			}
			m_value = _value;
		}

		template<class Scalar>
		float TrapezoidFancyColorizer<Scalar>::getAlpha () const
		{
			return m_alpha;
		}

		template<class Scalar>
		void TrapezoidFancyColorizer<Scalar>::setAlpha (float _alpha)
		{
			if (_alpha < 0.0f || _alpha > 1.0f)
			{
				throw std::domain_error ("Alpha must be in range [0,1]");
			}
			m_alpha = _alpha;
		}

		template<class Scalar>
		void TrapezoidStrokePainter<Scalar>::draw (const TrapezoidalMap<Scalar> &_trapezoidalMap, int _index, const cg3::Color &_color) const
		{
			const Trapezoid<Scalar> &trapezoid { _trapezoidalMap->getTrapezoids ()[_index] };
			Internals::drawTrapezoid (trapezoid, _color, m_thickness, false);
		}

		template<class Scalar>
		int TrapezoidStrokePainter<Scalar>::getThickness () const
		{
			return m_thickness;
		}

		template<class Scalar>
		void TrapezoidStrokePainter<Scalar>::setThickness (int _thickness)
		{
			if (_thickness < 0)
			{
				throw std::domain_error ("Thickness cannot be negative");
			}
			m_thickness = _thickness;
		}

		template<class Scalar>
		void TrapezoidFillPainter<Scalar>::draw (const TrapezoidalMap<Scalar> &_trapezoidalMap, int _index, const cg3::Color &_color) const
		{
			const Trapezoid<Scalar> &trapezoid { _trapezoidalMap->getTrapezoids ()[_index] };
			Internals::drawTrapezoid (trapezoid, _color, 0, true);
		}

	}

}