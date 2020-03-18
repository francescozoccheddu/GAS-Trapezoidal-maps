#pragma once

#include "binary_dag.hpp"

#include <cassert>
#include <vector>
#include <utility>

namespace GAS
{

	namespace BDAG
	{

		template<class Data>
		const Node<Data> &Node<Data>::fromData (const char *_data)
		{
			static_assert(std::is_standard_layout<Node>::value, "Node is not a standard layout type");
			return *reinterpret_cast<const Node *>(_data - offsetof (Node, m_data));
		}

		template<class Data>
		const Node<Data> &Node<Data>::from (const Data &_data)
		{
			return getNode (reinterpret_cast<const char *>(&_data));
		}

		template<class Data>
		Node<Data> &Node<Data>::from (Data &_data)
		{
			// Casting away constness is safe since _data is non-const
			return const_cast<Node &>(fromData (reinterpret_cast<const char *>(&_data)));
		}

		template<class Data>
		Node<Data>::Node (const Data &_data, Node &_left, Node &_right) : m_data { _data }, m_leaf { false }, m_left { &_left }, m_right { &_right }
		{}

		template<class Data>
		Node<Data>::Node (Data &&_data, Node &_left, Node &_right) : m_data { _data }, m_leaf { false }, m_left { &_left }, m_right { &_right }
		{}

		template<class Data>
		Node<Data>::Node (const Data &_data) : m_data { _data }, m_leaf { true }
		{}

		template<class Data>
		Node<Data>::Node (Data &&_data) : m_data { _data }, m_leaf { true }
		{}

		template<class Data>
		bool Node<Data>::isLeaf () const
		{
			return m_leaf;
		}

		template<class Data>
		const Data &Node<Data>::data () const
		{
			return m_data;
		}

		template<class Data>
		const Node<Data> &Node<Data>::left () const
		{
			assert (!isLeaf ());
			return *m_left;
		}

		template<class Data>
		const Node<Data> &Node<Data>::right () const
		{
			assert (!isLeaf ());
			return *m_right;
		}

		template<class Data>
		Data &Node<Data>::data ()
		{
			return m_data;
		}

		template<class Data>
		Node<Data> &Node<Data>::left ()
		{
			assert (!isLeaf ());
			return *m_left;
		}

		template<class Data>
		Node<Data> &Node<Data>::right ()
		{
			assert (!isLeaf ());
			return *m_right;
		}

		template<class Data>
		void Node<Data>::setInner (Node &_left, Node &_right)
		{
			m_leaf = false;
			m_left = &_left;
			m_right = &_right;
		}

		template<class Data>
		void Node<Data>::setLeaf ()
		{
			m_leaf = true;
		}

#ifdef GAS_BDAG_ENABLE_VISITED_FLAG

		template<class Data>
		bool Node<Data>::isVisited () const
		{
			return m_visited;
		}

		template<class Data>
		void Node<Data>::setVisitedFlag () const
		{
			m_visited = true;
		}

		template<class Data>
		void Node<Data>::resetVisitedFlag () const
		{
			m_visited = false;
		}

#endif

		template<class Data>
		ConstIterator<Data>::ConstIterator () : m_current {}
		{}

