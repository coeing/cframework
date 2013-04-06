#ifndef CFOUNDATION_NODE_H
#define CFOUNDATION_NODE_H

#include "cfoundation/Text/String.h"

namespace CFoundation
{
	enum MoveType
	{
		INSERT_BEFORE,
		INSERT_AFTER,
		INSERT_ASFIRSTCHILD,
		INSERT_ASLASTCHILD
	};
	enum RemoveType
	{
		REMOVE_ONCE,
		REMOVE_ALL
	};

	template <class NodeType>
	class BasicNode
	{
	public:
		BasicNode( const CFoundation::String &rName );
		virtual ~BasicNode()
		{
			// delete childs
			NodeType *pNode = GetFirstChildNode();

			while( pNode != NULL )
			{
				NodeType *pTmpNode = pNode;

				pNode = pNode->GetNextNode();

				delete pTmpNode;
			}
		}

		void SetName( const CFoundation::String &sName );
		const CFoundation::String& GetName() const;

		virtual NodeType* GetParentNode() const;
		virtual NodeType* GetPrevNode() const;
		virtual NodeType* GetNextNode() const;
		virtual NodeType* GetFirstChildNode() const;

		virtual Unsigned32 GetNumChildren() const;

		virtual void  MoveNode( MoveType type, NodeType& neighbour );

		/*! Removes an underlying Node
		 *
		 *  \param rNode	Node to remove
		 *	\param eType	Method to remove ( once or all )
		 *
		 *	\return			Was a Node removed?
		 *
		 */
		virtual bool  RemoveNode( const NodeType &rNode, RemoveType eType = REMOVE_ONCE );

		NodeType* FindNode( const CFoundation::String &rName ) const;

	private:
		CFoundation::String	m_sName;

		NodeType* m_pParent;
		NodeType* m_pPrev;
		NodeType* m_pNext;
		NodeType* m_pFirstChild;
	};

	class Node : public BasicNode<Node>
	{
	public:
		Node( const CFoundation::String &rName )
		:	BasicNode<Node>( rName ) {};
	};
}

#include "Node.inl"

#endif
