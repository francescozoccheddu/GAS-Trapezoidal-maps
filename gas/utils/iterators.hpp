/// Utility classes for iteration.
/// \file
/// \author Francesco Zoccheddu

#ifndef GAS_UTILS_ITERATORS_INCLUDED
#define GAS_UTILS_ITERATORS_INCLUDED

namespace GAS
{

	namespace Utils
	{

		/*
			I could have \c used boost::iterator_adaptor but it seems that external libraries are not allowed by the project specifications.
		*/
		/// Iterator adapter to map the output of a bidirectional iterator using a custom function.
		/// \tparam Iterator
		/// The type of the iterator.
		/// \tparam Output
		/// The adapted item type.
		/// \tparam Function
		/// The map function
		template<class Iterator, class Output, Output & (*Function)(const Iterator &)>
		class IteratorAdapter final
		{

			Iterator m_iterator {};

		public:

			/// Construct an adapter for the empty iterator.
			/// The undelying iterator is constructed by calling its default constructor.
			/// \pre
			/// \c Iterator type must be default constructible.
			IteratorAdapter () = default;

			/// Construct an adapter for the specified iterator.
			/// The undelying iterator is constructed by calling its copy constructor.
			/// \pre
			/// \c Iterator type must be copy constructible.
			IteratorAdapter (const Iterator &iterator);

			/// Construct an adapter for the specified iterator.
			/// The undelying iterator is constructed by calling its move constructor.
			/// \pre
			/// \c Iterator type must be move constructible.
			IteratorAdapter (Iterator &&iterator);

			IteratorAdapter &operator++();
			IteratorAdapter operator++(int);
			IteratorAdapter &operator--();
			IteratorAdapter operator--(int);

			bool operator==(const IteratorAdapter &other) const;
			bool operator!=(const IteratorAdapter &other) const;

			Output &operator*() const;
			Output *operator->() const;

		};

		/// Convenience class for providing iterators.
		/// \tparam Iterator
		/// The iterator type.
		template<class Iterator>
		class Iterable final
		{

			Iterator m_begin;
			Iterator m_end;

		public:

			Iterable (const Iterator &begin, const Iterator &end = {});
			Iterable (Iterator &&begin, Iterator &&end = {});

			Iterator begin () const;
			const Iterator &end () const;

		};

	}

}

#include "iterators.tpp"

#endif