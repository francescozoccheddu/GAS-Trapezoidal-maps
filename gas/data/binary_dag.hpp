/// Binary directed acyclic graph data structures and utils.
/// \file
/// \author Francesco Zoccheddu

#ifndef GAS_DATA_BINARY_DAG_INCLUDED
#define GAS_DATA_BINARY_DAG_INCLUDED

#include <gas/utils/intrusive_list_iterator.hpp>

namespace GAS
{

	/// Binary directed acyclic graph data structures and utils.
	namespace BDAG
	{

		template<class Data>
		class Graph;

		/// Binary directed acyclic graph node.
		/// Can be a leaf or an inner node with two valid children nodes, the \e left child and the \e right child.
		/// Can only be created by a Graph object.
		/// Should only be used with the Graph object that created it.
		/// Cannot be copied or assigned and should only be used through pointers or references.
		/// \tparam Data 
		/// The user data type.
		template<class Data>
		class Node final
		{

			friend class Graph<Data>;

			Data m_data {};
			/// Intrusive linked list pointers for the other nodes in the parent Graph. 
			Node *m_previous {}, *m_next {};
			bool m_leaf { true };
			/// Pointers to the children nodes or intrusive linked list pointers to the other leaf nodes in the parent Graph if the node is a leaf.
			Node *m_left {}, *m_right {};

			/// Construct a leaf node.
			/// The user data is initialized by calling its default constructor.
			/// \pre
			/// \p Data type must be default constructible.
			Node () = default;

			/// Construct a leaf node with \p data as the user data.
			/// The user data is copied by calling its copy constructor.
			/// \pre
			/// \p Data type must be copy constructible.
			explicit Node (const Data &data);

			/// Construct a leaf node with \p data as the user data.
			/// The user data is moved by calling its move constructor.
			/// \pre
			/// \p Data type must be move constructible.
			explicit Node (Data &&data);

			~Node () = default;

			Node (const Node &) = delete;
			Node (const Node &&) = delete;

			Node &operator=(const Node &) = delete;
			Node &operator=(Node &&) = delete;

		public:

			/// Get the node containing the specified user data \p data.
			/// \pre 
			/// \p Data type must be a standard layout type.
			/// \param[in] data
			/// An object returned by one of the data() methods.
			/// \return
			/// The node holding \p data.
			static const Node &from (const Data &data);

			/// \copydoc from 
			static Node &from (Data &data);

			/// \return 
			/// \c true if the node is a leaf, \c false otherwise.
			bool isLeaf () const;

			/// \return
			/// The user data.
			const Data &data () const;

			/// \copydoc data
			const Data &operator *() const;

			/// \copydoc data
			const Data *operator ->() const;

			/// \copydoc data
			Data &data ();

			/// \copydoc data
			Data &operator *();

			/// \copydoc data
			Data *operator ->();

			/// \pre 
			/// The node must be an inner node.
			/// \return 
			/// The left children node.
			const Node &left () const;

			/// \copydoc left
			Node &left ();

			/// \pre
			/// The node must be an inner node.
			/// \return
			/// The right children node.
			const Node &right () const;

			/// \copydoc right
			Node &right ();

		};

		/// Binary directed acyclic graph.
		/// Allows to create and modify leaf and inner Node and keeps track of them, 
		/// providing easy copy, move, deletion and iteration through all nodes or through leaves.
		/// \tparam Data 
		/// The data type to store in the nodes.
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

			/// Active nodes iterator.
			using ConstNodeIterator = Utils::IntrusiveListIterator<const Node, &Node::m_previous, &Node::m_next>;

			/// \copydoc ConstNodeIterator
			using NodeIterator = Utils::IntrusiveListIterator<Node, &Node::m_previous, &Node::m_next>;

			/// Active leaf nodes iterator.
			using ConstLeafNodeIterator = Utils::IntrusiveListIterator<const Node, &Node::m_left, &Node::m_right>;

			/// \copydoc ConstLeafNodeIterator
			using LeafNodeIterator = Utils::IntrusiveListIterator<Node, &Node::m_left, &Node::m_right>;

			/// Construct an empty graph.
			Graph () = default;

			/// Clone an existing graph.
			/// Each node data is copied by calling its copy constructor.
			/// \pre 
			/// \p Data type must be copy constructible.
			/// \param[in] copy 
			/// The graph to clone.
			Graph (const Graph &copy);

			/// Move an existing graph.
			/// Each node data is moved by calling its move constructor.
			/// After calling this constructor, \p moved is empty and valid.
			/// \pre 
			/// \p Data type must be move constructible.
			/// \param[in] moved 
			/// The graph to move and clear.
			Graph (Graph &&moved);

			/// \see clear()
			~Graph ();

			/// Clear the active nodes and clone an existing graph.
			/// Each node data is copied by calling its copy assignment operator.
			/// \pre 
			/// \p Data type must be copy assignable.
			/// \param[in] copy
			/// The graph to clone.
			Graph &operator =(const Graph &copy);

