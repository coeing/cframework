#ifndef CGUI_TOOLTIPSYSTEM_H
#define CGUI_TOOLTIPSYSTEM_H

#include <map>

#include "cfoundation/Utils/Singleton.h"
#include "cfoundation/DateTime/Timer.h"
#include "cfoundation/Math/Vector.h"

#include "cgui/Input/InputEvents.h"

namespace CGui
{
    class GuiObject;
    typedef SharedPtr< GuiObject > GuiObjectPtr;
    typedef SharedPtr< const GuiObject > GuiObjectCPtr;

    class TooltipController;
    typedef SharedPtr< TooltipController > TooltipControllerPtr;    

	class TooltipSystem : public CFoundation::Singleton< TooltipSystem >
	{
	public:
        /// Per frame update
		void                    Update( Float32 _f32TimeDifference );

        /// Sets the tooltip controller for the passed object
        void                    SetTooltipController( GuiObjectCPtr _spObject, TooltipControllerPtr _spTooltipController );

        /// Removes the tooltip controller for the passed object
        void                    RemoveTooltipController( GuiObjectCPtr _spObject );

        /// Returns the tooltip controller for the passed object
        TooltipControllerPtr    GetTooltipController( GuiObjectCPtr _spObject ) const;

	private:
		friend class CFoundation::Singleton< TooltipSystem >;

        typedef std::map< CGui::GuiObjectCPtr, TooltipControllerPtr > ObjectTooltipControllerMap;

        /// Constructor
		TooltipSystem();

        /// Destructor
		~TooltipSystem();

        /// Callback when the pointed object has changed
        void                        OnPointedObjectChanged( GuiObjectPtr _spOldObject, GuiObjectPtr _spNewObject );

        /// Sets the new tooltip controller
        /// _spObject is the object that caused start of the tooltip controller
        void                        SetActiveTooltipController( TooltipControllerPtr _spTooltipController, GuiObjectPtr _spObject );

        /// Tooltip controllers for the gui objects
        ObjectTooltipControllerMap  m_mapTooltipControllers;

        /// Current active tooltip controller
		TooltipControllerPtr        m_spTooltipController;

        /// Current active tooltip object
        CGui::GuiObjectPtr          m_spTooltipObject;

		CFoundation::Timer	            m_clTimer;

        /// Delegate when the pointed object has changed 
        InputEventDelegatePtr       m_spDelegatePointedObjectChanged;
	};
}

#endif
