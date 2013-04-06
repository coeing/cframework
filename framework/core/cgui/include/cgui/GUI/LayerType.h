#ifndef CGUI_LAYERTYPE_H
#define CGUI_LAYERTYPE_H

namespace CGui
{
    enum LayerType
    {
        LAYER_BG,
        LAYER_MAIN,
        LAYER_FG,

        LAYER_HOVER,        //< Objects in this layer can't be pointed, clicked, etc. (e.g. currently dragged objects or tooltips)

        LAYER_END
    };
}

#endif
