#pragma once
#include "common/Common.h"
#include "common/CommonTypes.h"


struct Calibrator
{
    // ������������� ����������� ����������, ������������ ������������ ������������ ��� ����� ������������ ���������
    static void SetK(uint8 channel, uint8 signal, uint8 range, uint8 param, int16 k);
    /// ���������� ����������� ���������� ���������
    static float GetAmplitudeK(Chan::E ch);
    /// ���������� ������������� ����������� �������� ��� 0�
    static float GetOffsetK_Zero(Chan::E ch);
    /// ���������� ������������� ����������� �������� ��� -5�
    static float GetOffsetK_Negative(Chan::E ch);
    /// ���������� ������������� ����������� �������� ��� +5�
    static float GetOffsetK_Positive(Chan::E ch);
    /// ���������� ������������� ����������� ������ ������������� �����������
    static float GetFreqMeterK_Trig();

private:
    static uint8 CalculateRange(Chan::E ch);
    /// ������������� ����� ������� ��� �������������� ����� �������
    static void SetFormWave(Chan::E ch, uint8 signal);
    /// ���������� ��������� ��� ��������� ���������
    static void SetAmplitude(Chan::E ch);
    /// ���������� �������� ��� ��������� ���������
    static void SetOffset(Chan::E ch, uint8 param);
    /// ���� true, �� �� ��������� � ������ ����������, � �������� ����� �� ������������, ������ �� �������������� ��������, � ����� ���, ������� ������ �����������
    static bool inModeCalibration;
    /// ��� ����� ��������� ������ ���������� ����������
    static uint8 range[Chan::Count];
    /// ��� ���� ����� ������� ���������� ����������. 0 - �����, 1 - ������������
    static uint8 signal[Chan::Count];
};
