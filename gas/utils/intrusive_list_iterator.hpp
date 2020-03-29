/// Instrusive list iterator class.
/// \file
/// \author Francesco Zoccheddu

#ifndef GAS_UTILS_INTRUSIVE_LIST_ITERATOR_INCLUDED
#define GAS_UTILS_INTRUSIVE_LIST_ITERATOR_INCLUDED

#include <type_traits>
#include <gas/utils/iterators.hpp>

namespace GAS
{

	namespace Utils
	{

		/*
			I could have used \c boost::intrusive::list but it seems that external libraries are not allowed by the project specifications.
			Anyway boost's \c list_member_hook would have required me to redesign part of the BDAG.
		*/
		/// Iterator for generic intrusive list implementations.
		/// \tparam Node
		/// The node type.
		///	\tparam Previous
		/// Member pointer to the previous node pointer member in the \c Node type.
		///	\tparam Next
		/// Member pointer to the next node pointer member in the \c Node type.
		template<typename Node,
			typename std::remove_const<Node>::type * std::remove_const<Node>::type:: * Previous,
			typename std::remove_const<Node>::type * std::remove_const<Node>::type:: * Next>
			class IntrusiveListIterator final
		{

			Node *m_current {};

		public:

			/// Convenience type alias for Iterable.
			using Iterable = Iterable<IntrusiveListIterator>;

			/// The	\c end iterator.
			/// \remark
			/// The \c end iterator does not refer to an actual node, thus should not be dereferenced.
			/// \remark
			/// The \c end iterator does not depend on the state of the list and can also be obtained through the default constructor IntrusiveListIterator().
			static const IntrusiveListIterator end;

			/// Construct an \c end iterator.
			/// \see #end.
			IntrusiveListIterator () = default;

			/// Construct an iterator referring to the specified node.
			/// \param[in] node
			/// The node to refer to.
			IntrusiveListIterator (Node &node);

			/// Moves the iterator to refer to the next node in the list.
			/// \pre
			/// The iterator must not compare equal to #end.
			/// \return
			/// The same iterator.
			IntrusiveListIterator &operator++();

			/// Moves the iterator to refer to the next node in the list.
			/// \pre
			/// The iterator must not compare equal to #end.
			/// \return
			/// The old iterator.
			IntrusiveListIterator operator++(int);

			/// Moves the iterator to refer to the previous node in the list.
			/// \pre
			/// The iterator must not compare equal to #end.
			/// \return
			/// The same iterator.
			IntrusiveListIterator &operator--();

			/// Moves the iterator to refer to the previous node in the list.
			/// \pre
			/// The iterator must not compare equal to #end.
			/// \return
			/// The old iterator.
			IntrusiveListIterator operator--(int);

			///	\return
			/// \c true if the iterator refers to the same node of \p other, \c false otherwise.
			bool operator==(const IntrusiveListIterator &other) const;

			///	\return
			/// \c true if the iterator does not refer to the same node of \p other, \c false otherwise.
			bool operator!=(const IntrusiveListIterator &other) const;

			/// \pre
			/// The iterator must not compare equal to #end.
			/// \return
			/// The node to which the iterator refers to.
			Node &operator*() const;

			/// \pre
			/// The iterator must not compare equal to #end.
			/// \return
			/// The node to which the iterator refers to.
			Node *operator->() const;

		};

	}

}

#include "intrusive_list_iterator.tpp"

#endif