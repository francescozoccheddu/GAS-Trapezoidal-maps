#ifndef GAS_UTILS_INTRUSIVE_LIST_ITERATOR_INCLUDED
#define GAS_UTILS_INTRUSIVE_LIST_ITERATOR_INCLUDED

#include <type_traits>
#include <gas/utils/iterators.hpp>

namespace GAS
{

	namespace Utils
	{

		/*
			I could have used boost::intrusive::list but it seems that external libraries are not allowed by the project specifications.
			Anyway boost's list_member_hook would have required me to redesign part of the BDAG.
		*/
		template<typename Node,
			typename std::remove_const<Node>::type * std::remove_const<Node>::type:: * Previous,
			typename std::remove_const<Node>::type * std::remove_const<Node>::type:: * Next>
			class IntrusiveListIterator final
		{

			Node *m_current {};

		public:

			using Iterable = Iterable<IntrusiveListIterator>;

			static const IntrusiveListIterator end;

			IntrusiveListIterator () = default;
			IntrusiveListIterator (Node &node);

			IntrusiveListIterator &operator++();
			IntrusiveListIterator operator++(int);
			IntrusiveListIterator &operator--();
			IntrusiveListIterator operator--(int);

			bool operator==(const IntrusiveListIterator &other) const;
			bool operator!=(const IntrusiveListIterator &other) const;

			Node &operator*() const;
			Node *operator->() const;

		};

	}

}

#include "intrusive_list_iterator.tpp"

#endif