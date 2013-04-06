#ifndef CGUI_DRAGDATA_H
#define CGUI_DRAGDATA_H

#include "cfoundation/Utils/SharedPtr.h"

namespace CGui
{
    /// Interface of data for a drag operation
    class DragData
    {
    public:
        virtual ~DragData() = 0
        {
        }
    };
    typedef SharedPtr< DragData > DragDataPtr;
}

#endif
