#include "cgui/GuiControls.h"

#include "cgui/Gui/GuiMain.h"

#include "cgui/Gui/Border.h"
#include "cgui/Gui/Button.h"
#include "cgui/Gui/CheckBox.h"
#include "cgui/Gui/ComboBox.h"
#include "cgui/Gui/EditBox.h"
#include "cgui/Gui/ListBox.h"
#include "cgui/Gui/ListBoxColumn.h"
#include "cgui/Gui/ListBoxCell.h"
#include "cgui/Gui/ProgressBar.h"
#include "cgui/Gui/ScaleImage.h"
#include "cgui/Gui/SimpleBorder.h"
#include "cgui/Gui/ScrollBar.h"
#include "cgui/Gui/ScrollPanel.h"
#include "cgui/Gui/Tree.h"
#include "cgui/Gui/TreeNode.h"
#include "cgui/Gui/Window.h"

#define REGISTER_OBJECT_CREATOR( Creator )  { GuiObjectCreatorPtr spCreator( new Creator() ); \
                                              CGui::GuiMain::GetInstance().RegisterObjectCreator( spCreator->GetTypeId(), spCreator ); }

bool CGui::InitControls()
{
    REGISTER_OBJECT_CREATOR( ButtonCreator );
    REGISTER_OBJECT_CREATOR( CheckBoxCreator );
    REGISTER_OBJECT_CREATOR( ComboBoxCreator );
    REGISTER_OBJECT_CREATOR( EditBoxCreator );
    REGISTER_OBJECT_CREATOR( ListBoxCreator );
    REGISTER_OBJECT_CREATOR( ListBoxColumnCreator );
    REGISTER_OBJECT_CREATOR( ListBoxCellCreator );
    REGISTER_OBJECT_CREATOR( ProgressBarCreator );
    REGISTER_OBJECT_CREATOR( ScaleImageCreator );
    REGISTER_OBJECT_CREATOR( ScrollBarCreator );
    REGISTER_OBJECT_CREATOR( ScrollPanelCreator );
    REGISTER_OBJECT_CREATOR( SimpleBorderCreator );
    REGISTER_OBJECT_CREATOR( TreeCreator );
    REGISTER_OBJECT_CREATOR( WindowCreator );

    return true;
}
