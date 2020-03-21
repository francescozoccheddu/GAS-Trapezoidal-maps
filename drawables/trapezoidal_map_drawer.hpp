#pragma once

#include <data_structures/trapezoidal_map.hpp>
#include <data_structures/trapezoid.hpp>
#include <cg3/utilities/color.h>
#include <cg3/viewer/interfaces/drawable_object.h>
#include <cg3/geometry/point3.h>
#include <cg3/viewer/glcanvas.h>

#ifdef WIN32
#include "windows.h"
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#define GAS_DRAWING_RESTORE_GL_STATE

/*
	I could have used cg3::opengl::draw* functions but they don't support transparency and
	they don't allow drawing quads with zero width strokes as glLineWidth(0) is illegal by OpenGL specs.
*/

namespace GAS
{

	namespace Drawing
	{

		using Color = cg3::Color;

		template<class Scalar>
		class TrapezoidalMapDrawer : public cg3::DrawableObject
		{

		public:

			class Colorizer
			{

			protected:

				friend void TrapezoidalMapDrawer::draw () const;

				// Interface methods
				virtual const Color &provideColor (const TrapezoidalMap<Scalar> &trapezoidalMap, int index, const Trapezoid<Scalar> &trapezoid) const = 0;
				virtual bool requiresBlending () const;

			public:

				virtual ~Colorizer () = default;

			};

			class Painter
			{

			protected:

				friend void TrapezoidalMapDrawer::draw () const;

				// Interface methods
				virtual void beforeDraw (const TrapezoidalMap<Scalar> &trapezoidalMap) const;
				virtual void draw (const TrapezoidalMap<Scalar> &trapezoidalMap, int index, const Trapezoid<Scalar> &trapezoid, const Color &color) const = 0;
				virtual void afterDraw (const TrapezoidalMap<Scalar> &trapezoidalMap) const;

			public:

				virtual ~Painter () = default;

			};

		private:

			const TrapezoidalMap<Scalar> *m_trapezoidalMap {};
			Colorizer *m_colorizer {};
			Painter *m_painter {};

		protected:

			// cg3::DrawableObject
			virtual void draw () const override final;
			virtual cg3::Point3d sceneCenter () const override final;
			virtual double sceneRadius () const override final;

			void ensureValid () const;
			void ensureValidMap () const;

		public:

			virtual ~TrapezoidalMapDrawer () = default;

			const TrapezoidalMap<Scalar> *trapezoidalMap () const;
			void setTrapezoidalMap (const TrapezoidalMap<Scalar> *trapezoidalMap);
			const Colorizer *colorizer () const;
			Colorizer *colorizer ();
			void setColorizer (Colorizer *colorizer);
			const Painter *painter () const;
			Painter *painter ();
			void setPainter (Painter *painter);

		};


		// Colorizers

		template<class Scalar>
		class TrapezoidConstantColorizer final : public TrapezoidalMapDrawer<Scalar>::Colorizer
		{

			Color m_color;

		protected:

			// TrapezoidalMapDrawer::Colorizer
			virtual const Color &provideColor (const TrapezoidalMap<Scalar> &trapezoidalMap, int index, const Trapezoid<Scalar> &trapezoid) const override final;
			virtual bool requiresBlending () const override final;

		public:

			TrapezoidConstantColorizer () = default;
			TrapezoidConstantColorizer (const Color &color);

			const Color &color () const;
			void setColor (const Color &color);

		};

		template<class Scalar>
		class TrapezoidFancyColorizer final : public TrapezoidalMapDrawer<Scalar>::Colorizer
		{

		private:

			float m_saturation { 1.0f };
			float m_value { 1.0f };
			float m_alpha { 1.0f };

		protected:

			// TrapezoidalMapDrawer::Colorizer
			virtual const Color &provideColor (const TrapezoidalMap<Scalar> &trapezoidalMap, int index, const Trapezoid<Scalar> &trapezoid) const override final;
			virtual bool requiresBlending () const override final;

		public:

			TrapezoidFancyColorizer () = default;
			TrapezoidFancyColorizer (float saturation, float value, float alpha = 1.0f);

			float saturation () const;
			float value () const;
			float alpha () const;

			void setSaturation (float saturation);
			void setValue (float saturation);
			void setAlpha (float saturation);

		};


		// Painters

		template<class Scalar>
		class TrapezoidStrokePainter final : public TrapezoidalMapDrawer<Scalar>::Painter
		{

		private:

			int m_thickness { 1 };

		protected:

			// TrapezoidalMapDrawer::Painter
			virtual void beforeDraw (const TrapezoidalMap<Scalar> &trapezoidalMap) const override final;
			virtual void draw (const TrapezoidalMap<Scalar> &trapezoidalMap, int index, const Trapezoid<Scalar> &trapezoid, const Color &color) const override final;
			virtual void afterDraw (const TrapezoidalMap<Scalar> &trapezoidalMap) const override final;

		public:

			TrapezoidStrokePainter () = default;
			TrapezoidStrokePainter (int thickness);

			int thickness () const;
			void setThickness (int thickness);

		};

		template<class Scalar>
		class TrapezoidFillPainter final : public TrapezoidalMapDrawer<Scalar>::Painter
		{

		protected:

			// TrapezoidalMapDrawer::Painter
			virtual void beforeDraw (const TrapezoidalMap<Scalar> &trapezoidalMap) const override final;
			virtual void draw (const TrapezoidalMap<Scalar> &trapezoidalMap, int index, const Trapezoid<Scalar> &trapezoid, const Color &color) const override final;
			virtual void afterDraw (const TrapezoidalMap<Scalar> &trapezoidalMap) const override final;

		};

		template<class Scalar>
		class TrapezoidTextPainter final : public TrapezoidalMapDrawer<Scalar>::Painter
		{

		private:

			cg3::viewer::GLCanvas *m_canvas;
			QFont m_font;
			QFontMetrics m_fontMetrics;

#ifdef GAS_DRAWING_ENABLE_TRAPEZOID_SERIAL
			static QString getTrapezoidSerial (const Trapezoid<Scalar> *trapezoid);
#endif

			static bool isRectInsideTrapezoid (const Point<Scalar> &center, Scalar halfWidth, Scalar halfHeight, const Trapezoid<Scalar> &trapezoid);

			void drawText (const QString &text, const Trapezoid<Scalar> &trapezoid) const;

		protected:

			// TrapezoidalMapDrawer::Painter
			virtual void beforeDraw (const TrapezoidalMap<Scalar> &trapezoidalMap) const override final;
			virtual void draw (const TrapezoidalMap<Scalar> &trapezoidalMap, int index, const Trapezoid<Scalar> &trapezoid, const Color &color) const override final;
			virtual void afterDraw (const TrapezoidalMap<Scalar> &trapezoidalMap) const override final;

		public:

			TrapezoidTextPainter (cg3::viewer::GLCanvas &canvas, const QFont &font = {});

			const QFont &font () const;
			void setFont (const QFont &font);

		};

	}

}

#include "trapezoidal_map_drawer.tpp"
