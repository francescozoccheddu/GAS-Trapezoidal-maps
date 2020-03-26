#pragma once

#include <type_traits>
#include <cstddef>

/*
	I could have used boost::intrusive::get_parent_from_member but it seems that external libraries are not allowed by the project specifications.
*/
#define GAS_UTILS_PARENT_FROM_MEMBER(_member, _parentType, _memberName) (GAS::Utils::Internals::fromMember<_parentType, offsetof(_parentType, _memberName)>(_member))

namespace GAS
{

	namespace Utils
	{

		namespace Internals
		{

			template<class Type, size_t MemberOffset, class MemberType>
			inline const Type &fromMember (const MemberType &_member)
			{
				static_assert(std::is_standard_layout<Type>::value, "Not a standard layout type");
				return *reinterpret_cast<const Type *>(reinterpret_cast<const char *>(&_member) - MemberOffset);
			}

			template<class Type, size_t MemberOffset, class MemberType>
			inline Type &fromMember (MemberType &_member)
			{
				// Casting away constness is safe since _member is non-const
				return const_cast<Type &>(fromMember<Type, MemberOffset> (static_cast<const MemberType &>(_member)));
			}

		}

	}

}