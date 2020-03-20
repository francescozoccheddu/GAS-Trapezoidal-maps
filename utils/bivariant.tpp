#pragma once

#include "bivariant.hpp"
#include <utils/parent_from_member.hpp>
#include <cassert>
#include <utility>
#include <new>

namespace GAS
{

	namespace Utils
	{

		template<class TypeA, class TypeB>
		BiVariant<TypeA, TypeB>::Union::Union ()
		{}

		template<class TypeA, class TypeB>
		BiVariant<TypeA, TypeB>::Union::Union (const TypeA &_copy) : a { _copy }
		{}

		template<class TypeA, class TypeB>
		BiVariant<TypeA, TypeB>::Union::Union (TypeA &&_moved) : a { std::move (_moved) }
		{}

		template<class TypeA, class TypeB>
		BiVariant<TypeA, TypeB>::Union::Union (const TypeB &_copy) : b { _copy }
		{}

		template<class TypeA, class TypeB>
		BiVariant<TypeA, TypeB>::Union::Union (TypeB &&_moved) : b { std::move (_moved) }
		{}

		template<class TypeA, class TypeB>
		BiVariant<TypeA, TypeB>::Union::~Union ()
		{}

		template<class TypeA, class TypeB>
		void BiVariant<TypeA, TypeB>::destroy ()
		{
			switch (m_type)
			{
				case EType::EFirst:
					m_data.a.~TypeA ();
					break;
				case EType::ESecond:
					m_data.b.~TypeB ();
					break;
			}
			m_type = EType::EDestroyed;
		}

		template<class TypeA, class TypeB>
		const BiVariant<TypeA, TypeB> &BiVariant<TypeA, TypeB>::from (const TypeA &_copy)
		{
			return GAS_UTILS_PARENT_FROM_MEMBER (_copy, BiVariant, m_data);
		}

		template<class TypeA, class TypeB>
		const BiVariant<TypeA, TypeB> &BiVariant<TypeA, TypeB>::from (const TypeB &_copy)
		{
			return GAS_UTILS_PARENT_FROM_MEMBER (_copy, BiVariant, m_data);
		}

		template<class TypeA, class TypeB>
		BiVariant<TypeA, TypeB> &BiVariant<TypeA, TypeB>::from (TypeA &_copy)
		{
			return GAS_UTILS_PARENT_FROM_MEMBER (_copy, BiVariant, m_data);
		}

		template<class TypeA, class TypeB>
		BiVariant<TypeA, TypeB> &BiVariant<TypeA, TypeB>::from (TypeB &_copy)
		{
			return GAS_UTILS_PARENT_FROM_MEMBER (_copy, BiVariant, m_data);
		}

		template<class TypeA, class TypeB>
		BiVariant<TypeA, TypeB>::BiVariant (const TypeA &_copy) : m_data { _copy }, m_type { EType::EFirst }
		{}

		template<class TypeA, class TypeB>
		BiVariant<TypeA, TypeB>::BiVariant (TypeA &&_moved) : m_data { std::move (_moved) }, m_type { EType::EFirst }
		{}

		template<class TypeA, class TypeB>
		BiVariant<TypeA, TypeB>::BiVariant (const TypeB &_copy) : m_data { _copy }, m_type { EType::ESecond }
		{}

		template<class TypeA, class TypeB>
		BiVariant<TypeA, TypeB>::BiVariant (TypeB &&_moved) : m_data { std::move (_moved) }, m_type { EType::ESecond }
		{}

		template<class TypeA, class TypeB>
		BiVariant<TypeA, TypeB>::BiVariant (const BiVariant &_copy)
		{
			*this = _copy;
		}

		template<class TypeA, class TypeB>
		BiVariant<TypeA, TypeB>::BiVariant (BiVariant &&_moved)
		{
			*this = std::move (_moved);
		}

		template<class TypeA, class TypeB>
		BiVariant<TypeA, TypeB>::~BiVariant ()
		{
			destroy ();
		}

