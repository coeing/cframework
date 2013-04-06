#ifndef CFOUNDATION_OBJECTNODE_H
#define CFOUNDATION_OBJECTNODE_H

#include "cfoundation/Node/Node.h"

namespace CFoundation
{
	template < class T, class NodeType >
	class ObjectNode : public BasicNode< NodeType >
	{
	public:
		ObjectNode( const CFoundation::String &rName, T *pObject = 0 )
		:	BasicNode<NodeType>( rName ),
			m_spObject( pObject )
		{
		}

		virtual ~ObjectNode()
		{
		}

		virtual void SetObject( T* pObject )
		{
			m_spObject = pObject;
		}
		
		T* GetObject()
		{
			return m_spObject;
		}
		
		const T* GetObject() const
		{
			return m_spObject;
		}

		bool HasObject() const
		{
			return m_spObject != NULL;
		}

	private:
		T*      m_spObject;
	};
};

#include "ObjectNode.inl"

#endif
