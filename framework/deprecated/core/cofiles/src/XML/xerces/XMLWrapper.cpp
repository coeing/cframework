#include "cofiles/XML/Xerces/XMLWrapper.h"

#include "cofiles/Makros.h"

using namespace CoFiles;
using namespace Xerces;
XERCES_CPP_NAMESPACE_USE

XMLWrapper::XMLWrapper()
:	m_pNode( NULL )
{
}

const DOMNode* XMLWrapper::GetXMLNode() const
{
	return m_pNode;
}

DOMNode* XMLWrapper::GetXMLNode()
{
	return m_pNode;
}

void XMLWrapper::SetXMLNode( DOMNode* pNode )
{
	m_pNode = pNode;
}
