#include "defines.h"
#include "FreqMeter.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Interface/Interface.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FreqMeter::Init()
{
    /*
        PE14 - ������������� 1 ���/50 ��
        PE15 - AC/DC
        PB10 - ��� ���/����

        ���������������� � CPU::InitPins()
    */
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::SetResist(FreqResist resist)
{
    CPU::WritePin(FREQ_METER_RESIST, resist.Is(FreqResist::_50Ohm));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::SetCouple(FreqCouple couple)
{
    CPU::WritePin(FREQ_METER_COUPLE, couple.Is(FreqCouple::DC));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::SetFiltr(FreqFiltr filtr)
{
    CPU::WritePin(FREQ_METER_FILTR, filtr.Is(FreqFiltr::Disable));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FreqMeter::Update()
{
    if(CPU::ReadPin(FREQ_METER_DRY))
    {
        uint data = 0;
        for(int i = 30; i >= 0; i--)
        {
            CPU::WritePin(FREQ_METER_CLK, true);
            volatile int j = 0;
            while(j < 25)
            {
                j++;
            }
            CPU::WritePin(FREQ_METER_CLK, false);
            Timer::PauseOnTime(1);
            if (CPU::ReadPin(FREQ_METER_DATA))
            {
                data += (1 << i);
            }
        }

        Interface::SendFrequency(data);
    }
}

