#ifndef COFILES_NODE_H
#define COFILES_NODE_H

#include "cofiles/String.h"

namespace CoFiles
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
		BasicNode( const CoFiles::String &rName );
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

		void SetName( const CoFiles::String &sName );
		const CoFiles::String& GetName() const;

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

		NodeType* FindNode( const CoFiles::String &rName ) const;

	private:
		CoFiles::String	m_sName;

		NodeType* m_pParent;
		NodeType* m_pPrev;
		NodeType* m_pNext;
		NodeType* m_pFirstChild;
	};

	class Node : public BasicNode<Node>
	{
	public:
		Node( const CoFiles::String &rName )
		:	BasicNode<Node>( rName ) {};
	};
}

#include "Node.inl"

#endif
