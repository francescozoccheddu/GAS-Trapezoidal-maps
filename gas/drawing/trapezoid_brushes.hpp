#pragma once

#include <gas/data/trapezoidal_map.hpp>
#include <gas/data/trapezoid.hpp>
#include <cg3/viewer/glcanvas.h>
#include <gas/drawing/color.hpp>

#define GAS_DRAWING_RESTORE_GL_STATE

namespace GAS
{

	namespace Drawing
	{

		template<class Scalar>
		class TrapezoidalMapDrawer;

		template<class Scalar>
		class TrapezoidBrush
		{

			friend class TrapezoidalMapDrawer<Scalar>;

		protected:

			virtual void beforeDraw (const TrapezoidalMap<Scalar> &trapezoidalMap) const;
			virtual void draw (const TrapezoidalMap<Scalar> &trapezoidalMap, int index, const Trapezoid<Scalar> &trapezoid, const Color &color) const = 0;
			virtual void afterDraw (const TrapezoidalMap<Scalar> &trapezoidalMap) const;

		public:

			virtual ~TrapezoidBrush () = default;

		};

		namespace TrapezoidBrushes
		{

			template<class Scalar>
			class Stroke final : public TrapezoidBrush<Scalar>
			{

			private:

				int m_thickness { 1 };

			protected:

				virtual void beforeDraw (const TrapezoidalMap<Scalar> &trapezoidalMap) const override final;
				virtual void draw (const TrapezoidalMap<Scalar> &trapezoidalMap, int index, const Trapezoid<Scalar> &trapezoid, const Color &color) const override final;
				virtual void afterDraw (const TrapezoidalMap<Scalar> &trapezoidalMap) const override final;

			public:

				Stroke () = default;
				Stroke (int thickness);

				int thickness () const;
				void setThickness (int thickness);

			};

			template<class Scalar>
			class Fill final : public TrapezoidBrush<Scalar>
			{

			protected:

				virtual void beforeDraw (const TrapezoidalMap<Scalar> &trapezoidalMap) const override final;
				virtual void draw (const TrapezoidalMap<Scalar> &trapezoidalMap, int index, const Trapezoid<Scalar> &trapezoid, const Color &color) const override final;
				virtual void afterDraw (const TrapezoidalMap<Scalar> &trapezoidalMap) const override final;

			};

			template<class Scalar>
			class Text final : public TrapezoidBrush<Scalar>
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

				Text () = default;
				Text (cg3::viewer::GLCanvas &canvas, const QFont &font = {});
				Text (const QFont &font);

				const QFont &font () const;
				const cg3::viewer::GLCanvas *canvas () const;

				const cg3::viewer::GLCanvas *&canvas ();

				void setFont (const QFont &font);


			};

		}

	}

}

#include "trapezoid_brushes.tpp"