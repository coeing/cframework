#ifndef COFILES_RAPIDXML_XMLWRAPPER_H
#define COFILES_RAPIDXML_XMLWRAPPER_H

#include "rapidxml/rapidxml.hpp"

namespace CoFiles
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
