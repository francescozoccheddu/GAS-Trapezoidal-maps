#ifndef GAS_DRAWING_TRAPEZOID_RENDERERS_IMPL_INCLUDED
#define GAS_DRAWING_TRAPEZOID_RENDERERS_IMPL_INCLUDED

#ifndef GAS_DRAWING_TRAPEZOID_RENDERERS_INCLUDED
#error 'gas/drawing/trapezoid_renderers.tpp' should not be directly included
#endif

#include "trapezoid_renderers.hpp"

#include <stdexcept>
#include <QString>
#include <type_traits>

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

		/// Internal functions not intended for direct usage.
		/// \note
		/// I could have used \c cg3::opengl::draw* functions but they don't support transparency and
		/// they don't allow drawing quads with zero width strokes as glLineWidth(0) is illegal by OpenGL specs.
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
			inline typename std::enable_if<!std::is_integral<Scalar>::value>::type
				glVertex (const Point<Scalar> &_point) { glVertexd (_point); }

			template<class Scalar>
			inline typename std::enable_if<std::is_integral<Scalar>::value>::type
				glVertex (const Point<Scalar> &_point) { glVertexi (_point); }

			inline void glVertex (const Point<float> &_point) { glVertexf (_point); }

		}

		template<class Scalar>
		void TrapezoidRenderer<Scalar>::beforeDraw (const TrapezoidalMap<Scalar> &/*_trapezoidalMap*/) const
		{}

		template<class Scalar>
		void TrapezoidRenderer<Scalar>::afterDraw (const TrapezoidalMap<Scalar> &/*_trapezoidalMap*/) const
		{}

		namespace TrapezoidRenderers
		{

			template<class Scalar, class RenderScalar>
			void Stroke<Scalar, RenderScalar>::beforeDraw (const TrapezoidalMap<Scalar> &/*_trapezoidalMap*/) const
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

			template<class Scalar, class RenderScalar>
			void Stroke<Scalar, RenderScalar>::draw (const TrapezoidalMap<Scalar> &/*_trapezoidalMap*/, int /*_index*/, const Trapezoid<Scalar> &_trapezoid, const Color &_color) const
			{
				if (m_thickness > 0 && _color.alpha () > 0)
				{
					Internals::glColor (_color);
					const Point<RenderScalar>
						&a { _trapezoid.template bottomLeft<RenderScalar> () },
						&b { _trapezoid.template topLeft<RenderScalar> () },
						&c { _trapezoid.template topRight<RenderScalar> () },
						&d { _trapezoid.template bottomRight<RenderScalar> () };
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

			template<class Scalar, class RenderScalar>
			void Stroke<Scalar, RenderScalar>::afterDraw (const TrapezoidalMap<Scalar> &/*_trapezoidalMap*/) const
			{
				if (m_thickness > 0)
				{
					glEnd ();
#ifdef GAS_DRAWING_RESTORE_GL_STATE
					glPopAttrib ();
#endif
				}
			}

			template<class Scalar, class RenderScalar>
			Stroke<Scalar, RenderScalar>::Stroke (int _thickness) : m_thickness { _thickness }
			{
				if (_thickness < 0)
				{
					throw std::invalid_argument ("Thickness cannot be negative");
				}
			}

			template<class Scalar, class RenderScalar>
			int Stroke<Scalar, RenderScalar>::thickness () const
			{
				return m_thickness;
			}

			template<class Scalar, class RenderScalar>
			void Stroke<Scalar, RenderScalar>::setThickness (int _thickness)
			{
				if (_thickness < 0)
				{
					throw std::invalid_argument ("Thickness cannot be negative");
				}
				m_thickness = _thickness;
			}

			template<class Scalar, class RenderScalar>
			void Fill<Scalar, RenderScalar>::beforeDraw (const TrapezoidalMap<Scalar> &/*_trapezoidalMap*/) const
			{
#ifdef GAS_DRAWING_RESTORE_GL_STATE
				glPushAttrib (GL_ENABLE_BIT);
#endif
				glDisable (GL_DEPTH_TEST);
				glDisable (GL_LIGHTING);
				glBegin (GL_QUADS);
			}

			template<class Scalar, class RenderScalar>
			void Fill<Scalar, RenderScalar>::draw (const TrapezoidalMap<Scalar> &/*_trapezoidalMap*/, int /*_index*/, const Trapezoid<Scalar> &_trapezoid, const Color &_color) const
			{
				if (_color.alpha () > 0)
				{
					Internals::glColor (_color);
					Internals::glVertex (_trapezoid.template bottomLeft<RenderScalar> ());
					Internals::glVertex (_trapezoid.template topLeft<RenderScalar> ());
					Internals::glVertex (_trapezoid.template topRight<RenderScalar> ());
					Internals::glVertex (_trapezoid.template bottomRight<RenderScalar> ());
				}
			}

			template<class Scalar, class RenderScalar>
			void Fill<Scalar, RenderScalar>::afterDraw (const TrapezoidalMap<Scalar> &/*_trapezoidalMap*/) const
			{
				glEnd ();
#ifdef GAS_DRAWING_RESTORE_GL_STATE
				glPopAttrib ();
#endif
			}

#ifdef GAS_DRAWING_ENABLE_TRAPEZOID_SERIAL
			template<class Scalar, class RenderScalar>
			QString Text<Scalar, RenderScalar>::getTrapezoidSerial (const Trapezoid<Scalar> *_trapezoid)
			{
				return QString::fromStdString (_trapezoid ? _trapezoid->serial () : GAS::Utils::Serial::null);
			}
#endif

			template<class Scalar, class RenderScalar>
			bool Text<Scalar, RenderScalar>::isRectInsideTrapezoid (const Point<RenderScalar> &_center, RenderScalar _halfWidth, RenderScalar _halfHeight, const Trapezoid<Scalar> &_trapezoid)
			{
				return _trapezoid.contains (_center + Point<RenderScalar>{-_halfWidth, -_halfHeight})
					&& _trapezoid.contains (_center + Point<RenderScalar>{-_halfWidth, _halfHeight})
					&& _trapezoid.contains (_center + Point<RenderScalar>{_halfWidth, _halfHeight})
					&& _trapezoid.contains (_center + Point<RenderScalar>{_halfWidth, -_halfHeight});
			}

			template<class Scalar, class RenderScalar>
			void Text<Scalar, RenderScalar>::drawText (const QString &_text, const Trapezoid<Scalar> &_trapezoid) const
			{
				const QRect projRect { m_fontMetrics.boundingRect(_text) };
				const Point<RenderScalar> centroid { _trapezoid.template centroid<RenderScalar> () };
				const qglviewer::Vec &qrealCentroid { static_cast<qreal>(centroid.x ()), static_cast<qreal>(centroid.y ()), 0 };
				{
					const qreal ratio { m_canvas->camera ()->pixelGLRatio (qrealCentroid) };
					RenderScalar halfWidth { static_cast<RenderScalar>(projRect.width() * ratio / qreal { 2 }) };
					RenderScalar halfHeight { static_cast<RenderScalar>(projRect.height() *ratio / qreal { 2 }) };
					if (!isRectInsideTrapezoid (centroid, halfWidth, halfHeight, _trapezoid))
					{
						return;
					}
				}
				const qglviewer::Vec projCentroid { m_canvas->camera ()->projectedCoordinatesOf (qrealCentroid) };
				m_canvas->drawText (projCentroid.x - projRect.width() / 2, projCentroid.y + projRect.height() / 2, _text, m_font);
			}

			template<class Scalar, class RenderScalar>
			void Text<Scalar, RenderScalar>::beforeDraw (const TrapezoidalMap<Scalar> &/*_trapezoidalMap*/) const
			{
#ifdef GAS_DRAWING_RESTORE_GL_STATE
				glPushAttrib (GL_ENABLE_BIT);
#endif
				glDisable (GL_DEPTH_TEST);
				glDisable (GL_LIGHTING);
			}

			template<class Scalar, class RenderScalar>
			void Text<Scalar, RenderScalar>::draw (const TrapezoidalMap<Scalar> &/*_trapezoidalMap*/, int _index, const Trapezoid<Scalar> &_trapezoid, const Color &_color) const
			{
				if (!m_canvas)
				{
					throw std::domain_error ("Canvas is null");
				}
				if (_color.alpha () > 0)
				{
					Internals::glColor (_color);
					const Point<Scalar> centroid { _trapezoid.centroid () };
#ifdef GAS_DRAWING_ENABLE_TRAPEZOID_SERIAL
					(void)_index;
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

			template<class Scalar, class RenderScalar>
			void Text<Scalar, RenderScalar>::afterDraw (const TrapezoidalMap<Scalar> &/*_trapezoidalMap*/) const
			{
#ifdef GAS_DRAWING_RESTORE_GL_STATE
				glPopAttrib ();
#endif
			}

			template<class Scalar, class RenderScalar>
			Text<Scalar, RenderScalar>::Text (cg3::viewer::GLCanvas &_canvas, const QFont &_font) : m_canvas { &_canvas }, m_font { _font }
			{}

			template<class Scalar, class RenderScalar>
			Text<Scalar, RenderScalar>::Text (const QFont &_font) : m_font { _font }
			{}

			template<class Scalar, class RenderScalar>
			const QFont &Text<Scalar, RenderScalar>::font () const
			{
				return m_font;
			}

			template<class Scalar, class RenderScalar>
			const cg3::viewer::GLCanvas *Text<Scalar, RenderScalar>::canvas () const
			{
				return m_canvas;
			}

			template<class Scalar, class RenderScalar>
			const cg3::viewer::GLCanvas *&Text<Scalar, RenderScalar>::canvas ()
			{
				return m_canvas;
			}

			template<class Scalar, class RenderScalar>
			void Text<Scalar, RenderScalar>::setFont (const QFont &_font)
			{
				m_font = _font;
				m_fontMetrics = QFontMetrics (m_font);
			}

		}

	}

}

#endif