		template<class TypeA, class TypeB>
		BiVariant<TypeA, TypeB> &BiVariant<TypeA, TypeB>::operator=(const BiVariant &_copy)
		{
			switch (_copy.m_type)
			{
				case EType::EFirst:
					set (_copy.m_data.a);
					break;
				case EType::ESecond:
					set (_copy.m_data.b);
					break;
				default:
					assert (false);
			}
			return *this;
		}

		template<class TypeA, class TypeB>
		BiVariant<TypeA, TypeB> &BiVariant<TypeA, TypeB>::operator=(BiVariant &&_moved)
		{
			switch (_moved.m_type)
			{
				case EType::EFirst:
					set (std::move (_moved.m_data.a));
					break;
				case EType::ESecond:
					set (std::move (_moved.m_data.b));
					break;
				default:
					assert (false);
			}
			_moved.destroy ();
			return *this;
		}

		template<class TypeA, class TypeB>
		BiVariant<TypeA, TypeB> &BiVariant<TypeA, TypeB>::operator=(const TypeA &_copy)
		{
			set (_copy);
			return *this;
		}

		template<class TypeA, class TypeB>
		BiVariant<TypeA, TypeB> &BiVariant<TypeA, TypeB>::operator=(TypeA &&_moved)
		{
			set (std::move (_moved));
			return *this;
		}

		template<class TypeA, class TypeB>
		BiVariant<TypeA, TypeB> &BiVariant<TypeA, TypeB>::operator=(const TypeB &_copy)
		{
			set (_copy);
			return *this;
		}

		template<class TypeA, class TypeB>
		BiVariant<TypeA, TypeB> &BiVariant<TypeA, TypeB>::operator=(TypeB &&_moved)
		{
			set (std::move (_moved));
			return *this;
		}

		template<class TypeA, class TypeB>
		bool BiVariant<TypeA, TypeB>::operator==(const BiVariant &_other) const
		{
			if (m_type == _other.m_type)
			{
				switch (m_type)
				{
					case EType::EFirst:
						return m_data.a == _other.m_data.a;
					case EType::ESecond:
						return m_data.b == _other.m_data.b;
				}
			}
			return false;
		}

		template<class TypeA, class TypeB>
		bool BiVariant<TypeA, TypeB>::operator!=(const BiVariant &_other) const
		{
			return !(*this == _other);
		}

		template<class TypeA, class TypeB>
		bool BiVariant<TypeA, TypeB>::isFirstType () const
		{
			return m_type == EType::EFirst;
		}

		template<class TypeA, class TypeB>
		bool BiVariant<TypeA, TypeB>::isSecondType () const
		{
			return m_type == EType::ESecond;
		}

		template<class TypeA, class TypeB>
		void BiVariant<TypeA, TypeB>::set (const TypeA &_copy)
		{
			destroy ();
			new (&m_data.a) TypeA { _copy };
			m_type = EType::EFirst;
		}

		template<class TypeA, class TypeB>
		void BiVariant<TypeA, TypeB>::set (TypeA &&_moved)
		{
			destroy ();
			new (&m_data.a) TypeA { std::move (_moved) };
			m_type = EType::EFirst;
		}

		template<class TypeA, class TypeB>
		void BiVariant<TypeA, TypeB>::set (const TypeB &_copy)
		{
			destroy ();
			new (&m_data.b) TypeB { _copy };
			m_type = EType::ESecond;
		}

		template<class TypeA, class TypeB>
		void BiVariant<TypeA, TypeB>::set (TypeB &&_moved)
		{
			destroy ();
			new (&m_data.b) TypeB { std::move (_moved) };
			m_type = EType::ESecond;
		}

		template<class TypeA, class TypeB>
		const TypeA &BiVariant<TypeA, TypeB>::first () const
		{
			assert (isFirstType ());
			return m_data.a;
		}

		template<class TypeA, class TypeB>
		const TypeB &BiVariant<TypeA, TypeB>::second () const
		{
			assert (isSecondType ());
			return m_data.b;
		}

		template<class TypeA, class TypeB>
		TypeA &BiVariant<TypeA, TypeB>::first ()
		{
			assert (isFirstType ());
			return m_data.a;
		}

		template<class TypeA, class TypeB>
		TypeB &BiVariant<TypeA, TypeB>::second ()
		{
			assert (isSecondType ());
			return m_data.b;
		}

	}

}

