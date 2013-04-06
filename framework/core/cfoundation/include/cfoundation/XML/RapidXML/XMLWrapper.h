#ifndef CFoundation_RAPIDXML_XMLWRAPPER_H
#define CFoundation_RAPIDXML_XMLWRAPPER_H

#include "rapidxml/rapidxml.hpp"

namespace CFoundation
{
    namespace RapidXML
    {
        class XMLWrapper
        {
        public:
            /// Constructor
            XMLWrapper();

            rapidxml::xml_node<>*       GetXMLNode();
		    const rapidxml::xml_node<>* GetXMLNode() const;
		    void                        SetXMLNode( rapidxml::xml_node<>* _pNode );
            
		private:
            //! RapidXML node
			rapidxml::xml_node<>*       m_pNode;
        };
    }
}

#endif
