#pragma once
#include "Common.h"
#include "Command.h"
#include "defines.h"
#include "GeneratorSettingsTypes.h"
#include "Hardware/CPU.h"
#include "Hardware/HAL/HAL_PIO.h"


struct AD5697
{
    static void Init();

    static void SetOffset(Chan::E ch, FloatValue offset);

    static void SetFreqLevel(float level);

    static void SetFreqHysteresys(float hyst);

private:
    static void Reset(Chan::E ch);
    /// �������� ��� ����� ������ �� ������� ������
    static void WriteParameter(uint8 address, uint8 data[3], HPort::E port, uint16 pin);

    static StructPIN PinLDAC(Chan::E ch);

//    static uint8 CreateCommandByte(ParameterValue param);
};
