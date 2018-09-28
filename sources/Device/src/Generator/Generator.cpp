#include "AD5697.h"
#include "FPGA.h"
#include "Generator.h"
#include "Multiplexor.h"
#include "GeneratorSettingsTypes.h"
#include "Hardware/CPU.h"
#include "FreqMeter/FreqMeter.h"
#include "CommonTypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool waveIsSine = true;          // ����� ��� ����, ����� ������ ������� � ���������� ����� - ���� ��� AD9952


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Generator::Init()
{
    EnableChannel(Chan::A, false);
    EnableChannel(Chan::B, false);
    AD9952::Init();
    AD5697::Init();
    FPGA::Init();
    Multiplexor::Init();
    FreqMeter::Init();

    /*
    Generator::EnableChannel(Chan::A, true);
    Generator::SetOffset(Chan::A, 0.0f);
    Generator::SetAmplitude(Chan::A, 10.0f);
    Generator::SetFormWave(Chan::A, Form::PacketImpuls);
    FPGA::PacketImpulse::SetPeriodImpulse(4e-8f);
    FPGA::PacketImpulse::SetDurationImpulse(2e-8f);
    FPGA::PacketImpulse::SetNumberImpules(50000);
    FPGA::PacketImpulse::SetPeriodPacket(2.5e-3f);
    */
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::EnableChannel(Chan ch, bool enable)
{
    if(!FPGA::Start())
    {
        static const GeneratorWritePin pin[Chan::Number] = { GeneratorWritePin::Pin_P3_OutA, GeneratorWritePin::Pin_P4_OutB };

        CPU::WritePin(pin[ch], !enable);
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetFormWave(Chan ch, Form form)
{
    if(ch.value < Chan::Number && form.value < Form::Number)
    {
        waveIsSine = form.Is(Form::Sine);

        if (!waveIsSine)
        {
            FPGA::SetWaveForm(ch, form);
        }

        Multiplexor::SetMode(ch, form);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetParameter(Chan ch, CommandPanel command, float value)
{
    static const struct StructFunc
    {
        typedef void (*pFuncChF)(Chan, float);
        pFuncChF func;
        StructFunc(pFuncChF f) : func(f) {};
    }
    func[CommandPanel::Number] =
    {
        EmptyFunc,
        EmptyFunc,
        EmptyFunc,
        SetFrequency,
        SetAmplitude,
        SetOffset,
        SetDuration,
        SetDutyRatio,
        SetPhase,
        EmptyFunc,
        EmptyFunc,
        SetDelay,
        EmptyFunc,
        EmptyFunc,
        EmptyFunc,
        EmptyFunc,
        EmptyFunc,
        EmptyFunc,
        EmptyFunc,
        EmptyFunc,
        SetPacketPeriod,
        SetPacketNumber,
        EmptyFunc,
        SetPeriod
    };

    func[command].func(ch, value);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetFrequency(Chan ch, float frequency)
{
    if (waveIsSine)
    {
        AD9952::SetFrequency(ch, frequency);
    }
    else
    {
        FPGA::SetFrequency(ch, frequency);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetPeriod(Chan ch, float period)
{
    FPGA::SetPeriodImpulse(ch, period);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetAmplitude(Chan ch, float amplitude)
{
    if (waveIsSine)
    {
        AD9952::SetAmplitude(ch, amplitude);
    }
    else
    {
        FPGA::SetAmplitude(ch, amplitude);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetOffset(Chan ch, float offset)
{
    if(waveIsSine)
    {
        AD5697::SetOffset(ch, offset);
    }
    else
    {
        FPGA::SetOffset(ch, offset);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetPhase(Chan ch, float phase)
{
    AD9952::SetPhase(ch, phase);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetPacketNumber(Chan, float)
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetPacketPeriod(Chan, float)
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetDutyRatio(Chan, float)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetDuration(Chan ch, float value)
{
    FPGA::SetDurationImpulse(ch, value);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetDelay(Chan, float)
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::EmptyFunc(Chan, float)
{
}
