#pragma once

#include <cstddef>
#include <deque>
#ifndef GAS_BDAG_ENABLE_VISITED_FLAG
#include <unordered_set>
#endif

namespace GAS
{

	namespace BDAG
	{

		template<class Data>
		class Node final
		{

			Data m_data;
			bool m_leaf;
			Node *m_left, *m_right;

#ifdef GAS_BDAG_ENABLE_VISITED_FLAG
			mutable bool m_visited { false };
#endif

			static const Node &fromData (const char *data);

		public:

			static const Node &from (const Data &data);
			static Node &from (Data &data);

			Node (const Data &data, Node &left, Node &right);
			Node (Data &&data, Node &left, Node &right);
			Node (const Data &data);
			Node (Data &&data);

			bool isLeaf () const;
			const Data &data () const;
			const Node &left () const;
			const Node &right () const;

			Data &data ();
			Node &left ();
			Node &right ();

			void setInner (Node &left, Node &right);
			void setLeaf ();

#ifdef GAS_BDAG_ENABLE_VISITED_FLAG
			bool isVisited () const;
			void setVisitedFlag () const;
			void resetVisitedFlag () const;
#endif

		};

		enum class EChild
		{
			Left, Right
		};

		template<class Data>
		class ConstIterator
		{

			const Node<Data> *m_current;
			std::deque<const Node<Data> *> m_postponed;
#ifdef GAS_BDAG_ENABLE_VISITED_FLAG
			const Node<Data> *m_first;
#else
			std::unordered_set<const Node<Data> *> m_visited;
#endif
			ConstIterator ();

			void postpone (const Node<Data> &node);

		public:

			static const ConstIterator end;

#ifdef GAS_BDAG_ENABLE_VISITED_FLAG
			ConstIterator (const Node<Data> &node);
			~ConstIterator ();
#else
			ConstIterator (const Node<Data> &node, std::size_t sizeHint = 0);
#endif

			const Node<Data> &operator*() const;
			const Node<Data> *operator&() const;
			const Node<Data> *operator->() const;

			ConstIterator &operator++();

			bool operator!=(const ConstIterator &other) const;

		};

		template<class Data>
		class Iterator : public ConstIterator<Data>
		{

		public:

#ifdef GAS_BDAG_ENABLE_VISITED_FLAG
			Iterator (Node &node);
#else
			Iterator (Node<Data> &node, std::size_t sizeHint = 0);
#endif

			Node<Data> &operator*() const;
			Node<Data> *operator&() const;
			Node<Data> *operator->() const;

		};

#ifdef GAS_BDAG_ENABLE_VISITED_FLAG
		template<class Data>
		void resetVisitedFlag (const Node<Data> &node);
#endif

		template<class Data>
		void deleteGraph (Node<Data> &node, std::size_t sizeHint);

		template<class Data, class Walker>
		const Node<Data> &walk (const Node<Data> &node, Walker walker);

		template<class Data, class Walker>
		Node<Data> &walk (Node<Data> &node, Walker walker);

	}

}

#include "binary_dag.tpp"
