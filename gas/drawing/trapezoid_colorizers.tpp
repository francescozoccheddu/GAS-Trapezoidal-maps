#pragma once

#include "trapezoid_colorizers.hpp"

#include <stdexcept>

namespace GAS
{

	namespace Drawing
	{

		template<class Scalar>
		bool TrapezoidColorizer<Scalar>::requiresBlending () const
		{
			return false;
		}

		namespace TrapezoidColorizers
		{

			template<class Scalar>
			Color Constant<Scalar>::provideColor (const TrapezoidalMap<Scalar> &/*_trapezoidalMap*/, int /*_index*/, const Trapezoid<Scalar> &/*_trapezoid*/) const
			{
				return m_color;
			}

			template<class Scalar>
			bool Constant<Scalar>::requiresBlending () const
			{
				return m_color.alpha () < 255;
			}

			template<class Scalar>
			Constant<Scalar>::Constant (const Color &_color) : m_color { _color }
			{
				if (!m_color.isValid ())
				{
					throw std::invalid_argument ("Invalid color");
				}
			}

			template<class Scalar>
			const Color &Constant<Scalar>::color () const
			{
				return m_color;
			}

			template<class Scalar>
			void Constant<Scalar>::setColor (const Color &_color)
			{
				if (!_color.isValid ())
				{
					throw std::invalid_argument ("Invalid color");
				}
				m_color = _color;
			}

			template<class Scalar>
			Color Rainbow<Scalar>::provideColor (const TrapezoidalMap<Scalar> &_trapezoidalMap, int _index, const Trapezoid<Scalar> &/*_trapezoid*/) const
			{
				Color color;
				color.setHsvF (_index / (float) _trapezoidalMap.trapezoidsCount (), m_saturation, m_value);
				color.setAlphaF (m_alpha);
				return color;
			}

			template<class Scalar>
			bool Rainbow<Scalar>::requiresBlending () const
			{
				return m_alpha < 1.0f;
			}

			template<class Scalar>
			Rainbow<Scalar>::Rainbow (float _saturation, float _value, float _alpha) : m_saturation { _saturation }, m_value { _value }, m_alpha { _alpha }
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
			float Rainbow<Scalar>::saturation () const
			{
				return m_saturation;
			}

			template<class Scalar>
			void Rainbow<Scalar>::setSaturation (float _saturation)
			{
				if (_saturation < 0.0f || _saturation > 1.0f)
				{
					throw std::invalid_argument ("Saturation must be in range [0,1]");
				}
				m_saturation = _saturation;
			}

			template<class Scalar>
			float Rainbow<Scalar>::value () const
			{
				return m_value;
			}

			template<class Scalar>
			void Rainbow<Scalar>::setValue (float _value)
			{
				if (_value < 0.0f || _value > 1.0f)
				{
					throw std::invalid_argument ("Value must be in range [0,1]");
				}
				m_value = _value;
			}

			template<class Scalar>
			float Rainbow<Scalar>::alpha () const
			{
				return m_alpha;
			}

			template<class Scalar>
			void Rainbow<Scalar>::setAlpha (float _alpha)
			{
				if (_alpha < 0.0f || _alpha > 1.0f)
				{
					throw std::invalid_argument ("Alpha must be in range [0,1]");
				}
				m_alpha = _alpha;
			}


			template<class Scalar>
			Color Selector<Scalar>::provideColor (const TrapezoidalMap<Scalar> &_trapezoidalMap, int _index, const Trapezoid<Scalar> &_trapezoid) const
			{
				if (&_trapezoid == m_selected)
				{
					if (!m_selection)
					{
						throw std::domain_error ("Selection colorizer is null");
					}
					return m_selection->provideColor (_trapezoidalMap, _index, _trapezoid);
				}
				else
				{
					if (!m_normal)
					{
						throw std::domain_error ("Normal colorizer is null");
					}
					return m_normal->provideColor (_trapezoidalMap, _index, _trapezoid);
				}
			}

			template<class Scalar>
			bool Selector<Scalar>::requiresBlending () const
			{
				if (!m_normal)
				{
					throw std::domain_error ("Normal colorizer is null");
				}
				if (m_selected && !m_selection)
				{
					throw std::domain_error ("Selection colorizer is null");
				}
				return m_normal->requiresBlending () || (m_selected && m_selection->requiresBlending ());
			}

			template<class Scalar>
			Selector<Scalar>::Selector (TrapezoidColorizer<Scalar> &_normal, TrapezoidColorizer<Scalar> &_selection) : m_normal { &_normal }, m_selection { &_selection }
			{}

			template<class Scalar>
			const TrapezoidColorizer<Scalar> *Selector<Scalar>::normal () const
			{
				return m_normal;
			}

			template<class Scalar>
			const TrapezoidColorizer<Scalar> *Selector<Scalar>::selection () const
			{
				return m_selection;
			}

			template<class Scalar>
			TrapezoidColorizer<Scalar> *&Selector<Scalar>::normal ()
			{
				return m_normal;
			}

			template<class Scalar>
			TrapezoidColorizer<Scalar> *&Selector<Scalar>::selection ()
			{
				return m_selection;
			}

			template<class Scalar>
			const Trapezoid<Scalar> *Selector<Scalar>::selected () const
			{
				return m_selected;
			}

			template<class Scalar>
			const Trapezoid<Scalar> *&Selector<Scalar>::selected ()
			{
				return m_selected;
			}

		}

	}

}