#pragma once
#include "Settings/SettingsTypes.h"


class Wave
{
public:

    struct Graphic
    {
        /// ���������� ���������� � ���������� ������� �� ������
        static void Draw(Chan ch);
        /// ������� x ������� ��������� �������
        static int X();
        /// ������� y ������� ��������� �������
        static int Y(Chan ch);
        /// ������ ������� ��������� �������
        static int Width();
        /// ������ ������� ��������� �������
        static int Height();
    private:
    
        static void DrawSignalUGO(Chan chan, int y0);
    
        static void DrawSignalParameters(Chan chan, int y0);
    
        static void DrawParameterValue(Chan chan, WaveParameter parameter, int x, int y);
    };
};
