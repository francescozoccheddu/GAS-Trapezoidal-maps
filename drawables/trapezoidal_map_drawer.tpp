#pragma once

#include "trapezoidal_map_drawer.hpp"
#include <stdexcept>
#include <qstring>
#include <type_traits>

namespace GAS
{

	namespace Drawing
	{

		namespace Internals
		{
			inline void glColor (const Color &_color)
			{
				glColor4f (_color.redF (), _color.greenF (), _color.blueF (), _color.alphaF ());
			}

			template<class Scalar>
			inline void glVertexf (const Point<Scalar> &_point)
			{
				glVertex2f (static_cast<float>(_point.x ()), static_cast<float>(_point.y ()));
			}

			template<class Scalar>
			inline void glVertexd (const Point<Scalar> &_point)
			{
				glVertex2d (static_cast<double>(_point.x ()), static_cast<double>(_point.y ()));
			}

			template<class Scalar>
			inline void glVertexi (const Point<Scalar> &_point)
			{
				glVertex2i (static_cast<int>(_point.x ()), static_cast<int>(_point.y ()));
			}

			template<class Scalar>
			inline typename std::enable_if<!std::is_integral<Scalar>::value && !std::is_same<Scalar, double>::value>::type
				glVertex (const Point<Scalar> &_point) { glVertexf (_point); }

			template<class Scalar>
			inline typename std::enable_if<std::is_integral<Scalar>::value>::type
				glVertex (const Point<Scalar> &_point) { glVertexi (_point); }

			inline void glVertex (const Point<double> &_point) { glVertexd (_point); }



		}

		template<class Scalar>
		bool TrapezoidalMapDrawer<Scalar>::Colorizer::requiresBlending () const
		{
			return false;
		}

		template<class Scalar>
		void TrapezoidalMapDrawer<Scalar>::Painter::beforeDraw (const TrapezoidalMap<Scalar> &_trapezoidalMap) const
		{}

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
			m_painter->beforeDraw (*m_trapezoidalMap);

			for (const Trapezoid<Scalar> &t : *m_trapezoidalMap)
			{
				Color color { m_colorizer->provideColor (*m_trapezoidalMap, i, t) };
				m_painter->draw (*m_trapezoidalMap, i, t, color);
				i++;
			}

			m_painter->afterDraw (*m_trapezoidalMap);

#ifdef GAS_DRAWING_RESTORE_GL_STATE
			glPopAttrib ();
#endif

		}

		template<class Scalar>
		void TrapezoidalMapDrawer<Scalar>::Painter::afterDraw (const TrapezoidalMap<Scalar> &_trapezoidalMap) const
		{}

		template<class Scalar>
		cg3::Point3d TrapezoidalMapDrawer<Scalar>::sceneCenter () const
		{
			ensureValidMap ();
			const Point<Scalar> center { (m_trapezoidalMap->bottomLeft () + m_trapezoidalMap->topRight ()) / 2.0 };
			return { center.x (), center.y (), 0.0 };
		}

