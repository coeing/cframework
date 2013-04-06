#ifndef CFOUNDATION_XERCES_XMLWRAPPER_H
#define CFOUNDATION_XERCES_XMLWRAPPER_H

#ifdef XML_IMPL_XERCES

#include <xercesc/dom/DOMNode.hpp>

namespace CFoundation
{
    namespace Xerces
    {
        class XMLWrapper
        {
        public:
            /// Constructor
            XMLWrapper();

		    XERCES_CPP_NAMESPACE_QUALIFIER DOMNode*         GetXMLNode();
		    const XERCES_CPP_NAMESPACE_QUALIFIER DOMNode*   GetXMLNode() const;
		    void                                            SetXMLNode( XERCES_CPP_NAMESPACE_QUALIFIER DOMNode* _pNode );
            
		private:
            //! Xerces node
			XERCES_CPP_NAMESPACE_QUALIFIER DOMNode* m_pNode;
        };
    }
}

#endif

#endif
