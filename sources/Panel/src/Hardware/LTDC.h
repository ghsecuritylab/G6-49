#pragma once
#include "Display/Colors.h"



namespace LTDC_
{
    void Init(uint frontBuffer, uint backBuffer);

    void SetColors(uint *clut, uint8 numColors);

    void ToggleBuffers();

    void FillRegion(int x, int y, int width, int height, Color color);
};
