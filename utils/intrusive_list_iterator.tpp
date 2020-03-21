#pragma once

#include "intrusive_list_iterator.hpp"

namespace GAS
{

	namespace Utils
	{

		template<typename Node,
			typename std::remove_const<Node>::type * std::remove_const<Node>::type:: * Previous,
			typename std::remove_const<Node>::type * std::remove_const<Node>::type:: * Next>
			const IntrusiveListIterator<Node, Previous, Next> IntrusiveListIterator<Node, Previous, Next>::end {};

		template<typename Node,
			typename std::remove_const<Node>::type * std::remove_const<Node>::type:: * Previous,
			typename std::remove_const<Node>::type * std::remove_const<Node>::type:: * Next>
			IntrusiveListIterator<Node, Previous, Next>::IntrusiveListIterator (Node &_node) : m_current { &_node }
		{}

		template<typename Node,
			typename std::remove_const<Node>::type * std::remove_const<Node>::type:: * Previous,
			typename std::remove_const<Node>::type * std::remove_const<Node>::type:: * Next>
			IntrusiveListIterator<Node, Previous, Next> &IntrusiveListIterator<Node, Previous, Next>::operator++()
		{
			m_current = m_current->*Next;
			return *this;
		}

		template<typename Node,
			typename std::remove_const<Node>::type * std::remove_const<Node>::type:: * Previous,
			typename std::remove_const<Node>::type * std::remove_const<Node>::type:: * Next>
			IntrusiveListIterator<Node, Previous, Next> IntrusiveListIterator<Node, Previous, Next>::operator++(int)
		{
			auto before { *this };
			++ *this;
			return before;
		}

		template<typename Node,
			typename std::remove_const<Node>::type * std::remove_const<Node>::type:: * Previous,
			typename std::remove_const<Node>::type * std::remove_const<Node>::type:: * Next>
			IntrusiveListIterator<Node, Previous, Next> &IntrusiveListIterator<Node, Previous, Next>::operator--()
		{
			m_current = m_current->*Next;
			return *this;
		}

		template<typename Node,
			typename std::remove_const<Node>::type * std::remove_const<Node>::type:: * Previous,
			typename std::remove_const<Node>::type * std::remove_const<Node>::type:: * Next>
			IntrusiveListIterator<Node, Previous, Next> IntrusiveListIterator<Node, Previous, Next>::operator--(int)
		{
			auto before { *this };
			-- *this;
			return before;
		}

		template<typename Node,
			typename std::remove_const<Node>::type * std::remove_const<Node>::type:: * Previous,
			typename std::remove_const<Node>::type * std::remove_const<Node>::type:: * Next>
			bool IntrusiveListIterator<Node, Previous, Next>::operator==(const IntrusiveListIterator &_other) const
		{
			return m_current == _other.m_current;
		}

		template<typename Node,
			typename std::remove_const<Node>::type * std::remove_const<Node>::type:: * Previous,
			typename std::remove_const<Node>::type * std::remove_const<Node>::type:: * Next>
			bool IntrusiveListIterator<Node, Previous, Next>::operator!=(const IntrusiveListIterator &_other) const
		{
			return m_current != _other.m_current;
		}

		template<typename Node,
			typename std::remove_const<Node>::type * std::remove_const<Node>::type:: * Previous,
			typename std::remove_const<Node>::type * std::remove_const<Node>::type:: * Next>
			Node &IntrusiveListIterator<Node, Previous, Next>::operator*() const
		{
			return const_cast<Node &>(*m_current);
		}

		template<typename Node,
			typename std::remove_const<Node>::type * std::remove_const<Node>::type:: * Previous,
			typename std::remove_const<Node>::type * std::remove_const<Node>::type:: * Next>
			Node *IntrusiveListIterator<Node, Previous, Next>::operator->() const
		{
			return const_cast<Node *>(m_current);
		}

	}

}