#ifndef GAS_DRAWING_TRAPEZOID_BRUSHES_IMPL_INCLUDED
#define GAS_DRAWING_TRAPEZOID_BRUSHES_IMPL_INCLUDED

#ifndef GAS_DRAWING_TRAPEZOID_BRUSHES_INCLUDED
#error 'gas/drawing/trapezoid_brushes.tpp' should not be directly included
#endif


#include "trapezoid_brushes.hpp"

#include <stdexcept>
#include <qstring>
#include <type_traits>

/*
	I could have used cg3::opengl::draw* functions but they don't support transparency and
	they don't allow drawing quads with zero width strokes as glLineWidth(0) is illegal by OpenGL specs.
*/

#ifdef WIN32
#include "windows.h"
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

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
		void TrapezoidBrush<Scalar>::beforeDraw (const TrapezoidalMap<Scalar> &/*_trapezoidalMap*/) const
		{}

		template<class Scalar>
		void TrapezoidBrush<Scalar>::afterDraw (const TrapezoidalMap<Scalar> &/*_trapezoidalMap*/) const
		{}

		namespace TrapezoidBrushes
		{

			template<class Scalar>
			void Stroke<Scalar>::beforeDraw (const TrapezoidalMap<Scalar> &/*_trapezoidalMap*/) const
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
			void Stroke<Scalar>::draw (const TrapezoidalMap<Scalar> &/*_trapezoidalMap*/, int /*_index*/, const Trapezoid<Scalar> &_trapezoid, const Color &_color) const
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
			void Stroke<Scalar>::afterDraw (const TrapezoidalMap<Scalar> &/*_trapezoidalMap*/) const
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
			Stroke<Scalar>::Stroke (int _thickness) : m_thickness { _thickness }
			{
				if (_thickness < 0)
				{
					throw std::invalid_argument ("Thickness cannot be negative");
				}
			}

			template<class Scalar>
			int Stroke<Scalar>::thickness () const
			{
				return m_thickness;
			}

			template<class Scalar>
			void Stroke<Scalar>::setThickness (int _thickness)
			{
				if (_thickness < 0)
				{
					throw std::invalid_argument ("Thickness cannot be negative");
				}
				m_thickness = _thickness;
			}

			template<class Scalar>
			void Fill<Scalar>::beforeDraw (const TrapezoidalMap<Scalar> &/*_trapezoidalMap*/) const
			{
#ifdef GAS_DRAWING_RESTORE_GL_STATE
				glPushAttrib (GL_ENABLE_BIT);
#endif
				glDisable (GL_DEPTH_TEST);
				glDisable (GL_LIGHTING);
				glBegin (GL_QUADS);
			}

			template<class Scalar>
			void Fill<Scalar>::draw (const TrapezoidalMap<Scalar> &/*_trapezoidalMap*/, int /*_index*/, const Trapezoid<Scalar> &_trapezoid, const Color &_color) const
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
			void Fill<Scalar>::afterDraw (const TrapezoidalMap<Scalar> &/*_trapezoidalMap*/) const
			{
				glEnd ();
#ifdef GAS_DRAWING_RESTORE_GL_STATE
				glPopAttrib ();
#endif
			}

			template<class Scalar>
			QString Text<Scalar>::getTrapezoidSerial (const Trapezoid<Scalar> *_trapezoid)
			{
				return QString::fromStdString (_trapezoid ? _trapezoid->serial () : GAS::Utils::Serial::null);
			}

			template<class Scalar>
			bool Text<Scalar>::isRectInsideTrapezoid (const Point<Scalar> &_center, Scalar _halfWidth, Scalar _halfHeight, const Trapezoid<Scalar> &_trapezoid)
			{
				return
					_trapezoid.contains (_center + Point<Scalar>{-_halfWidth, -_halfHeight}) &&
					_trapezoid.contains (_center + Point<Scalar>{-_halfWidth, _halfHeight}) &&
					_trapezoid.contains (_center + Point<Scalar>{_halfWidth, _halfHeight}) &&
					_trapezoid.contains (_center + Point<Scalar>{_halfWidth, -_halfHeight});
			}

			template<class Scalar>
			void Text<Scalar>::drawText (const QString &_text, const Trapezoid<Scalar> &_trapezoid) const
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
			void Text<Scalar>::beforeDraw (const TrapezoidalMap<Scalar> &/*_trapezoidalMap*/) const
			{
#ifdef GAS_DRAWING_RESTORE_GL_STATE
				glPushAttrib (GL_ENABLE_BIT);
#endif
				glDisable (GL_DEPTH_TEST);
				glDisable (GL_LIGHTING);
			}

			template<class Scalar>
			void Text<Scalar>::draw (const TrapezoidalMap<Scalar> &/*_trapezoidalMap*/, int /*_index*/, const Trapezoid<Scalar> &_trapezoid, const Color &_color) const
			{
				if (!m_canvas)
				{
					throw std::domain_error ("Canvas is null");
				}
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
			void Text<Scalar>::afterDraw (const TrapezoidalMap<Scalar> &/*_trapezoidalMap*/) const
			{
#ifdef GAS_DRAWING_RESTORE_GL_STATE
				glPopAttrib ();
#endif
			}

			template<class Scalar>
			Text<Scalar>::Text (cg3::viewer::GLCanvas &_canvas, const QFont &_font) : m_canvas { &_canvas }, m_font { _font }
			{}

			template<class Scalar>
			Text<Scalar>::Text (const QFont &_font) : m_font { _font }
			{}

			template<class Scalar>
			const QFont &Text<Scalar>::font () const
			{
				return m_font;
			}

			template<class Scalar>
			const cg3::viewer::GLCanvas *Text<Scalar>::canvas () const
			{
				return m_canvas;
			}

			template<class Scalar>
			const cg3::viewer::GLCanvas *&Text<Scalar>::canvas ()
			{
				return m_canvas;
			}

			template<class Scalar>
			void Text<Scalar>::setFont (const QFont &_font)
			{
				m_font = _font;
				m_fontMetrics = QFontMetrics (m_font);
			}

		}

	}

}

#endif