#pragma once

#include "iterator_utils.hpp"

#include <utility>

namespace GAS
{

	namespace Utils
	{

		template<class Iterator, class Output, Output & (*Function)(const Iterator &)>
		IteratorAdapter<Iterator, Output, Function>::IteratorAdapter (const Iterator &_iterator) : m_iterator { _iterator }
		{}

		template<class Iterator, class Output, Output & (*Function)(const Iterator &)>
		IteratorAdapter<Iterator, Output, Function>::IteratorAdapter (Iterator &&_iterator) : m_iterator { std::move (_iterator) }
		{}

		template<class Iterator, class Output, Output & (*Function)(const Iterator &)>
		IteratorAdapter<Iterator, Output, Function> &IteratorAdapter<Iterator, Output, Function>::operator++()
		{
			++m_iterator;
			return *this;
		}

		template<class Iterator, class Output, Output & (*Function)(const Iterator &)>
		IteratorAdapter<Iterator, Output, Function> IteratorAdapter<Iterator, Output, Function>::operator++(int)
		{
			IteratorAdapter before { *this };
			m_iterator++;
			return before;
		}

		template<class Iterator, class Output, Output & (*Function)(const Iterator &)>
		IteratorAdapter<Iterator, Output, Function> &IteratorAdapter<Iterator, Output, Function>::operator--()
		{
			--m_iterator;
			return *this;
		}

		template<class Iterator, class Output, Output & (*Function)(const Iterator &)>
		IteratorAdapter<Iterator, Output, Function> IteratorAdapter<Iterator, Output, Function>::operator--(int)
		{
			IteratorAdapter before { *this };
			m_iterator--;
			return before;
		}

		template<class Iterator, class Output, Output & (*Function)(const Iterator &)>
		bool IteratorAdapter<Iterator, Output, Function>::operator==(const IteratorAdapter &_other) const
		{
			return m_iterator == _other.m_iterator;
		}

		template<class Iterator, class Output, Output & (*Function)(const Iterator &)>
		bool IteratorAdapter<Iterator, Output, Function>::operator!=(const IteratorAdapter &_other) const
		{
			return m_iterator != _other.m_iterator;
		}

		template<class Iterator, class Output, Output & (*Function)(const Iterator &)>
		Output &IteratorAdapter<Iterator, Output, Function>::operator*() const
		{
			return Function (*m_iterator);
		}

		template<class Iterator, class Output, Output & (*Function)(const Iterator &)>
		Output *IteratorAdapter<Iterator, Output, Function>::operator->() const
		{
			return &(**this);
		}

		template<class Iterator>
		Iterable<Iterator>::Iterable (const Iterator &_begin, const Iterator &_end) : m_begin { _begin }, m_end { _end }
		{}

		template<class Iterator>
		Iterable<Iterator>::Iterable (Iterator &&_begin, Iterator &&_end) : m_begin { std::move (_begin) }, m_end { std::move (_end) }
		{}

		template<class Iterator>
		Iterator Iterable<Iterator>::begin () const
		{
			return m_begin;
		}

		template<class Iterator>
		const Iterator &Iterable<Iterator>::end () const
		{
			return m_end;
		}

	}

}