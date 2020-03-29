/// Utility macro for retrieving an object through a member pointer.
/// \file
/// \author Francesco Zoccheddu

#ifndef GAS_UTILS_PARENT_FROM_MEMBER_INCLUDED
#define GAS_UTILS_PARENT_FROM_MEMBER_INCLUDED

#include <type_traits>
#include <cstddef>

/// Get an object given a pointer to one of its members.
/// \param[in] member
/// The pointer to the member.
/// \param[in] parentType
/// The type of the parent object.
/// \param[in] memberName
/// The name of the attribute \p member of \p parentType.
/// \pre
/// \p parentType must be a standard layout type.
/// \return
/// The parent object of \p member with type \p parentType.
/// \note
/// I could have used \c boost::intrusive::get_parent_from_member but it seems that external libraries are not allowed by the project specifications.
#define GAS_UTILS_PARENT_FROM_MEMBER(member, parentType, memberName) (GAS::Utils::Internals::fromMember<parentType, offsetof(parentType, memberName)>(member))

namespace GAS
{

	namespace Utils
	{

		/// Internal functions not intended for direct usage.
		namespace Internals
		{

			/// Get an object given a pointer to one of its members.
			/// \tparam Type
			/// The type of the parent object.
			/// \tparam MemberOffset
			/// The offset of the member in the parent structure (obtainable through the \c offsetof macro).
			/// \tparam MemberType
			/// The member object type.
			/// \param[in] _member
			/// The member object.
			/// \pre
			/// \p Type must be a standard layout type.
			/// \return
			/// The parent object.
			template<class Type, size_t MemberOffset, class MemberType>
			inline const Type &fromMember (const MemberType &_member)
			{
				static_assert(std::is_standard_layout<Type>::value, "Not a standard layout type");
				return *reinterpret_cast<const Type *>(reinterpret_cast<const char *>(&_member) - MemberOffset);
			}

			/// \copydoc fromMember
			template<class Type, size_t MemberOffset, class MemberType>
			inline Type &fromMember (MemberType &_member)
			{
				// Casting away constness is safe since _member is non-const
				return const_cast<Type &>(fromMember<Type, MemberOffset> (static_cast<const MemberType &>(_member)));
			}

		}

	}

}

#endif