/// GAS::Utils::BiVariant utility class.
/// \file
/// \author Francesco Zoccheddu

#ifndef GAS_UTILS_BIVARIANT_INCLUDED
#define GAS_UTILS_BIVARIANT_INCLUDED

#include <gas/utils/ignore.hpp>

namespace GAS
{

	/// Generic utility classes and functions.
	namespace Utils
	{

		/// Union-like structure with two members.
		/// Keeps track of the active member and automatically handles its lifecycle.
		/// \tparam First
		/// The type of the first member.
		/// \tparam Second
		/// The type of the second member.
		/// \pre
		/// \c First must be different from \c Second.
		/// \note
		/// I could have used \c boost::variant but it seems that external libraries are not allowed by the project specifications.
		/// Also \c std::variant is not available prior to C++17.
		template<class First, class Second>
		class BiVariant final
		{

			union Union
			{
				Ignore ignore {};
				First first;
				Second second;

				Union ();
				Union (const First &copy);
				Union (First &&moved);
				Union (const Second &copy);
				Union (Second &&moved);

				~Union ();

			} m_data;

			/// Enum to keep track of the active member.
			enum class EType
			{
				EFirst, ESecond, EDestroyed
			} m_type { EType::EDestroyed };

			/// Destroy the current active member (if any).
			void destroy ();

		public:

			/// Get the bivariant having \p first as the first active member.
			/// \pre 
			/// \p First type must be a standard layout type.
			/// \param[in] first
			/// An object returned by the first() method.
			/// \return
			/// The bivariant holding \p first.
			static const BiVariant &from (const First &first);

			/// Get the bivariant having \p second as the second active member.
			/// \pre 
			/// \p Second type must be a standard layout type.
			/// \param[in] second
			/// An object returned by the second() method.
			/// \return
			/// The bivariant holding \p second.
			static const BiVariant &from (const Second &second);

			/// \copydoc from(const First&)
			static BiVariant &from (First &first);

			/// \copydoc from(const Second&)
			static BiVariant &from (Second &second);

			/// Construct a bivariant by cloning \p copy and making it the first active member.
			/// The first member is constructed by calling its copy constructor.
			/// \param[in] copy
			/// The object to clone.
			/// \pre
			/// \c First type must be copy constructible.
			BiVariant (const First &copy);

			/// Construct a bivariant by moving \p moved and making it the first active member.
			/// The first member is constructed by calling its move constructor.
			/// \param[in] moved
			/// The object to move.
			/// \pre
			/// \c First type must be move constructible.
			BiVariant (First &&moved);

			/// Construct a bivariant by cloning \p copy and making it the second active member.
			/// The second member is constructed by calling its copy constructor.
			/// \param[in] copy
			/// The object to clone.
			/// \pre
			/// \c Second type must be copy constructible.
			BiVariant (const Second &copy);

			/// Construct a bivariant by moving \p moved and making it the second active member.
			/// The second member is constructed by calling its move constructor.
			/// \param[in] moved
			/// The object to move.
			/// \pre
			/// \c Second type must be move constructible.
			BiVariant (Second &&moved);

			/// Construct a bivariant by copying the active member of \p copy.
			/// The active member is constructed by calling its copy constructor.
			/// \param[in] copy
			/// The bivariant to clone.
			/// \pre
			/// The type of the active member of \p copy must be copy constructible.
			BiVariant (const BiVariant &copy);

			/// Construct a bivariant by moving the active member of \p moved.
			/// The active member is constructed by calling its move constructor then the active member of \p moved is destroyed.
			/// \param[in] moved
			/// The bivariant to move.
			/// \pre
			/// The type of the active member of \p moved must be move constructible.
			/// \remark
			/// After calling this constructor \p moved will not have any active member but it will still be valid.
			BiVariant (BiVariant &&moved);

			/// Destroy the currently active member.
			~BiVariant ();

			/// Destroy the current active member and replace it by copying the active member of \p copy.
			/// The active member is constructed by calling its copy constructor.
			/// \param[in] copy
			/// The bivariant to clone.
			/// \pre
			/// The type of the active member of \p copy must be copy constructible.
			BiVariant &operator=(const BiVariant &copy);

			/// Destroy the current active member and replace it by moving the active member of \p moved.
			/// The active member is constructed by calling its move constructor then the active member of \p moved is destroyed.
			/// \param[in] moved
			/// The bivariant to move.
			/// \pre
			/// The type of the active member of \p moved must be move constructible.
			/// \remark
			/// After calling this constructor \p moved will not have any active member but it will still be valid.
			BiVariant &operator=(BiVariant &&moved);

			/// \see set(const First&)
			BiVariant &operator=(const First &copy);

			/// \see set(First&&)
			BiVariant &operator=(First &&moved);

			/// \see set(const Second&)
			BiVariant &operator=(const Second &copy);

			/// \see set(Second&&)
			BiVariant &operator=(Second &&moved);

			/// \return
			/// \c true if the bivariant has the same active member of \p other and they compare equal, \c false otherwise.
			bool operator==(const BiVariant &other) const;

			/// \return
			/// \c true if the bivariant has not the same active member of \p other or if they do not compare equal, \c false otherwise.
			bool operator!=(const BiVariant &other) const;

			/// \return
			/// \c true if the first member is active, \c false otherwise.
			bool isFirstType () const;

			/// \return
			/// \c true if the second member is active, \c false otherwise.
			bool isSecondType () const;

			/// Destroy the current active member and set the first active member by cloning \p copy.
			/// The first member is constructed by calling its copy constructor.
			/// \param[in] copy
			/// The object to clone.
			/// \pre
			/// \c First type must be copy constructible.
			void set (const First &copy);

			/// Destroy the current active member and set the first active member by moving \p moved.
			/// The first member is constructed by calling its move constructor.
			/// \param[in] moved
			/// The object to move.
			/// \pre
			/// \c First type must be move constructible.
			void set (First &&moved);

			/// Destroy the current active member and set the second active member by cloning \p copy.
			/// The second member is constructed by calling its copy constructor.
			/// \param[in] copy
			/// The object to clone.
			/// \pre
			/// \c Second type must be copy constructible.
			void set (const Second &copy);

			/// Destroy the current active member and set the second active member by moving \p moved.
			/// The second member is constructed by calling its move constructor.
			/// \param[in] moved
			/// The object to move.
			/// \pre
			/// \c Second type must be move constructible.
			void set (Second &&moved);

			/// \pre
			/// The first member must be the current active member.
			/// \return
			/// The first member.
			const First &first () const;

			/// \pre
			/// The second member must be the current active member.
			/// \return
			/// The second member.
			const Second &second () const;

			/// \copydoc first
			First &first ();

			/// \copydoc second
			Second &second ();

		};

	}

}

#include "bivariant.tpp"

#endif