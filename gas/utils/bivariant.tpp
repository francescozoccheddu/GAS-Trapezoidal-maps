#ifndef GAS_UTILS_BIVARIANT_IMPL_INCLUDED
#define GAS_UTILS_BIVARIANT_IMPL_INCLUDED

#ifndef GAS_UTILS_BIVARIANT_INCLUDED
#error 'gas/utils/bivariant.tpp' should not be directly included
#endif

#include "bivariant.hpp"

#include <gas/utils/parent_from_member.hpp>
#include <cassert>
#include <utility>
#include <new>

namespace GAS
{

	namespace Utils
	{
		template<class First, class Second>
		BiVariant<First, Second>::Union::Union () : monostate {}
		{}

		template<class First, class Second>
		BiVariant<First, Second>::Union::Union (const First &_copy) : first { _copy }
		{}

		template<class First, class Second>
		BiVariant<First, Second>::Union::Union (First &&_moved) : first { std::move (_moved) }
		{}

		template<class First, class Second>
		BiVariant<First, Second>::Union::Union (const Second &_copy) : second { _copy }
		{}

		template<class First, class Second>
		BiVariant<First, Second>::Union::Union (Second &&_moved) : second { std::move (_moved) }
		{}

		template<class First, class Second>
		BiVariant<First, Second>::Union::~Union ()
		{}

		template<class First, class Second>
		void BiVariant<First, Second>::destroy ()
		{
			switch (m_type)
			{
				case EType::EFirst:
					m_data.first.~First ();
					break;
				case EType::ESecond:
					m_data.second.~Second ();
					break;
			}
			m_type = EType::EDestroyed;
		}

		template<class First, class Second>
		const BiVariant<First, Second> &BiVariant<First, Second>::from (const First &_first)
		{
			return GAS_UTILS_PARENT_FROM_MEMBER (_first, BiVariant, m_data);
		}

		template<class First, class Second>
		const BiVariant<First, Second> &BiVariant<First, Second>::from (const Second &_second)
		{
			return GAS_UTILS_PARENT_FROM_MEMBER (_second, BiVariant, m_data);
		}

		template<class First, class Second>
		BiVariant<First, Second> &BiVariant<First, Second>::from (First &_first)
		{
			return GAS_UTILS_PARENT_FROM_MEMBER (_first, BiVariant, m_data);
		}

		template<class First, class Second>
		BiVariant<First, Second> &BiVariant<First, Second>::from (Second &_second)
		{
			return GAS_UTILS_PARENT_FROM_MEMBER (_second, BiVariant, m_data);
		}

		template<class First, class Second>
		BiVariant<First, Second>::BiVariant (const First &_copy) : m_data { _copy }, m_type { EType::EFirst }
		{}

		template<class First, class Second>
		BiVariant<First, Second>::BiVariant (First &&_moved) : m_data { std::move (_moved) }, m_type { EType::EFirst }
		{}

		template<class First, class Second>
		BiVariant<First, Second>::BiVariant (const Second &_copy) : m_data { _copy }, m_type { EType::ESecond }
		{}

		template<class First, class Second>
		BiVariant<First, Second>::BiVariant (Second &&_moved) : m_data { std::move (_moved) }, m_type { EType::ESecond }
		{}

		template<class First, class Second>
		BiVariant<First, Second>::BiVariant (const BiVariant &_copy)
		{
			*this = _copy;
		}
		template<class First, class Second>
		BiVariant<First, Second>::BiVariant (BiVariant &&_moved)
		{
			*this = std::move (_moved);
		}

		template<class First, class Second>
		BiVariant<First, Second>::~BiVariant ()
		{
			destroy ();
		}

		template<class First, class Second>
		BiVariant<First, Second> &BiVariant<First, Second>::operator=(const BiVariant &_copy)
		{
			switch (_copy.m_type)
			{
				case EType::EFirst:
					set (_copy.m_data.first);
					break;
				case EType::ESecond:
					set (_copy.m_data.second);
					break;
				default:
					assert (false);
			}
			return *this;
		}

		template<class First, class Second>
		BiVariant<First, Second> &BiVariant<First, Second>::operator=(BiVariant &&_moved)
		{
			switch (_moved.m_type)
			{
				case EType::EFirst:
					set (std::move (_moved.m_data.first));
					break;
				case EType::ESecond:
					set (std::move (_moved.m_data.second));
					break;
				default:
					assert (false);
			}
			_moved.destroy ();
			return *this;
		}

		template<class First, class Second>
		BiVariant<First, Second> &BiVariant<First, Second>::operator=(const First &_copy)
		{
			set (_copy);
			return *this;
		}

		template<class First, class Second>
		BiVariant<First, Second> &BiVariant<First, Second>::operator=(First &&_moved)
		{
			set (std::move (_moved));
			return *this;
		}

		template<class First, class Second>
		BiVariant<First, Second> &BiVariant<First, Second>::operator=(const Second &_copy)
		{
			set (_copy);
			return *this;
		}

		template<class First, class Second>
		BiVariant<First, Second> &BiVariant<First, Second>::operator=(Second &&_moved)
		{
			set (std::move (_moved));
			return *this;
		}

		template<class First, class Second>
		bool BiVariant<First, Second>::operator==(const BiVariant &_other) const
		{
			if (m_type == _other.m_type)
			{
				switch (m_type)
				{
					case EType::EFirst:
						return m_data.first == _other.m_data.first;
					case EType::ESecond:
						return m_data.second == _other.m_data.second;
				}
			}
			return false;
		}

		template<class First, class Second>
		bool BiVariant<First, Second>::operator!=(const BiVariant &_other) const
		{
			return !(*this == _other);
		}

		template<class First, class Second>
		bool BiVariant<First, Second>::isFirstType () const
		{
			return m_type == EType::EFirst;
		}

		template<class First, class Second>
		bool BiVariant<First, Second>::isSecondType () const
		{
			return m_type == EType::ESecond;
		}

		template<class First, class Second>
		void BiVariant<First, Second>::set (const First &_copy)
		{
			destroy ();
			new (&m_data.first) First { _copy };
			m_type = EType::EFirst;
		}

		template<class First, class Second>
		void BiVariant<First, Second>::set (First &&_moved)
		{
			destroy ();
			new (&m_data.first) First { std::move (_moved) };
			m_type = EType::EFirst;
		}

		template<class First, class Second>
		void BiVariant<First, Second>::set (const Second &_copy)
		{
			destroy ();
			new (&m_data.second) Second { _copy };
			m_type = EType::ESecond;
		}

		template<class First, class Second>
		void BiVariant<First, Second>::set (Second &&_moved)
		{
			destroy ();
			new (&m_data.second) Second { std::move (_moved) };
			m_type = EType::ESecond;
		}

		template<class First, class Second>
		const First &BiVariant<First, Second>::first () const
		{
			assert (isFirstType ());
			return m_data.first;
		}

		template<class First, class Second>
		const Second &BiVariant<First, Second>::second () const
		{
			assert (isSecondType ());
			return m_data.second;
		}

		template<class First, class Second>
		First &BiVariant<First, Second>::first ()
		{
			assert (isFirstType ());
			return m_data.first;
		}

		template<class First, class Second>
		Second &BiVariant<First, Second>::second ()
		{
			assert (isSecondType ());
			return m_data.second;
		}

	}

}

#endif