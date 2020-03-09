#pragma once

#include <data_structures/trapezoidal_map.hpp>
#include <data_structures/trapezoid.hpp>
#include <cg3/utilities/color.h>
#include <cg3/viewer/interfaces/drawable_object.h>
#include <cg3/geometry/point3.h>
#include <cg3/viewer/glcanvas.h>

namespace GAS
{

	namespace Drawing
	{

		template<class Scalar>
		class TrapezoidalMapDrawer : public cg3::DrawableObject
		{

		public:

			class Colorizer
			{

			protected:

				friend void TrapezoidalMapDrawer::draw () const;

				// Pure virtual methods
				virtual const cg3::Color &provideColor (const TrapezoidalMap<Scalar> &trapezoidalMap, int index, const Trapezoid<Scalar> &trapezoid) const = 0;

			public:

				virtual ~Colorizer () = default;

			};

			class Painter
			{

			protected:

				friend void TrapezoidalMapDrawer::draw () const;

				// Pure virtual methods
				virtual void draw (const TrapezoidalMap<Scalar> &trapezoidalMap, int index, const Trapezoid<Scalar> &trapezoid, const cg3::Color &color) const = 0;

			public:

				virtual ~Painter () = default;

			};

		private:

			const TrapezoidalMap<Scalar> *m_trapezoidalMap;
			Colorizer *m_colorizer;
			Painter *m_painter;

		protected:

			// cg3::DrawableObject
			virtual void draw () const override final;
			virtual cg3::Point3d sceneCenter () const override final;
			virtual double sceneRadius () const override final;

			void ensureValid () const;
			void ensureValidMap () const;

		public:

			virtual ~TrapezoidalMapDrawer () = default;

			const TrapezoidalMap<Scalar> *getTrapezoidalMap () const;
			void setTrapezoidalMap (const TrapezoidalMap<Scalar> *trapezoidalMap);
			const Colorizer *getColorizer () const;
			Colorizer *getColorizer ();
			void setColorizer (Colorizer *colorizer);
			const Painter *getPainter () const;
			Painter *getPainter ();
			void setPainter (Painter *painter);

		};


		// Colorizers

		template<class Scalar>
		class TrapezoidConstantColorizer final : public TrapezoidalMapDrawer<Scalar>::Colorizer
		{

		protected:

			// TrapezoidalMapDrawer::Colorizer
			virtual const cg3::Color &provideColor (const TrapezoidalMap<Scalar> &trapezoidalMap, int index, const Trapezoid<Scalar> &trapezoid) const override final;

		public:

			cg3::Color color;

			TrapezoidConstantColorizer () = default;
			TrapezoidConstantColorizer (const cg3::Color &color);

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
			virtual const cg3::Color &provideColor (const TrapezoidalMap<Scalar> &trapezoidalMap, int index, const Trapezoid<Scalar> &trapezoid) const override final;

		public:

			TrapezoidFancyColorizer () = default;
			TrapezoidFancyColorizer (float saturation, float value, float alpha = 1.0f);

			float getSaturation () const;
			void setSaturation (float saturation);
			float getValue () const;
			void setValue (float value);
			float getAlpha () const;
			void setAlpha (float alpha);

		};


		// Painters

		template<class Scalar>
		class TrapezoidStrokePainter final : public TrapezoidalMapDrawer<Scalar>::Painter
		{

		private:

			int m_thickness { 1 };

		protected:

			// TrapezoidalMapDrawer::Painter
			virtual void draw (const TrapezoidalMap<Scalar> &trapezoidalMap, int index, const Trapezoid<Scalar> &trapezoid, const cg3::Color &color) const override final;

		public:

			TrapezoidStrokePainter () = default;
			TrapezoidStrokePainter (int thickness);

			int getThickness () const;
			void setThickness (int thickness);

		};

		template<class Scalar>
		class TrapezoidFillPainter final : public TrapezoidalMapDrawer<Scalar>::Painter
		{

		protected:

			// TrapezoidalMapDrawer::Painter
			virtual void draw (const TrapezoidalMap<Scalar> &trapezoidalMap, int index, const Trapezoid<Scalar> &trapezoid, const cg3::Color &color) const override final;

		};

		template<class Scalar>
		class TrapezoidTextPainter final : public TrapezoidalMapDrawer<Scalar>::Painter
		{

		private:

			cg3::viewer::GLCanvas &m_canvas;
			QFont m_font;
			QFontMetrics m_fontMetrics;

		protected:

			// TrapezoidalMapDrawer::Painter
			virtual void draw (const TrapezoidalMap<Scalar> &trapezoidalMap, int index, const Trapezoid<Scalar> &trapezoid, const cg3::Color &color) const override final;

		public:

			TrapezoidTextPainter (cg3::viewer::GLCanvas &_canvas, const QFont &font = { "Times", 10 });

			const QFont &getFont () const;
			void setFont (const QFont &font);

		};

	}

}

#include "trapezoidal_map_drawer.tpp"
