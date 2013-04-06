#ifndef CCTRL_GUIRECORDER_H
#define CCTRL_GUIRECORDER_H

#include "cfoundation/Events/MessageReceiver.h"

#include "cgui/Gui/GuiObject.h"

#include "cctrl/GuiRecord.h"

namespace CCtrl
{
	class GuiRecorder : public CFoundation::MessageReceiver
	{
	public:
		GuiRecorder( CGui::GuiObjectPtr _spObject );
		virtual ~GuiRecorder();

		virtual void ProcessMessage( const CFoundation::Message& clMessage );

		void Start();
		void Stop();
		void Continue();

		bool IsActive() const;

		const GuiRecord&	GetCurrentRecord() const;

	private:
		GuiRecorder& operator=( const GuiRecorder& rhs );

		bool			    m_bActive;

		CGui::GuiObjectPtr  m_spObject;

		GuiRecord		    m_clCurRecord;
	};
}

#endif
