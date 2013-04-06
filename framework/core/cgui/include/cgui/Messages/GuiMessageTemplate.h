#ifndef CGUI_GUIMESSAGETEMPLATE_H
#define CGUI_GUIMESSAGETEMPLATE_H

#include "cfoundation/Events/Message.h"

#include "cgui/GUI/GuiObject.h"

namespace CGui
{
    //! Message which is send for a special object
    template < class T >
	class GuiMessageTemplate : public CFoundation::MessageTemplate< T >
	{
	public:
        //! Constructor
		GuiMessageTemplate( GuiObjectPtr _spObject )
        :   m_spObject( _spObject )
        {
        }

        //! Copy constructor
        GuiMessageTemplate( const GuiMessageTemplate& _rhs )
        :   CFoundation::MessageTemplate< T >( _rhs ),
            m_spObject( _rhs.m_spObject )
        {
        }

        //! Destructor
        virtual ~GuiMessageTemplate() = 0
        {
        }

        //! Returns the object which this message is for
        GuiObjectPtr    GetObject() const
        {
            return m_spObject;
        }

	private:
        //! Assign operator
        const GuiMessageTemplate& operator=( const GuiMessageTemplate& );

        //! Object this message is for
        GuiObjectPtr    m_spObject;
	};
}

#endif
