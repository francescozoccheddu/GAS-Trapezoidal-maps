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

			static const Node &getNode (const char *data);

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

		template<class LeafData, class InnerData, class Walker>
		inline const Node<LeafData, InnerData> &walk (const Node<LeafData, InnerData> &node, Walker walker);

		template<class LeafData, class InnerData, class Walker>
		inline Node<LeafData, InnerData> &walk (Node<LeafData, InnerData> &node, Walker walker);

	}

}

#include "binary_dag.tpp"
