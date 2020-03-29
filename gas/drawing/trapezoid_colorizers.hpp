/// GAS::Drawing::TrapezoidColorizer interface and implementations.
/// \file
/// \author Francesco Zoccheddu

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

		/// Component for providing drawing colors when drawing trapezoidal maps through TrapezoidalMapDrawer.
		/// \tparam Scalar
		/// The scalar type.
		template<class Scalar>
		class TrapezoidColorizer
		{

			friend class TrapezoidalMapDrawer<Scalar>;
			friend class TrapezoidColorizers::Selector<Scalar>;

		protected:

			/// Provide the color for drawing the trapezoid \p trapezoid.
			/// \param[in] trapezoidalMap
			/// The map to which \p trapezoid belongs.
			/// \param[in] index
			/// The zero-based index of \p trapezoid in the map.
			/// \param[in] trapezoid
			/// The trapezoid for which the color should be provided.
			/// \return
			/// The color for drawing \p trapezoid.
			virtual Color provideColor (const TrapezoidalMap<Scalar> &trapezoidalMap, int index, const Trapezoid<Scalar> &trapezoid) const = 0;

			/// Check if the colorizer requires color blending to be enabled.
			/// The default implementation always returns \c false.
			/// \return
			/// \c true if blending is required, \c false otherwise.
			virtual bool requiresBlending () const;

		public:

			virtual ~TrapezoidColorizer () = default;

		};

		/// TrapezoidColorizer implementations.
		namespace TrapezoidColorizers
		{

			/// Colorizer that provides the same constant color for all the trapezoids.
			/// \tparam Scalar
			/// The scalar type.
			template<class Scalar>
			class Constant final : public TrapezoidColorizer<Scalar>
			{

				Color m_color;

			protected:

				virtual Color provideColor (const TrapezoidalMap<Scalar> &trapezoidalMap, int index, const Trapezoid<Scalar> &trapezoid) const override final;
				virtual bool requiresBlending () const override final;

			public:

				/// Construct a black colorizer.
				Constant () = default;

				/// Construct a colorizer with \p color.
				/// \param[in] color
				/// The color.
				/// \exception std::invalid_argument
				/// If \p color is not a valid RGBA8888 color.
				Constant (const Color &color);

				/// \return
				/// The color.
				const Color &color () const;

				/// Set the color.
				/// \param[in] color
				/// The color.
				/// \exception std::invalid_argument
				/// If \p color is not a valid RGBA8888 color.
				void setColor (const Color &color);

			};

			/// Colorizer that distributes the hue color scale along all trapezoids in a map.
			/// \tparam Scalar
			/// The scalar type.
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

				/// Construct a colorizer with maximum saturation, value and alpha values.
				Rainbow () = default;

				/// Construct a colorizer with the specified (H)SBA parameters.
				/// \param[in] saturation
				/// The HSB saturation in range [0,1].
				/// \param[in] value
				/// The HSB value in range [0,1].
				/// \param[in] alpha
				/// The opacity in range [0,1].
				/// \exception std::invalid_argument
				/// If at least one argument does not fall inside the range [0,1].
				Rainbow (float saturation, float value, float alpha = 1.0f);

				/// \return
				/// The HSB saturation in range [0,1].
				float saturation () const;

				/// \return
				/// The HSB value in range [0,1].
				float value () const;

				/// \return
				/// The opacity in range [0,1].
				float alpha () const;

				/// Set the HSB saturation.
				/// \param[in] saturation
				/// The HSB saturation in range [0,1].
				/// \exception std::invalid_argument
				/// If \p saturation does not fall inside the range [0,1].
				void setSaturation (float saturation);

				/// Set the HSB value.
				/// \param[in] value
				/// The HSB value in range [0,1].
				/// \exception std::invalid_argument
				/// If \p value does not fall inside the range [0,1].
				void setValue (float value);

				/// Set the opacity.
				/// \param[in] alpha
				/// The opacity in range [0,1].
				/// \exception std::invalid_argument
				/// If \p alpha does not fall inside the range [0,1].
				void setAlpha (float alpha);

			};

			/// Colorizer selector that chooses the right colorizer depending on whether the trapezoid to draw is selected.
			/// \tparam Scalar
			/// The scalar type.
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

				/// Construct an empty selector.
				/// \remark
				/// The delegated colorizers must be set before start drawing.
				Selector () = default;


				/// Construct a selector with the specified colorizers.
				/// \param[in] normal
				/// The colorizer to use for unselected trapezoids.
				/// \param[in] selection
				/// The colorizer to use for selected trapezoids.
				Selector (TrapezoidColorizer &normal, TrapezoidColorizer &selection);

				/// \return
				/// The colorizer used for unselected trapezoids.
				const TrapezoidColorizer *normal () const;

				/// \return
				/// The colorizer used for selected trapezoids.
				const TrapezoidColorizer *selection () const;

				/// \copydoc normal
				TrapezoidColorizer *&normal ();

				/// \copydoc selection
				TrapezoidColorizer *&selection ();

				/// \return
				/// The selected trapezoid or \c nullptr if no trapezoid is selected.
				const Trapezoid<Scalar> *selected () const;

				/// \copydoc selected
				const Trapezoid<Scalar> *&selected ();

			};

		}

	}

}

#include "trapezoid_colorizers.tpp"

#endif