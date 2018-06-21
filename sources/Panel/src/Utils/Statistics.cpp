#include "Statistics.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"
#include "Utils/StringUtils.h"
#include <string.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int  Statistics::timeFrame = 0;
int  Statistics::timeAllFrames = 0;
uint Statistics::timeStartFrames = 0;
uint Statistics::timeAccumFrames = 0;
uint Statistics::timeStart = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Statistics::BeginFrame()
{
    timeStart = TIME_MS;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Statistics::EndFrame()
{
    timeFrame = (int)(TIME_MS - timeStart);

    timeAccumFrames += timeFrame;

    if (TIME_MS >= timeStartFrames + 1000)
    {
        timeAllFrames = (int)timeAccumFrames;
        timeStartFrames = TIME_MS;
        timeAccumFrames = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Statistics::Show()
{
    if (SHOW_STATISTICS)
    {
        Painter::DrawFilledRectangle(0, SCREEN_HEIGHT - 12, 60, 10, Color::BACK, Color::FILL);
        char buffer[100] = {0};
        strcpy(buffer, Int2String((int)(CPU::GetFPS() + 0.5f), false, 3));
        strcat(buffer, "/");
        strcat(buffer, Int2String(timeFrame, false, 3));
        strcat(buffer, "/");
        strcat(buffer, Int2String(timeAllFrames, false, 3));
        Text::DrawText(2, SCREEN_HEIGHT - 11, buffer, Color::FILL);
    }
}