		template<class Data>
		void ConstIterator<Data>::postpone (const Node<Data> &_node)
		{
#ifdef GAS_BDAG_ENABLE_VISITED_FLAG
			if (!_node.isVisited ())
			{
				_node.setVisitedFlag ();
#else
			if (std::get<1> (m_visited.insert (&_node)))
			{
#endif
				m_postponed.push_back (&_node);
			}
		}

		template<class Data>
		const ConstIterator<Data> ConstIterator<Data>::end {};

#ifdef GAS_BDAG_ENABLE_VISITED_FLAG
		template<class Data>
		ConstIterator<Data>::ConstIterator (const Node<Data> & _node) : m_current { &_node }, m_first { &_node }
#else
		template<class Data>
		ConstIterator<Data>::ConstIterator (const Node<Data> & _node, std::size_t _sizeHint) : m_current { &_node }
#endif
		{
#ifndef GAS_BDAG_ENABLE_VISITED_FLAG
			if (_sizeHint > 0)
			{
				m_visited.reserve (_sizeHint);
			}
#endif
			postpone (*m_current);
			++(*this);
		}

#ifdef GAS_BDAG_ENABLE_VISITED_FLAG
		template<class Data>
		ConstIterator<Data>::~ConstIterator ()
		{
			resetVisitedFlag (*m_first);
		}
#endif

		template<class Data>
		const Node<Data> &ConstIterator<Data>::operator* () const
		{
			assert (m_current != nullptr);
			return *m_current;
		}

		template<class Data>
		const Node<Data> *ConstIterator<Data>::operator& () const
		{
			assert (m_current != nullptr);
			return m_current;
		}

		template<class Data>
		const Node<Data> *ConstIterator<Data>::operator-> () const
		{
			assert (m_current != nullptr);
			return m_current;
		}

		template<class Data>
		ConstIterator<Data> &ConstIterator<Data>::operator++ ()
		{
			if (!m_postponed.empty ())
			{
				m_current = m_postponed.front ();
				m_postponed.pop_front ();
				if (!m_current->isLeaf ())
				{
					postpone (m_current->left ());
					postpone (m_current->right ());
				}
			}
			else
			{
				m_current = nullptr;
			}
			return *this;
		}

		template<class Data>
		bool ConstIterator<Data>::operator != (const ConstIterator & _other) const
		{
			return m_current != m_current || m_postponed != _other.m_postponed;
		}

#ifdef GAS_BDAG_ENABLE_VISITED_FLAG
		template<class Data>
		Iterator<Data>::Iterator (Node<Data> & _node) : ConstIterator { _node }
		{}
#else
		template<class Data>
		Iterator<Data>::Iterator (Node<Data> & _node, std::size_t _sizeHint) : ConstIterator { _node, _sizeHint }
		{}
#endif

		template<class Data>
		Node<Data> &Iterator<Data>::operator* () const
		{
			// Casting away constness is safe since _node was non-const
			return const_cast<Node<Data> &> (ConstIterator::operator*());
		}

		template<class Data>
		Node<Data> *Iterator<Data>::operator& () const
		{
			// Casting away constness is safe since _node was non-const
			return const_cast<Node<Data> *> (ConstIterator::operator&());
		}

		template<class Data>
		Node<Data> *Iterator<Data>::operator-> () const
		{
			// Casting away constness is safe since _node was non-const
			return const_cast<Node<Data> *> (ConstIterator::operator->());
		}

#ifdef GAS_BDAG_ENABLE_VISITED_FLAG
		template<class Data>
		void deleteComponent (const Node<Data> & _node)
		{
			for (ConstIterator it { _node }; it != ConstIterator<Data>::end; ++it)
			{
				it->resetVisitedFlag ();
			}
		}
#endif

		template<class Data>
		void deleteGraph (Node<Data> & _node, std::size_t _sizeHint)
		{
#ifdef GAS_BDAG_ENABLE_VISITED_FLAG
			std::vector<Node<Data> *> nodes;
			if (_sizeHint > 0)
			{
				nodes.reserve (_sizeHint);
			}
			for (Iterator<Data> it { _node }; it != Iterator<Data>::end; ++it)
			{
				nodes.push_back (&it);
			}
			for (Iterator<Data> it { _node }; it != Iterator<Data>::end; ++it)
			{
				delete &it;
			}
#else
			for (Iterator<Data> it { _node, _sizeHint }; it != Iterator<Data>::end; ++it)
			{
				delete &it;
			}
#endif
		}

		template<class Data, class Walker>
		const Node<Data> &walk (const Node<Data> & _node, Walker _walker)
		{
			const Node<Data> *node { &_node };
			while (!node->isLeaf ())
			{
				switch (_walker (node->data ()))
				{
					case EChild::Left:
						node = &node->left ();
						break;
					case EChild::Right:
						node = &node->right ();
						break;
					default:
						assert (false);
				}
			}
			return *node;
		}

		template<class Data, class Walker>
		Node<Data> &walk (Node<Data> & _node, Walker _walker)
		{
			// Casting away constness is safe since _node is non-const
			return const_cast<Node<Data> &>(walk (static_cast<const Node<Data> &> (_node), _walker));
		}

	}

}