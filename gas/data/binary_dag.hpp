#pragma once

#include <gas/utils/intrusive_list_iterator.hpp>

namespace GAS
{

	namespace BDAG
	{

		template<class Data>
		class Graph;

		template<class Data>
		class Node final
		{

			friend class Graph<Data>;

			Data m_data {};
			Node *m_previous {}, *m_next {}; // Intrusive linked list pointers
			bool m_leaf { true };
			Node *m_left {}, *m_right {};

			Node () = default;
			explicit Node (const Data &data);
			explicit Node (Data &&data);

			~Node () = default;

			Node (const Node &) = delete;
			Node (const Node &&) = delete;

			Node &operator=(const Node &) = delete;
			Node &operator=(Node &&) = delete;

		public:

			static const Node &from (const Data &data);
			static Node &from (Data &data);

			bool isLeaf () const;
			const Data &data () const;
			const Data &operator *() const;
			const Data *operator ->() const;
			const Node &left () const;
			const Node &right () const;

			Data &data ();
			Data &operator *();
			Data *operator ->();
			Node &left ();
			Node &right ();

		};

		template<class Data>
		class Graph
		{
			using Node = Node<Data>;

			Node *m_firstNode {}, *m_lastNode {}, *m_firstLeafNode {}, *m_lastLeafNode {};
			int m_nodesCount {}, m_leafNodesCount {};

			void registerNode (Node &node);
			void registerLeaf (Node &node);
			void unregisterNode (Node &node);
			void unregisterLeaf (Node &node);

		public:

			using ConstNodeIterator = Utils::IntrusiveListIterator<const Node, &Node::m_previous, &Node::m_next>;
			using NodeIterator = Utils::IntrusiveListIterator<Node, &Node::m_previous, &Node::m_next>;
			using ConstLeafNodeIterator = Utils::IntrusiveListIterator<const Node, &Node::m_left, &Node::m_right>;
			using LeafNodeIterator = Utils::IntrusiveListIterator<Node, &Node::m_left, &Node::m_right>;

			Graph () = default;
			Graph (const Graph &copy);
			Graph (Graph &&moved);

			~Graph ();

			Graph &operator =(const Graph &copy);
			Graph &operator =(Graph &&moved);

			bool isEmpty () const;
			int nodesCount () const;
			int leafNodesCount () const;
			int innerNodesCount () const;

			Node &createLeaf ();
			Node &createLeaf (const Data &data);
			Node &createLeaf (Data &&data);
			Node &createInner (Node &left, Node &right);
			Node &createInner (const Data &data, Node &left, Node &right);
			Node &createInner (Data &&data, Node &left, Node &right);

			void setLeaf (Node &node);
			void setInner (Node &node, Node &left, Node &right);

			void destroyNode (Node &node);

			void clear ();

			typename ConstNodeIterator::Iterable nodes () const;
			typename NodeIterator::Iterable nodes ();
			typename ConstLeafNodeIterator::Iterable leafNodes () const;
			typename LeafNodeIterator::Iterable leafNodes ();

		};

		enum class EChild
		{
			Left, Right
		};

		template<class Data, class Walker>
		const Node<Data> &walk (const Node<Data> &root, Walker walker);

		template<class Data, class Walker>
		Node<Data> &walk (Node<Data> &root, Walker walker);

	}

}


#include "binary_dag.tpp"