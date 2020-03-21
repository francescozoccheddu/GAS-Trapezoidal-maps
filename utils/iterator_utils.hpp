#pragma once

#include <type_traits>
#include <utility>

namespace GAS
{

	namespace Utils
	{

		/*
			I could have used boost::iterator_adaptor but it seems that external libraries are not allowed by the project specifications.
		*/
		template<class Iterator, class Output, Output & (*Function)(const Iterator &)>
		class IteratorAdapter final
		{

			Iterator m_iterator {};

		public:

			IteratorAdapter () = default;
			IteratorAdapter (const Iterator &iterator);
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

#include "iterator_utils.tpp"