			/// Clear the active nodes and move an existing graph.
			/// Each node data is moved by calling its move assignment operator.
			/// After calling this constructor, \p moved is empty and valid.
			/// \pre 
			/// \p Data type must be move assignable.
			/// \param[in] moved 
			/// The graph to move and clear.
			Graph &operator =(Graph &&moved);

			/// \return 
			/// \c true if the graph is empty, \c false otherwise.
			bool isEmpty () const;

			/// \return 
			/// The number of active (created and not yet destroyed) nodes.
			int nodesCount () const;

			/// \return 
			/// The number of active (created and not yet destroyed) leaf nodes.
			int leafNodesCount () const;

			/// \return 
			/// The number of active (created and not yet destroyed) inner nodes.
			int innerNodesCount () const;

			/// Create a leaf node.
			/// The node data is initialized by calling its default constructor.
			/// \pre 
			/// \p Data type must be default constructible.
			/// \return 
			/// The created node.
			Node &createLeaf ();

			/// Create a leaf node with \p data as the user data.
			/// The node data is initialized by calling its copy constructor.
			/// \pre 
			/// \p Data type must be copy constructible.
			/// \param[in] data
			/// The node data to clone.
			/// \return 
			/// The created node.
			Node &createLeaf (const Data &data);

			/// Create a leaf node with \p data as the user data.
			/// The node data is initialized by calling its move constructor.
			/// \pre 
			/// \p Data type must be move constructible.
			/// \param[in] data
			/// The node data to move.
			/// \return 
			/// The created node.
			Node &createLeaf (Data &&data);

			/// Create an inner node.
			/// The node data is initialized by calling its default constructor.
			/// \pre 
			/// \p Data type must be default constructible.
			/// \pre 
			/// \p left and \p right must have been created by this graph.
			/// \param[in] left 
			/// The left children node.
			/// \param[in] right 
			/// The right children node.
			/// \return 
			/// The created node.
			Node &createInner (Node &left, Node &right);

			/// Create an inner node with \p data as the user data.
			/// The node data is initialized by calling its copy constructor.
			/// \pre 
			/// \p Data type must be copy constructible.
			/// \pre 
			/// \p left and \p right must have been created by this graph.
			/// \param[in] data 
			/// The node data to clone.
			/// \param[in] left 
			/// The left children node.
			/// \param[in] right 
			/// The right children node.
			/// \return 
			/// The created node.
			Node &createInner (const Data &data, Node &left, Node &right);

			/// Create an inner node with \p data as the user data.
			/// The node data is initialized by calling its move constructor.
			/// \pre 
			/// \p Data type must be move constructible.
			/// \pre 
			/// \p left and \p right must have been created by this graph.
			/// \param[in] data 
			/// The node data to move.
			/// \param[in] left 
			/// The left children node.
			/// \param[in] right 
			/// The right children node.
			/// \return 
			/// The created node.
			Node &createInner (Data &&data, Node &left, Node &right);

			/// Make \p node a leaf.
			/// \remark 
			/// Eventual \p node children will \e not be destroyed.
			/// \param[in] node 
			/// The node to set.
			void setLeaf (Node &node);

			/// Make \p node an inner node.
			/// \pre 
			/// \p left and \p right must have been created by this graph.
			/// \param[in] left 
			/// The left children node.
			/// \param[in] right 
			/// The right children node.
			/// \param[in] node 
			/// The node to set.
			void setInner (Node &node, Node &left, Node &right);

			/// Delete the node \p node making it unusable.
			/// \remark 
			/// Eventual inner nodes having \p node as children will \e not be automatically updated.
			/// \param[in] node 
			/// The node to destroy.
			void destroyNode (Node &node);

			/// Deletes all the active nodes.
			/// \remark
			/// All the references to nodes created by this graph will be invalidated.
			void clear ();

			/// \remark 
			/// The iteration follows the order of creation of the nodes.
			/// \return 
			/// An object that allows to iterate through all the active nodes.
			typename ConstNodeIterator::Iterable nodes () const;

			/// \copydoc nodes
			typename NodeIterator::Iterable nodes ();

			/// \remark 
			/// The iteration follows the order in which the nodes were created or became leaves for the last time.
			/// \return 
			/// An object that allows to iterate through all the active leaf nodes.
			typename ConstLeafNodeIterator::Iterable leafNodes () const;

			/// \copydoc leafNodes
			typename LeafNodeIterator::Iterable leafNodes ();

		};

		/// Left or right children of a Node.
		enum class EChild
		{
			Left,	///< Left children.
			Right  	///< Right children. 
		};

		/// Convenience function for walking from \p root to some leaf.
		/// \tparam Data
		/// The Node data type.
		/// \tparam Walker 
		/// Any type that can be called with a \p Data argument and returns an #EChild.
		/// \param[in] root 
		/// The starting Node.
		/// \param[in] walker 
		/// A callable object that decides whether to continue walking through the left or the right child.
		/// \return 
		/// The reached leaf Node.
		template<class Data, class Walker>
		const Node<Data> &walk (const Node<Data> &root, Walker walker);

		/// \copydoc walk
		template<class Data, class Walker>
		Node<Data> &walk (Node<Data> &root, Walker walker);

	}

}


#include "binary_dag.tpp"

#endif