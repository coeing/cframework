#ifndef CCTRL_APP_H
#define CCTRL_APP_H

#include "cfoundation/Events/MessageReceiver.h"
#include "cfoundation/Text/String.h"
#include "cfoundation/DateTime/Timer.h"
#include "cfoundation/Math/Vector.h"
#include "cfoundation/Serialization/Serializable.h"

#include "cctrl/WindowController.h"

namespace CGui
{
    INCLUDE_SHARED_CLASS( GuiStyle );
}

namespace CCtrl
{
	class App : public CFoundation::MessageReceiver, public CFoundation::Serializable
	{
	public:
        /// Initializes and starts the application
		void            Start();

		virtual void    ProcessMessage( const CFoundation::Message &clMessage );

		Float32	        GetFPS() const;

		virtual void    Save( CFoundation::Serializer &clSerializer ) const;
		virtual void    Load( const CFoundation::Serializer &clSerializer );

		void            SaveFile( const CFoundation::String &sFileName ) const;
		void            LoadFile( const CFoundation::String &sFileName );

        /// Quits the application
		virtual void    Exit();

	protected:
		App();
		virtual ~App() = 0;

        /// Initialization
		virtual bool                        Init();

        /// Deinitialization
		virtual void                        Deinit();

        /// Returns the caption of the render window
		virtual const CFoundation::String   GetWindowCaption() const;

        /// Returns the width of the render window
		virtual Unsigned32                  GetWidth() const;
        
        /// Returns the height of the render window
		virtual Unsigned32                  GetHeight() const;
        
        /// Returns the application dimensions
        virtual CFoundation::Vector2Du      GetDimensions() const;

        /// Creates the start windows
		virtual void                        CreateStartWindows() = 0;

        /// Returns the default gui style
        virtual CGui::GuiStylePtr           GetDefaultStyle() const;

        /// Per frame update
        virtual void                        Update( Float32 _f32TimeDifference );

        /// Called when application should be exited
        virtual void                        OnExit();

	private:
		bool				m_bDone;
		CFoundation::Timer  m_clFPSTimer;
		Unsigned32			m_u32NumFPS;
		Float32				m_f32FPS;
	};
}

#endif
