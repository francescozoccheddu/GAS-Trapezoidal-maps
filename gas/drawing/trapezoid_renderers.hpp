/// Trapezoid renderer interface and implementations.
/// \file
/// \author Francesco Zoccheddu

#ifndef GAS_DRAWING_TRAPEZOID_RENDERERS_INCLUDED
#define GAS_DRAWING_TRAPEZOID_RENDERERS_INCLUDED

#include <gas/data/trapezoidal_map.hpp>
#include <gas/data/trapezoid.hpp>
#include <cg3/viewer/glcanvas.h>
#include <gas/drawing/color.hpp>

/// Backup and restore the OpenGL server state after drawing.
/// If defined, the OpenGL server state is left untouched after drawing is finished.
#define GAS_DRAWING_RESTORE_GL_STATE

namespace GAS
{

	namespace Drawing
	{

		template<class Scalar>
		class TrapezoidalMapDrawer;

		/// Component for rendering trapezoids through TrapezoidalMapDrawer.
		/// \tparam Scalar
		/// The scalar type for TrapezoidalMap.
		template<class Scalar>
		class TrapezoidRenderer
		{

			friend class TrapezoidalMapDrawer<Scalar>;

		protected:

			/// Method called before start drawing to allow eventual setup operations.
			/// \param[in] trapezoidalMap
			/// The trapezoidal map that will be drawn.
			virtual void beforeDraw (const TrapezoidalMap<Scalar> &trapezoidalMap) const;

			/// Render a single trapezoid.
			/// \param[in] trapezoidalMap
			/// The map to which \p trapezoid belongs.
			/// \param[in] index
			/// The zero-based index of \p trapezoid in the map.
			/// \param[in] trapezoid
			/// The trapezoid to draw.
			/// \param[in] color
			/// The color to use for drawing the trapezoid.
			virtual void draw (const TrapezoidalMap<Scalar> &trapezoidalMap, int index, const Trapezoid<Scalar> &trapezoid, const Color &color) const = 0;

			/// Method called after drawing is finished to allow eventual cleanup operations.
			/// \param[in] trapezoidalMap
			/// The trapezoidal map that has been drawn.
			virtual void afterDraw (const TrapezoidalMap<Scalar> &trapezoidalMap) const;

		public:

			virtual ~TrapezoidRenderer () = default;

		};

		/// TrapezoidRenderer implementations.
		namespace TrapezoidRenderers
		{

			/// Renderer that draws the edge lines of the trapezoids.
			/// \tparam Scalar
			/// The scalar type for TrapezoidalMap.
			template<class Scalar>
			class Stroke final : public TrapezoidRenderer<Scalar>
			{

			private:

				int m_thickness { 1 };

			protected:

				virtual void beforeDraw (const TrapezoidalMap<Scalar> &trapezoidalMap) const override final;
				virtual void draw (const TrapezoidalMap<Scalar> &trapezoidalMap, int index, const Trapezoid<Scalar> &trapezoid, const Color &color) const override final;
				virtual void afterDraw (const TrapezoidalMap<Scalar> &trapezoidalMap) const override final;

			public:

				/// Construct a renderer with the default thickness.
				Stroke () = default;

				/// Construct a renderer with the specified thickness.
				/// \param[in] thickness
				/// The stroke thickness in pixels.
				/// \exception std::invalid_argument
				/// If thickness is negative.
				Stroke (int thickness);

				/// \return
				/// The stroke thickness in pixels.
				int thickness () const;

				/// Set the stroke thickness.
				/// \param[in] thickness
				/// The stroke thickness in pixels.
				/// \exception std::invalid_argument
				/// If thickness is negative.
				void setThickness (int thickness);

			};

			/// Renderer that fills the interior of the trapezoids.
			/// \tparam Scalar
			/// The scalar type for TrapezoidalMap.
			template<class Scalar>
			class Fill final : public TrapezoidRenderer<Scalar>
			{

			protected:

				virtual void beforeDraw (const TrapezoidalMap<Scalar> &trapezoidalMap) const override final;
				virtual void draw (const TrapezoidalMap<Scalar> &trapezoidalMap, int index, const Trapezoid<Scalar> &trapezoid, const Color &color) const override final;
				virtual void afterDraw (const TrapezoidalMap<Scalar> &trapezoidalMap) const override final;

			};

			/// Renderer that draws an unique number inside each trapezoid.
			/// \tparam Scalar
			/// The scalar type for TrapezoidalMap.
			template<class Scalar>
			class Text final : public TrapezoidRenderer<Scalar>
			{

			private:

				cg3::viewer::GLCanvas *m_canvas;
				QFont m_font {};
				QFontMetrics m_fontMetrics { m_font };

#ifdef GAS_DRAWING_ENABLE_TRAPEZOID_SERIAL
				static QString getTrapezoidSerial (const Trapezoid<Scalar> *trapezoid);
#endif

				static bool isRectInsideTrapezoid (const Point<Scalar> &center, Scalar halfWidth, Scalar halfHeight, const Trapezoid<Scalar> &trapezoid);

				void drawText (const QString &text, const Trapezoid<Scalar> &trapezoid) const;

			protected:

				virtual void beforeDraw (const TrapezoidalMap<Scalar> &trapezoidalMap) const override final;
				virtual void draw (const TrapezoidalMap<Scalar> &trapezoidalMap, int index, const Trapezoid<Scalar> &trapezoid, const Color &color) const override final;
				virtual void afterDraw (const TrapezoidalMap<Scalar> &trapezoidalMap) const override final;

			public:

				/// Construct a text renderer with the default font.
				/// \remark
				/// The canvas must be set before start drawing.
				Text () = default;

				/// Construct a text renderer with the specified canvas and font.
				/// \param[in] canvas
				/// The canvas for drawing text.
				/// \param[in] font
				/// The text font to use.
				/// \remark
				/// The text drawing flag on the canvas must be manually set before start drawing.
				Text (cg3::viewer::GLCanvas &canvas, const QFont &font = {});

				/// Construct a text renderer with the specified font.
				/// \param[in] font
				/// The text font to use.
				/// \remark
				/// The canvas must be set before start drawing.
				Text (const QFont &font);

				/// \return
				/// The text font to use.
				const QFont &font () const;

				/// \return
				/// The canvas for drawing text.
				const cg3::viewer::GLCanvas *canvas () const;

				/// \remark
				/// The text drawing flag on the canvas must be manually set before start drawing.
				/// \return
				/// The canvas for drawing text.
				const cg3::viewer::GLCanvas *&canvas ();

				/// Set the text font.
				/// \param[in] font
				/// The text font to use.
				void setFont (const QFont &font);

			};

		}

	}

}

#include "trapezoid_renderers.tpp"

#endif