#ifndef GAS_UTILS_BIVARIANT_INCLUDED
#define GAS_UTILS_BIVARIANT_INCLUDED

namespace GAS
{

	namespace Utils
	{

		/*
			I could have used boost::variant but it seems that external libraries are not allowed by the project specifications.
			Also std::variant is not available prior to C++17.
		*/
		template<class TypeA, class TypeB>
		class BiVariant final
		{

			struct Monostate {}; // std::monostate is not available prior to C++17.

			union Union
			{
				Monostate monostate {};
				TypeA a;
				TypeB b;

				Union ();
				Union (const TypeA &copy);
				Union (TypeA &&moved);
				Union (const TypeB &copy);
				Union (TypeB &&moved);

				~Union ();

			} m_data;

			enum class EType
			{
				EFirst, ESecond, EDestroyed
			} m_type { EType::EDestroyed };

			void destroy ();

		public:

			static const BiVariant &from (const TypeA &a);
			static const BiVariant &from (const TypeB &b);
			static BiVariant &from (TypeA &a);
			static BiVariant &from (TypeB &a);

			BiVariant (const TypeA &copy);
			BiVariant (TypeA &&moved);
			BiVariant (const TypeB &copy);
			BiVariant (TypeB &&moved);

			BiVariant (const BiVariant &copy);
			BiVariant (BiVariant &&moved);

			~BiVariant ();

			BiVariant &operator=(const BiVariant &copy);
			BiVariant &operator=(BiVariant &&moved);
			BiVariant &operator=(const TypeA &copy);
			BiVariant &operator=(TypeA &&moved);
			BiVariant &operator=(const TypeB &copy);
			BiVariant &operator=(TypeB &&moved);

			bool operator==(const BiVariant &other) const;
			bool operator!=(const BiVariant &other) const;

			bool isFirstType () const;
			bool isSecondType () const;

			void set (const TypeA &copy);
			void set (TypeA &&moved);
			void set (const TypeB &copy);
			void set (TypeB &&moved);

			const TypeA &first () const;
			const TypeB &second () const;
			TypeA &first ();
			TypeB &second ();

		};

	}

}

#include "bivariant.tpp"

#endif