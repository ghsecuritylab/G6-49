#pragma once
#include "defines.h"
#include "Keyboard/Controls.h"
#include "Settings/SettingsTypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Display
{
public:
    static void Init();

    static void Update();

    static void ToggleBuffers();

    static uint8 *GetBuffer();

    static void SetButton(Control control);

    static void AddStringToConsole(char *string);

    static void SetColorBackground();

private:
    static void DrawSignal(Channel chan);

    static void DrawSignalUGO(Channel chan, int y0);

    static void DrawSignalParameters(Channel chan, int y0);

    static void DrawParameterValue(Channel chan, WaveParameter parameter, int x, int y);

    static void DrawConsole();

    static const char *text;

    static const int STRING_IN_CONSOLE = 29;

    static const int SYMBOLS_IN_STRING = 64;

    static char bufferConsole[STRING_IN_CONSOLE][SYMBOLS_IN_STRING];

    static LTDC_HandleTypeDef hltdc;
    // �����
    static uint8 frontBuffer[320 * 240];
    // ������ �����. � ��� ���������� ���������, � ����� ����������� ���������� �� frontBuffer
    static uint8 backBuffer[320 * 240];
};
