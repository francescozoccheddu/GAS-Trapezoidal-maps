#ifndef GAS_DATA_BINARY_DAG_IMPL_INCLUDED
#define GAS_DATA_BINARY_DAG_IMPL_INCLUDED

#ifndef GAS_DATA_BINARY_DAG_INCLUDED
#error 'gas/data/binary_dag.tpp' should not be directly included
#endif

#include "binary_dag.hpp"

#include <cassert>
#include <unordered_map>
#include <utility>
#include <gas/utils/parent_from_member.hpp>

namespace GAS
{

	namespace BDAG
	{

		template<class Data>
		Node<Data>::Node (const Data &_data) : m_data { _data }
		{}

		template<class Data>
		Node<Data>::Node (Data &&_data) : m_data { std::move (_data) }
		{}

		template<class Data>
		const Node<Data> &Node<Data>::from (const Data &_data)
		{
			return GAS_UTILS_PARENT_FROM_MEMBER (_data, Node, m_data);
		}

		template<class Data>
		Node<Data> &Node<Data>::from (Data &_data)
		{
			return GAS_UTILS_PARENT_FROM_MEMBER (_data, Node, m_data);
		}

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
		const Data &Node<Data>::operator*() const
		{
			return m_data;
		}

		template<class Data>
		const Data *Node<Data>::operator->() const
		{
			return &m_data;
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
		Data &Node<Data>::operator*()
		{
			return m_data;
		}

		template<class Data>
		Data *Node<Data>::operator->()
		{
			return &m_data;
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
		void Graph<Data>::registerNode (Node &_node)
		{
			m_nodesCount++;
			if (m_lastNode)
			{
				m_lastNode->m_next = &_node;
			}
			else
			{
				m_firstNode = &_node;
			}
			_node.m_previous = m_lastNode;
			m_lastNode = &_node;
		}

		template<class Data>
		void Graph<Data>::registerLeaf (Node &_node)
		{
			assert (_node.isLeaf ());
			m_leafNodesCount++;
			if (m_lastLeafNode)
			{
				m_lastLeafNode->m_right = &_node;
			}
			else
			{
				m_firstLeafNode = &_node;
			}
			_node.m_left = m_lastLeafNode;
			m_lastLeafNode = &_node;
		}

		template<class Data>
		void Graph<Data>::unregisterNode (Node &_node)
		{
			m_nodesCount--;
			if (_node.m_previous)
			{
				_node.m_previous->m_next = _node.m_next;
			}
			if (_node.m_next)
			{
				_node.m_next->m_previous = _node.m_previous;
			}
			if (&_node == m_firstNode)
			{
				m_firstNode = _node.m_next;
			}
			if (&_node == m_lastNode)
			{
				m_lastNode = _node.m_previous;
			}
		}

		template<class Data>
		void Graph<Data>::unregisterLeaf (Node &_node)
		{
			assert (_node.isLeaf ());
			m_leafNodesCount--;
			if (_node.m_left)
			{
				_node.m_left->m_right = _node.m_right;
			}
			if (_node.m_right)
			{
				_node.m_right->m_left = _node.m_left;
			}
			if (&_node == m_firstLeafNode)
			{
				m_firstLeafNode = _node.m_right;
			}
			if (&_node == m_lastLeafNode)
			{
				m_lastLeafNode = _node.m_left;
			}
		}

		template<class Data>
		Graph<Data>::Graph (const Graph &_copy)
		{
			// See copy assignment operator
			*this = _copy;
		}

		template<class Data>
		Graph<Data>::Graph (Graph &&_moved) :
			m_firstNode { _moved.m_firstNode }, m_lastNode { _moved.m_lastNode },
			m_firstLeafNode { _moved.m_firstLeafNode }, m_lastLeafNode { _moved.m_lastLeafNode },
			m_nodesCount { _moved.m_nodesCount }, m_leafNodesCount { _moved.m_leafNodesCount }
		{
			_moved.m_firstNode = _moved.m_lastNode = _moved.m_firstLeafNode = _moved.m_lastLeafNode = nullptr;
			_moved.m_nodesCount = _moved.m_leafNodesCount = 0;
			_moved.clear ();
		}

		template<class Data>
		Graph<Data>::~Graph ()
		{
			clear ();
		}

		template<class Data>
		Graph<Data> &Graph<Data>::operator=(const Graph &_copy)
		{
			clear ();
			std::unordered_map<const Node *, Node *> map;
			map.reserve (_copy.nodesCount ());
			for (const Node &node : _copy.nodes ())
			{
				Node &clone { *new Node { node.m_data } };
				clone.m_leaf = node.m_leaf;
				registerNode (clone);
				if (clone.m_leaf)
				{
					registerLeaf (clone);
				}
				else
				{
					clone.m_left = node.m_left;
					clone.m_right = node.m_right;
				}
				map.emplace (&node, &clone);
			}
			for (const std::pair<const Node *, Node *> &entry : map)
			{
				Node &node { *entry.second };
				if (!node.m_leaf)
				{
					node.m_left = map.at (node.m_left);
					node.m_right = map.at (node.m_right);
				}
			}
			return *this;
		}

		template<class Data>
		Graph<Data> &Graph<Data>::operator=(Graph &&_moved)
		{
			clear ();
			m_firstNode = _moved.m_firstNode;
			m_lastNode = _moved.m_lastNode;
			m_lastLeafNode = _moved.m_lastLeafNode;
			m_lastLeafNode = _moved.m_lastLeafNode;
			m_nodesCount = _moved.m_nodesCount;
			m_leafNodesCount = _moved.m_leafNodesCount;
			_moved.m_firstNode = _moved.m_lastNode = _moved.m_firstLeafNode = _moved.m_lastLeafNode = nullptr;
			_moved.m_nodesCount = _moved.m_leafNodesCount = 0;
			_moved.clear ();
		}

		template<class Data>
		bool Graph<Data>::isEmpty () const
		{
			return !m_nodesCount;
		}

		template<class Data>
		int Graph<Data>::nodesCount () const
		{
			return m_nodesCount;
		}

		template<class Data>
		int Graph<Data>::leafNodesCount () const
		{
			return m_leafNodesCount;
		}

		template<class Data>
		int Graph<Data>::innerNodesCount () const
		{
			return m_nodesCount - m_leafNodesCount;
		}

		template<class Data>
		Node<Data> &Graph<Data>::createLeaf ()
		{
			Node &node { *new Node };
			registerNode (node);
			registerLeaf (node);
			return node;
		}

		template<class Data>
		Node<Data> &Graph<Data>::createLeaf (const Data &_data)
		{
			Node &node { *new Node { _data } };
			registerNode (node);
			registerLeaf (node);
			return node;
		}

		template<class Data>
		Node<Data> &Graph<Data>::createLeaf (Data &&_data)
		{
			Node &node { *new Node { std::move (_data) } };
			registerNode (node);
			registerLeaf (node);
			return node;
		}

		template<class Data>
		Node<Data> &Graph<Data>::createInner (Node &_left, Node &_right)
		{
			Node &node { *new Node };
			node.m_left = &_left;
			node.m_right = &_right;
			node.m_leaf = false;
			registerNode (node);
			return node;
		}

		template<class Data>
		Node<Data> &Graph<Data>::createInner (const Data &_data, Node &_left, Node &_right)
		{
			Node &node { *new Node { _data } };
			node.m_left = &_left;
			node.m_right = &_right;
			node.m_leaf = false;
			registerNode (node);
			return node;
		}

		template<class Data>
		Node<Data> &Graph<Data>::createInner (Data &&_data, Node &_left, Node &_right)
		{
			Node &node { *new Node { std::move (_data) } };
			node.m_left = &_left;
			node.m_right = &_right;
			node.m_leaf = false;
			registerNode (node);
			return node;
		}

		template<class Data>
		void Graph<Data>::setLeaf (Node &_node)
		{
			if (!_node.isLeaf ())
			{
				_node.m_leaf = true;
				registerLeaf (_node);
			}
		}

		template<class Data>
		void Graph<Data>::setInner (Node &_node, Node &_left, Node &_right)
		{
			if (_node.isLeaf ())
			{
				unregisterLeaf (_node);
				_node.m_left = &_left;
				_node.m_right = &_right;
				_node.m_leaf = false;
			}
		}

		template<class Data>
		void Graph<Data>::destroyNode (Node &_node)
		{
			unregisterNode (_node);
			if (_node.isLeaf ())
			{
				unregisterLeaf (_node);
			}
			delete &_node;
		}

		template<class Data>
		void Graph<Data>::clear ()
		{
			if (!isEmpty ())
			{
				// Deleting the current node invalidates the iterator
				Node *lastNode {};
				for (Node &node : nodes ())
				{
					delete lastNode;
					lastNode = &node;
				}
				delete lastNode;
			}
			m_firstNode = m_lastNode = m_firstLeafNode = m_lastLeafNode = nullptr;
			m_nodesCount = m_leafNodesCount = 0;
		}

		template<class Data>
		typename Graph<Data>::ConstNodeIterator::Iterable Graph<Data>::nodes () const
		{
			return ConstNodeIterator::Iterable { *m_firstNode };
		}

		template<class Data>
		typename Graph<Data>::NodeIterator::Iterable Graph<Data>::nodes ()
		{
			return NodeIterator::Iterable { *m_firstNode };
		}

		template<class Data>
		typename Graph<Data>::ConstLeafNodeIterator::Iterable Graph<Data>::leafNodes () const
		{
			return ConstLeafNodeIterator::Iterable { *m_firstLeafNode };
		}

		template<class Data>
		typename Graph<Data>::LeafNodeIterator::Iterable Graph<Data>::leafNodes ()
		{
			return LeafNodeIterator::Iterable { *m_firstLeafNode };
		}

		template<class Data, class Walker>
		const Node<Data> &walk (const Node<Data> &_root, Walker _walker)
		{
			const Node<Data> *node { &_root };
			while (!node->isLeaf ())
			{
				switch (_walker (node->data ()))
				{
					default:
						assert (false);
					case EChild::Left:
						node = &node->left ();
						break;
					case EChild::Right:
						node = &node->right ();
						break;
				}
			}
			return *node;
		}

		template<class Data, class Walker>
		Node<Data> &walk (Node<Data> &_root, Walker _walker)
		{
			// Casting away constness is safe since _root is non-const
			return const_cast<Node<Data> &>(walk (static_cast<const Node<Data> &>(_root), _walker));
		}

	}

}

#endif