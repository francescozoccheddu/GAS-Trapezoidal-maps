#pragma once

#include <cstddef>

namespace GAS
{

	namespace BDAG
	{


		template<class LeafData, class InnerData>
		class Node final
		{

		private:

			union
			{
				LeafData leafData;
				struct
				{
					InnerData data;
					Node *left, *right;
				} innerData;
			} m_data {};

			bool m_leaf;
			bool m_deleted { false };

			static const Node &getNode (const void *data);
			static Node &getNode (void *data);

		public:

			// TODO Partial specialization if LeafData == InnerData

			static void recursiveDelete (Node &node, std::size_t sizeHint = 0);

			static const Node &getNode (const LeafData &data);
			static const Node &getNode (const InnerData &data);
			static Node &getNode (LeafData &data);
			static Node &getNode (InnerData &data);

			Node (const LeafData &data);
			Node (const InnerData &data, Node &left, Node &right);

			bool isLeaf () const;
			const Node &getLeft () const;
			const Node &getRight () const;
			const LeafData &getLeafData () const;
			const InnerData &getInnerData () const;

			Node &getLeft ();
			Node &getRight ();
			LeafData &leafData ();
			InnerData &innerData ();

			void setInner (const InnerData &data, Node &left, Node &right);
			void setLeaf (const LeafData &data);

		};

		enum class EChild
		{
			Left, Right
		};

		template<class LeafData, class InnerData>
		using Walker = EChild (*)(const InnerData & node);

		template<class LeafData, class InnerData>
		const Node<LeafData, InnerData> &walk (const Node<LeafData, InnerData> &node, Walker<LeafData, InnerData> walker);

		template<class LeafData, class InnerData>
		Node<LeafData, InnerData> &walk (Node<LeafData, InnerData> &node, Walker<LeafData, InnerData> walker);

	}

}

#include "binary_dag.tpp"
