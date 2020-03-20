#pragma once

#include <type_traits>

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

			class Iterable final
			{

				using Iterator = IntrusiveListIterator<Node, Previous, Next>;

				Node *m_begin {};

			public:

				Iterable (Node &begin);

				Iterator begin () const;
				const Iterator &end () const;

			};

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
