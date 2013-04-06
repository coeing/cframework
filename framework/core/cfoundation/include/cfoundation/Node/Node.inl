namespace CFoundation
{
	template <class NodeType>
	BasicNode<NodeType>::BasicNode( const String &rName )
	:	m_sName( rName ),
		m_pParent(0),
		m_pPrev(0),
		m_pNext(0),
		m_pFirstChild(0)
	{
	}

	template <class NodeType>
	void BasicNode<NodeType>::SetName( const String &sName )
	{
		m_sName = sName;
	}

	template <class NodeType>
	const CFoundation::String& BasicNode<NodeType>::GetName() const
	{
		return m_sName;
	}

	template <class NodeType>
	void  BasicNode<NodeType>::MoveNode( MoveType type, NodeType& neighbour )
	{
		switch (type)
		{
		case INSERT_BEFORE:
			m_pParent =		neighbour.m_pParent;
			m_pPrev	=		neighbour.m_pPrev;
			m_pNext =		&neighbour;
			break;

		case INSERT_AFTER:
			m_pParent =		neighbour.m_pParent;
			m_pPrev =		&neighbour;
			m_pNext =		neighbour.m_pNext;
			break;

		case INSERT_ASFIRSTCHILD:
			{
				m_pParent =		&neighbour;

				NodeType *pLastNode = static_cast< NodeType* >( this );
				while (pLastNode->m_pNext != 0)
				{
					pLastNode = pLastNode->m_pNext;
					pLastNode->m_pParent = &neighbour;
				}

				NodeType *pFirstNode = static_cast< NodeType* >( this );
				while (pFirstNode->m_pPrev != 0)
				{
					pFirstNode = pFirstNode->m_pPrev;
					pFirstNode->m_pParent = &neighbour;
				}

				pLastNode->m_pNext = neighbour.m_pFirstChild;
				if (neighbour.m_pFirstChild)
					neighbour.m_pFirstChild->m_pPrev = pLastNode;

				m_pParent->m_pFirstChild = pFirstNode;
			}
			break;
		case INSERT_ASLASTCHILD:
			{
				m_pParent =		&neighbour;

				NodeType *pLastNode = static_cast< NodeType* >( this );
				while (pLastNode->m_pNext != 0)
				{
					pLastNode = pLastNode->m_pNext;
					pLastNode->m_pParent = &neighbour;
				}

				NodeType *pFirstNode = static_cast< NodeType* >( this );
				while( pFirstNode->m_pPrev != NULL )
				{
					pFirstNode = pFirstNode->m_pPrev;
					pFirstNode->m_pParent = &neighbour;
				}

				if( neighbour.m_pFirstChild != NULL )
				{
					NodeType *pOldLastNode = static_cast< NodeType* >( neighbour.m_pFirstChild );
					while( pOldLastNode->m_pNext != NULL )
					{
						pOldLastNode = pOldLastNode->m_pNext;
						pOldLastNode->m_pParent = &neighbour;
					}
					pOldLastNode->m_pNext = pFirstNode;
					pFirstNode->m_pPrev = pOldLastNode;
				}
				else
				{
					neighbour.m_pFirstChild = pFirstNode;
				}
			}
			break;
		}

		// connect neighbours
		if (m_pPrev != 0)
		{
			m_pPrev->m_pNext = static_cast< NodeType* >( this );
		}
		if (m_pNext != 0)
		{
			m_pNext->m_pPrev = static_cast< NodeType* >( this );
		}
	}

	template <class NodeType>
	Unsigned32 BasicNode<NodeType>::GetNumChildren() const
	{
		Unsigned32 u32Num = 0;

		NodeType *pNode = GetFirstChildNode();
		while( pNode != NULL )
		{
			++u32Num;
			pNode = pNode->GetNextNode();
		}

		return u32Num;
	}

	template <class NodeType>
	bool BasicNode<NodeType>::RemoveNode( const NodeType &rNode, RemoveType eType )
	{
		NodeType *pNode = GetFirstChildNode();
		bool bRemoved = false;
		while( pNode != NULL )
		{
			// Test if this node should be removed
			if( pNode == &rNode )
			{
				// remove childs from node
				//NodeType *pNodeChild = pNode->GetFirstChildNode();
				//while( pNodeChild != NULL )
				{
					//pNode->RemoveNode( *pNodeChild );
					//pNodeChild = pNode->GetFirstChildNode();
				}

				if( pNode->m_pPrev != NULL )
				{
					pNode->m_pPrev->m_pNext = pNode->m_pNext;
				}
				else	// is first child node
				{
					pNode->m_pParent->m_pFirstChild = pNode->m_pNext;
				}

				if( pNode->m_pNext != NULL )
				{
					pNode->m_pNext->m_pPrev = pNode->m_pPrev;
				}

				pNode->m_pParent = NULL;
				pNode->m_pPrev = NULL;
				pNode->m_pNext = NULL;

				bRemoved = true;
			}

			if( bRemoved && eType == REMOVE_ONCE )
			{
				break;
			}

			pNode = pNode->GetNextNode();
		}

		return bRemoved;
	}

	template <class NodeType>
	NodeType* BasicNode<NodeType>::FindNode( const CFoundation::String &rName ) const
	{
		NodeType *pNode = GetFirstChildNode();
		while( pNode != NULL )
		{
			if( pNode->GetName() == rName )
			{
				return pNode;
			}
			NodeType *pFoundNode = pNode->FindNode( rName );
			if( pFoundNode != NULL )
			{
				return pFoundNode;
			}

			pNode = pNode->GetNextNode();
		}

		return NULL;
	}

	template <class NodeType>
	NodeType* BasicNode<NodeType>::GetParentNode() const
	{
		return m_pParent;
	}

	template <class NodeType>
	NodeType* BasicNode<NodeType>::GetPrevNode() const
	{
		return m_pPrev;
	}

	template <class NodeType>
	NodeType* BasicNode<NodeType>::GetNextNode() const
	{
		return m_pNext;
	}

	template <class NodeType>
	NodeType* BasicNode<NodeType>::GetFirstChildNode() const
	{
		return m_pFirstChild;
	}
}
