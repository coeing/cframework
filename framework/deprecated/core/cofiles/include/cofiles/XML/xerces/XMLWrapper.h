#ifndef COFILES_XERCES_XMLWRAPPER_H
#define COFILES_XERCES_XMLWRAPPER_H

#include <xercesc/dom/DOMNode.hpp>

namespace CoFiles
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
