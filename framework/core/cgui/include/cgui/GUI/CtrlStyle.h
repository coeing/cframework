#ifndef CGUI_CTRLSTYLE_H
#define CGUI_CTRLSTYLE_H

#include "cfoundation/Text/String.h"
#include "cfoundation/Utils/SharedPtr.h"

namespace CFoundation
{
    INCLUDE_SHARED_CLASS( XMLDocument );
    INCLUDE_SHARED_CLASS( XMLElement );
}

namespace CGui
{
	class CtrlStyle
	{
		public:
			CtrlStyle();
			explicit CtrlStyle( const CFoundation::String& sXMLFilename );
            explicit CtrlStyle( CFoundation::XMLElementCPtr _spElement );
			virtual ~CtrlStyle();
            
            const CtrlStyle& operator=( const CtrlStyle& );

			CFoundation::XMLElementCPtr     GetRoot() const;

		private:
            CFoundation::XMLDocumentCPtr    m_spDocument;
            CFoundation::XMLElementCPtr     m_spElement;
	};
}

#endif
