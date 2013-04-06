#ifndef CGUI_PROGRESS_BAR_H
#define CGUI_PROGRESS_BAR_H

#include "cgui/Gui/CombinedCtrl.h"

namespace CGui
{
    INCLUDE_SHARED_CLASS( ProgressBar );

    class ProgressBar : public CombinedCtrl
	{
	public:
        /// Destructor
        virtual ~ProgressBar();

        /// Returns the type of this class
        static const CFoundation::String    GetTypeID();

        /// Returns the type of this object
		virtual const CFoundation::String   GetTypeId() const;

        /// Sets the ratio
		void                                SetRatio( Float16 _f16Ratio );

        /// Returns the ratio
		Float16                             GetRatio() const;

	private:
		friend class ProgressBarCreator;

        /// Constructor
		ProgressBar( const CFoundation::String& _strName );

        /// Sets the bar
        void                                SetBar( GuiObjectPtr _spObjBar );

        /// Updates the initial size of the bar
        void                                UpdateInitialSize();

        /// Updates the bar position and size
        void                                UpdateBar();

        /// Bar
        GuiObjectPtr            m_spObjBar;

        /// Initial bar size
        CFoundation::Vector2Du  m_vInitialSize;

        /// Ratio
		Float16			        m_f16Ratio;
	};

	class ProgressBarCreator : public CombinedCtrlCreator
	{
	public:
        /// Returns the type id of the objects that are created with this creator
		virtual const CFoundation::String GetTypeId() const;

        /// Creates an object with the passed name
		virtual GuiObjectPtr    CreateObject( const CFoundation::String& _strName ) const;

        /// Does the default initialization
		virtual void            InitDefault( GuiObjectPtr _spObject ) const;

        /// Initializes the passed object from the passed xml element
		virtual void            InitFromXML( GuiObjectPtr _spObject, CFoundation::XMLElementCPtr _spXMLElement ) const;

        /// Initializes an object from another
		virtual void            InitFromTemplate( GuiObjectPtr _spObject, GuiObjectCPtr _spTemplate ) const;
	};
}

#endif
