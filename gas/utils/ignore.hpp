/// Empty class for ignoring arguments.
/// \file
/// \author Francesco Zoccheddu

#ifndef GAS_UTILS_IGNORE_INCLUDED
#define GAS_UTILS_IGNORE_INCLUDED

namespace GAS
{

	namespace Utils
	{

		/// Does nothing.
		/// It is copy and move constructible and assignable with any argument type.
		struct Ignore
		{

			Ignore () {};

			template<class Any>
			Ignore (const Any &) {}

			template<class Any>
			Ignore (Any &&) {}

			template<class Any>
			Ignore &operator=(const Any &)
			{
				return *this;
			}

			template<class Any>
			Ignore &operator=(Any &&)
			{
				return *this;
			}

		};

	}

}

#endif