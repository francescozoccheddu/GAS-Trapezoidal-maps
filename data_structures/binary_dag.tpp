#pragma once

#include "binary_dag.hpp"

#include <cassert>
#include <vector>

namespace GAS
{

	namespace BDAG
	{

		template<class LeafData, class InnerData>
		const Node<LeafData, InnerData> &Node<LeafData, InnerData>::getNode (const char *_data)
		{
			static_assert(std::is_standard_layout<Node>::value, "Node is not a standard layout type");
			return *reinterpret_cast<const Node *>(_data - offsetof (Node, m_data));
		}

		template<class LeafData, class InnerData>
		void Node<LeafData, InnerData>::recursiveDelete (Node<LeafData, InnerData> &_node, std::size_t _sizeHint)
		{
			// Reserve space for better performance
			assert (_sizeHint >= 0);
			std::vector<Node *> consumed {};
			consumed.reserve (_sizeHint);
			std::vector<Node *> postponed {};
			postponed.reserve (_sizeHint / 2);
			// Traverse
			postponed.push_back (&_node);
			while (!postponed.empty ())
			{
				Node *node { postponed.back () };
				postponed.pop_back ();
				if (!node->m_deleted)
				{
					if (!node->isLeaf ())
					{
						postponed.push_back (&node->getLeft ());
						postponed.push_back (&node->getRight ());
					}
					node->m_deleted = true;
					consumed.push_back (node);
				}
			}
			// Delete
			for (Node *node : consumed)
			{
				delete node;
			}
		}

		template<class LeafData, class InnerData>
		const Node<LeafData, InnerData> &Node<LeafData, InnerData>::getNode (const LeafData &_data)
		{
			return getNode (reinterpret_cast<const char *>(&_data));
		}

		template<class LeafData, class InnerData>
		const Node<LeafData, InnerData> &Node<LeafData, InnerData>::getNode (const InnerData &_data)
		{
			return getNode (reinterpret_cast<const char *>(&_data));
		}

		template<class LeafData, class InnerData>
		Node<LeafData, InnerData> &Node<LeafData, InnerData>::getNode (LeafData &_data)
		{
			// Casting away constness is safe since _data is non-const
			return const_cast<Node &>(getNode (reinterpret_cast<const char *>(&_data)));
		}

		template<class LeafData, class InnerData>
		Node<LeafData, InnerData> &Node<LeafData, InnerData>::getNode (InnerData &_data)
		{
			// Casting away constness is safe since _data is non-const
			return const_cast<Node &>(getNode (reinterpret_cast<const char *>(&_data)));
		}

		template<class LeafData, class InnerData>
		Node<LeafData, InnerData>::Node (const LeafData &_data)
		{
			setLeaf (_data);
		}

		template<class LeafData, class InnerData>
		Node<LeafData, InnerData>::Node (const InnerData &_data, Node &_left, Node &_right)
		{
			setInner (_data, _left, _right);
		}

		template<class LeafData, class InnerData>
		bool Node<LeafData, InnerData>::isLeaf () const
		{
			return m_leaf;
		}

		template<class LeafData, class InnerData>
		const Node<LeafData, InnerData> &Node<LeafData, InnerData>::getLeft () const
		{
			assert (!isLeaf ());
			return *m_data.innerData.left;
		}

		template<class LeafData, class InnerData>
		const Node<LeafData, InnerData> &Node<LeafData, InnerData>::getRight () const
		{
			assert (!isLeaf ());
			return *m_data.innerData.right;
		}

		template<class LeafData, class InnerData>
		const LeafData &Node<LeafData, InnerData>::getLeafData () const
		{
			assert (isLeaf ());
			return m_data.leafData;
		}

		template<class LeafData, class InnerData>
		const InnerData &Node<LeafData, InnerData>::getInnerData () const
		{
			assert (!isLeaf ());
			return m_data.innerData.data;
		}

		template<class LeafData, class InnerData>
		Node<LeafData, InnerData> &Node<LeafData, InnerData>::getLeft ()
		{
			assert (!isLeaf ());
			return *m_data.innerData.left;
		}

		template<class LeafData, class InnerData>
		Node<LeafData, InnerData> &Node<LeafData, InnerData>::getRight ()
		{
			assert (!isLeaf ());
			return *m_data.innerData.right;
		}

		template<class LeafData, class InnerData>
		LeafData &Node<LeafData, InnerData>::leafData ()
		{
			assert (isLeaf ());
			return m_data.leafData;
		}

		template<class LeafData, class InnerData>
		InnerData &Node<LeafData, InnerData>::innerData ()
		{
			assert (!isLeaf ());
			return m_data.innerData.data;
		}

		template<class LeafData, class InnerData>
		void Node<LeafData, InnerData>::setInner (const InnerData &_data, Node &_left, Node &_right)
		{
			m_leaf = false;
			m_data.innerData.data = _data;
			m_data.innerData.left = &_left;
			m_data.innerData.right = &_right;
		}

		template<class LeafData, class InnerData>
		void Node<LeafData, InnerData>::setLeaf (const LeafData &_data)
		{
			m_leaf = true;
			m_data.leafData = _data;
		}

		template<class LeafData, class InnerData, class Walker>
		const Node<LeafData, InnerData> &walk (const Node<LeafData, InnerData> &_node, Walker _walker)
		{
			const Node<LeafData, InnerData> *node { &_node };
			while (!node->isLeaf ())
			{
				switch (_walker (node->getInnerData ()))
				{
					case EChild::Left:
						node = &node->getLeft ();
						break;
					case EChild::Right:
						node = &node->getRight ();
						break;
					default:
						assert (false);
				}
			}
			return *node;
		}

		template<class LeafData, class InnerData, class Walker>
		Node<LeafData, InnerData> &walk (Node<LeafData, InnerData> &_node, Walker _walker)
		{
			// Casting away constness is safe since _node is non-const
			return const_cast<Node<LeafData, InnerData> &>(walk (static_cast<const Node<LeafData, InnerData> &> (_node), _walker));
		}

	}

}