		template<class Scalar>
		double TrapezoidalMapDrawer<Scalar>::sceneRadius () const
		{
			ensureValidMap ();
			return m_trapezoidalMap->bottomLeft ().dist (m_trapezoidalMap->topRight ()) / 2.0;
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
		const TrapezoidalMap<Scalar> *TrapezoidalMapDrawer<Scalar>::trapezoidalMap () const
		{
			return m_trapezoidalMap;
		}

		template<class Scalar>
		void TrapezoidalMapDrawer<Scalar>::setTrapezoidalMap (const TrapezoidalMap<Scalar> *_trapezoidalMap)
		{
			m_trapezoidalMap = _trapezoidalMap;
		}

		template<class Scalar>
		const typename TrapezoidalMapDrawer<Scalar>::Colorizer *TrapezoidalMapDrawer<Scalar>::colorizer () const
		{
			return m_colorizer;
		}

		template<class Scalar>
		typename TrapezoidalMapDrawer<Scalar>::Colorizer *TrapezoidalMapDrawer<Scalar>::colorizer ()
		{
			return m_colorizer;
		}

		template<class Scalar>
		void TrapezoidalMapDrawer<Scalar>::setColorizer (Colorizer *_colorizer)
		{
			m_colorizer = _colorizer;
		}

		template<class Scalar>
		const typename TrapezoidalMapDrawer<Scalar>::Painter *TrapezoidalMapDrawer<Scalar>::painter () const
		{
			return m_painter;
		}

		template<class Scalar>
		typename TrapezoidalMapDrawer<Scalar>::Painter *TrapezoidalMapDrawer<Scalar>::painter ()
		{
			return m_painter;
		}

		template<class Scalar>
		void TrapezoidalMapDrawer<Scalar>::setPainter (Painter *_painter)
		{
			m_painter = _painter;
		}

		template<class Scalar>
		const Color &TrapezoidConstantColorizer<Scalar>::provideColor (const TrapezoidalMap<Scalar> &_trapezoidalMap, int _index, const Trapezoid<Scalar> &_trapezoid) const
		{
			return m_color;
		}

		template<class Scalar>
		bool TrapezoidConstantColorizer<Scalar>::requiresBlending () const
		{
			return m_color.alpha () < 255;
		}

		template<class Scalar>
		TrapezoidConstantColorizer<Scalar>::TrapezoidConstantColorizer (const Color &_color) : m_color { _color }
		{
			if (!m_color.isValid ())
			{
				throw std::invalid_argument ("Invalid color");
			}
		}

		template<class Scalar>
		const Color &TrapezoidConstantColorizer<Scalar>::color () const
		{
			return m_color;
		}

		template<class Scalar>
		void TrapezoidConstantColorizer<Scalar>::setColor (const Color &_color)
		{
			if (!_color.isValid ())
			{
				throw std::invalid_argument ("Invalid color");
			}
			m_color = _color;
		}

		template<class Scalar>
		const Color &TrapezoidFancyColorizer<Scalar>::provideColor (const TrapezoidalMap<Scalar> &_trapezoidalMap, int _index, const Trapezoid<Scalar> &_trapezoid) const
		{
			Color color;
			color.setHsvF (_index / (float) _trapezoidalMap.trapezoidsCount (), m_saturation, m_value);
			color.setAlphaF (m_alpha);
			return color;
		}

		template<class Scalar>
		bool TrapezoidFancyColorizer<Scalar>::requiresBlending () const
		{
			return m_alpha < 1.0f;
		}

		template<class Scalar>
		TrapezoidFancyColorizer<Scalar>::TrapezoidFancyColorizer (float _saturation, float _value, float _alpha) : m_saturation { _saturation }, m_value { _value }, m_alpha { _alpha }
		{
			if (_saturation < 0.0f || _saturation > 1.0f)
			{
				throw std::invalid_argument ("Saturation must be in range [0,1]");
			}
			if (_value < 0.0f || _value > 1.0f)
			{
				throw std::invalid_argument ("Value must be in range [0,1]");
			}
			if (_alpha < 0.0f || _alpha > 1.0f)
			{
				throw std::invalid_argument ("Alpha must be in range [0,1]");
			}
		}

		template<class Scalar>
		float TrapezoidFancyColorizer<Scalar>::saturation () const
		{
			return m_saturation;
		}

		template<class Scalar>
		void TrapezoidFancyColorizer<Scalar>::setSaturation (float _saturation)
		{
			if (_saturation < 0.0f || _saturation > 1.0f)
			{
				throw std::invalid_argument ("Saturation must be in range [0,1]");
			}
			m_saturation = _saturation;
		}

		template<class Scalar>
		float TrapezoidFancyColorizer<Scalar>::value () const
		{
			return m_value;
		}

		template<class Scalar>
		void TrapezoidFancyColorizer<Scalar>::setValue (float _value)
		{
			if (_value < 0.0f || _value > 1.0f)
			{
				throw std::invalid_argument ("Value must be in range [0,1]");
			}
			m_value = _value;
		}

		template<class Scalar>
		float TrapezoidFancyColorizer<Scalar>::alpha () const
		{
			return m_alpha;
		}

		template<class Scalar>
		void TrapezoidFancyColorizer<Scalar>::setAlpha (float _alpha)
		{
			if (_alpha < 0.0f || _alpha > 1.0f)
			{
				throw std::invalid_argument ("Alpha must be in range [0,1]");
			}
			m_alpha = _alpha;
		}

		template<class Scalar>
		void TrapezoidStrokePainter<Scalar>::beforeDraw (const TrapezoidalMap<Scalar> &_trapezoidalMap) const
		{
			if (m_thickness > 0)
			{
#ifdef GAS_DRAWING_RESTORE_GL_STATE
				glPushAttrib (GL_LINE_BIT | GL_ENABLE_BIT);
#endif
				glDisable (GL_DEPTH_TEST);
				glDisable (GL_LIGHTING);
				glLineWidth (m_thickness);
				glBegin (GL_LINES); // or GL_LINE_LOOP with multiple glBegin/glEnd calls
			}
		}

		template<class Scalar>
		void TrapezoidStrokePainter<Scalar>::draw (const TrapezoidalMap<Scalar> &_trapezoidalMap, int _index, const Trapezoid<Scalar> &_trapezoid, const Color &_color) const
		{
			if (m_thickness > 0 && _color.alpha () > 0)
			{
				Internals::glColor (_color);
				const Point<Scalar>
					&a { _trapezoid.bottomLeft () },
					&b { _trapezoid.topLeft () },
					&c { _trapezoid.topRight () },
					&d { _trapezoid.bottomRight () };
				Internals::glVertex (a);
				Internals::glVertex (b);
				Internals::glVertex (b);
				Internals::glVertex (c);
				Internals::glVertex (c);
				Internals::glVertex (d);
				Internals::glVertex (d);
				Internals::glVertex (a);
			}
		}

		template<class Scalar>
		void TrapezoidStrokePainter<Scalar>::afterDraw (const TrapezoidalMap<Scalar> &_trapezoidalMap) const
		{
			if (m_thickness > 0)
			{
				glEnd ();
#ifdef GAS_DRAWING_RESTORE_GL_STATE
				glPopAttrib ();
#endif
			}
		}

		template<class Scalar>
		TrapezoidStrokePainter<Scalar>::TrapezoidStrokePainter (int _thickness) : m_thickness { _thickness }
		{
			if (_thickness < 0)
			{
				throw std::invalid_argument ("Thickness cannot be negative");
			}
		}

		template<class Scalar>
		int TrapezoidStrokePainter<Scalar>::thickness () const
		{
			return m_thickness;
		}

		template<class Scalar>
		void TrapezoidStrokePainter<Scalar>::setThickness (int _thickness)
		{
			if (_thickness < 0)
			{
				throw std::invalid_argument ("Thickness cannot be negative");
			}
			m_thickness = _thickness;
		}

		template<class Scalar>
		void TrapezoidFillPainter<Scalar>::beforeDraw (const TrapezoidalMap<Scalar> &_trapezoidalMap) const
		{
#ifdef GAS_DRAWING_RESTORE_GL_STATE
			glPushAttrib (GL_ENABLE_BIT);
#endif
			glDisable (GL_DEPTH_TEST);
			glDisable (GL_LIGHTING);
			glBegin (GL_QUADS);
		}

		template<class Scalar>
		void TrapezoidFillPainter<Scalar>::draw (const TrapezoidalMap<Scalar> &_trapezoidalMap, int _index, const Trapezoid<Scalar> &_trapezoid, const Color &_color) const
		{
			if (_color.alpha () > 0)
			{
				Internals::glColor (_color);
				Internals::glVertex (_trapezoid.bottomLeft ());
				Internals::glVertex (_trapezoid.topLeft ());
				Internals::glVertex (_trapezoid.topRight ());
				Internals::glVertex (_trapezoid.bottomRight ());
			}
		}

		template<class Scalar>
		void TrapezoidFillPainter<Scalar>::afterDraw (const TrapezoidalMap<Scalar> &_trapezoidalMap) const
		{
			glEnd ();
#ifdef GAS_DRAWING_RESTORE_GL_STATE
			glPopAttrib ();
#endif
		}

		template<class Scalar>
		QString TrapezoidTextPainter<Scalar>::getTrapezoidSerial (const Trapezoid<Scalar> *_trapezoid)
		{
			return QString::fromStdString (_trapezoid ? _trapezoid->serial () : GAS::Utils::Serial::null);
		}

		template<class Scalar>
		bool TrapezoidTextPainter<Scalar>::isRectInsideTrapezoid (const Point<Scalar> &_center, Scalar _halfWidth, Scalar _halfHeight, const Trapezoid<Scalar> &_trapezoid)
		{
			return
				_trapezoid.contains (_center + Point<Scalar>{-_halfWidth, -_halfHeight}) &&
				_trapezoid.contains (_center + Point<Scalar>{-_halfWidth, _halfHeight}) &&
				_trapezoid.contains (_center + Point<Scalar>{_halfWidth, _halfHeight}) &&
				_trapezoid.contains (_center + Point<Scalar>{_halfWidth, -_halfHeight});
		}

		template<class Scalar>
		void TrapezoidTextPainter<Scalar>::drawText (const QString &_text, const Trapezoid<Scalar> &_trapezoid) const
		{
			const int width { m_fontMetrics.width (_text) }, height { m_fontMetrics.height () };
			const Point<Scalar> center { _trapezoid.centroid () };
			const qglviewer::Vec screenCenter { m_canvas->camera ()->projectedCoordinatesOf ({ center.x (), center.y (), 0 }) };
			{
				const qreal ratio { m_canvas->camera ()->pixelGLRatio ({ center.x (), center.y (), 0 }) };
				Scalar halfWidth { static_cast<Scalar> (width * ratio / 2.0) };
				Scalar halfHeight { static_cast<Scalar> (height * ratio / 2.0) };
				if (!isRectInsideTrapezoid (center, halfWidth, halfHeight, _trapezoid))
				{
					return;
				}
			}
			m_canvas->drawText (screenCenter.x - width / 2, screenCenter.y + height / 2, _text, m_font);
		}

		template<class Scalar>
		void TrapezoidTextPainter<Scalar>::beforeDraw (const TrapezoidalMap<Scalar> &_trapezoidalMap) const
		{
#ifdef GAS_DRAWING_RESTORE_GL_STATE
			glPushAttrib (GL_ENABLE_BIT);
#endif
			glDisable (GL_DEPTH_TEST);
			glDisable (GL_LIGHTING);
		}

		template<class Scalar>
		inline void TrapezoidTextPainter<Scalar>::draw (const TrapezoidalMap<Scalar> &_trapezoidalMap, int _index, const Trapezoid<Scalar> &_trapezoid, const Color &_color) const
		{
			if (_color.alpha () > 0)
			{
				glColor3f (_color.redF (), _color.greenF (), _color.blueF ());
				const Point<Scalar> centroid { _trapezoid.centroid () };
#ifdef GAS_DRAWING_ENABLE_TRAPEZOID_SERIAL
				QString text { QString { "%1 (%2 %3 %4 %5)" }
					.arg (getTrapezoidSerial (&_trapezoid))
					.arg (getTrapezoidSerial (_trapezoid.lowerLeftNeighbor ()))
					.arg (getTrapezoidSerial (_trapezoid.upperLeftNeighbor ()))
					.arg (getTrapezoidSerial (_trapezoid.upperRightNeighbor ()))
					.arg (getTrapezoidSerial (_trapezoid.lowerRightNeighbor ()))
				};
#else
				QString text { QString::number (_index) };
#endif 
				drawText (text, _trapezoid);
			}
		}

		template<class Scalar>
		void TrapezoidTextPainter<Scalar>::afterDraw (const TrapezoidalMap<Scalar> &_trapezoidalMap) const
		{
#ifdef GAS_DRAWING_RESTORE_GL_STATE
			glPopAttrib ();
#endif
		}

		template<class Scalar>
		TrapezoidTextPainter<Scalar>::TrapezoidTextPainter (cg3::viewer::GLCanvas &_canvas, const QFont &_font) : m_canvas { &_canvas }, m_font { _font }, m_fontMetrics { m_font }
		{}

		template<class Scalar>
		const QFont &TrapezoidTextPainter<Scalar>::font () const
		{
			return m_font;
		}

		template<class Scalar>
		void TrapezoidTextPainter<Scalar>::setFont (const QFont &_font)
		{
			m_font = _font;
			m_fontMetrics = QFontMetrics (m_font);
		}
	}

}