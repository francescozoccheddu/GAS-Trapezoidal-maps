#ifndef GAS_DRAWING_TRAPEZOID_COLORIZERS_INCLUDED
#define GAS_DRAWING_TRAPEZOID_COLORIZERS_INCLUDED

#include <gas/data/trapezoidal_map.hpp>
#include <gas/data/trapezoid.hpp>
#include <gas/drawing/color.hpp>

namespace GAS
{

	namespace Drawing
	{

		template<class Scalar>
		class TrapezoidalMapDrawer;

		namespace TrapezoidColorizers
		{
			template<class Scalar>
			class Selector;
		}

		template<class Scalar>
		class TrapezoidColorizer
		{

			friend class TrapezoidalMapDrawer<Scalar>;
			friend class TrapezoidColorizers::Selector<Scalar>;

		protected:

			virtual Color provideColor (const TrapezoidalMap<Scalar> &trapezoidalMap, int index, const Trapezoid<Scalar> &trapezoid) const = 0;
			virtual bool requiresBlending () const;

		public:

			virtual ~TrapezoidColorizer () = default;

		};

		namespace TrapezoidColorizers
		{

			template<class Scalar>
			class Constant final : public TrapezoidColorizer<Scalar>
			{

				Color m_color;

			protected:

				virtual Color provideColor (const TrapezoidalMap<Scalar> &trapezoidalMap, int index, const Trapezoid<Scalar> &trapezoid) const override final;
				virtual bool requiresBlending () const override final;

			public:

				Constant () = default;
				Constant (const Color &color);

				const Color &color () const;
				void setColor (const Color &color);

			};

			template<class Scalar>
			class Rainbow final : public TrapezoidColorizer<Scalar>
			{

			private:

				float m_saturation { 1.0f };
				float m_value { 1.0f };
				float m_alpha { 1.0f };

			protected:

				virtual Color provideColor (const TrapezoidalMap<Scalar> &trapezoidalMap, int index, const Trapezoid<Scalar> &trapezoid) const override final;
				virtual bool requiresBlending () const override final;

			public:

				Rainbow () = default;
				Rainbow (float saturation, float value, float alpha = 1.0f);

				float saturation () const;
				float value () const;
				float alpha () const;

				void setSaturation (float saturation);
				void setValue (float saturation);
				void setAlpha (float saturation);

			};

			template<class Scalar>
			class Selector final : public TrapezoidColorizer<Scalar>
			{

			private:

				TrapezoidColorizer *m_normal {}, *m_selection {};
				const Trapezoid<Scalar> *m_selected {};

			protected:

				virtual Color provideColor (const TrapezoidalMap<Scalar> &trapezoidalMap, int index, const Trapezoid<Scalar> &trapezoid) const override final;
				virtual bool requiresBlending () const override final;

			public:

				Selector () = default;
				Selector (TrapezoidColorizer &normal, TrapezoidColorizer &selection);

				const TrapezoidColorizer *normal () const;
				const TrapezoidColorizer *selection () const;
				TrapezoidColorizer *&normal ();
				TrapezoidColorizer *&selection ();

				const Trapezoid<Scalar> *selected () const;
				const Trapezoid<Scalar> *&selected ();

			};

		}

	}

}

#include "trapezoid_colorizers.tpp"

#endif