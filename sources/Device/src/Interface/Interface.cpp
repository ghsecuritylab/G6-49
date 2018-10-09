#include "defines.h"
#include "Log.h"
#include "Interface.h"
#include "Utils/Console.h"
#include "Generator/Generator.h"
#include "Generator/FPGA.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "FreqMeter/FreqMeter.h"
#include "Utils/Debug.h"
#include "Utils/StringUtils.h"
#include "Command.h"
#include "structs.h"
#include <string.h>
#include <stdlib.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static SPI_HandleTypeDef hSPI1 =                                   // ��� ����� � �������
{
    SPI1,
    {
        SPI_MODE_SLAVE,                 // Init.Mode
        SPI_DIRECTION_2LINES,           // Init.Direction
        SPI_DATASIZE_8BIT,              // Init.DataSize
        SPI_POLARITY_HIGH,              // Init.CLKPolarity
        SPI_PHASE_2EDGE,                // Init.CLKPhase
        SPI_NSS_SOFT,                   // Init.NSS
        SPI_BAUDRATEPRESCALER_32,       // Init.BaudRatePrescaler
        SPI_FIRSTBIT_MSB,               // Init.FirstBit
        SPI_TIMODE_DISABLED,            // Init.TIMode
        SPI_CRCCALCULATION_DISABLED,    // Init.CRCCalculation
        7                               // InitCRCPolynomial
    },
    0, 0, 0, 0, 0, 0, 
    0, 
    0, 
    0, 0, HAL_UNLOCKED, HAL_SPI_STATE_RESET, 0
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static uint8 *recv = 0;                         ///< ����� ��� ����������� ������
uint  Interface::freqForSend = MAX_UINT;
uint  Interface::timeLastReceive = 0;

static const struct FuncInterface
{
    typedef void(*pFuncInterfaceVV)();
    pFuncInterfaceVV func;
    FuncInterface(pFuncInterfaceVV f) : func(f) {};
}
commands[CommandPanel::Number] =
{
    Interface::Empty,
    Interface::Enable,              ///< EnableChannel
    Interface::FormWave,            ///< SetFormWave
    Interface::ParameterValue,      ///< SetFrequency
    Interface::ParameterValue,      ///< SetAmplitude
    Interface::ParameterValue,      ///< SetOffset
    Interface::ParameterValue,      ///< SetDuration
    Interface::ParameterValue,      ///< SetDutyRatio
    Interface::ParameterValue,      ///< SetPhase
    Interface::Reset,               ///< RunReset
    Interface::ModeDebug,           ///< ModeDebug
    Interface::ParameterValue,      ///< SetDelay
    Interface::WriteRegister,       ///< WriteRegister
    Interface::Empty,               ///< SetDurationRise
    Interface::Empty,               ///< SetDurationFall
    Interface::Empty,               ///< SetDurationStady
    Interface::Empty,               ///< SetDutyFactor
    Interface::SetManipulation,     ///< SetManipulation
    Interface::ParameterValue,      ///< SetManipulationDuration
    Interface::ParameterValue,      ///< SetManipulationPeriod
    Interface::ParameterValue,      ///< SetPacketPeriod
    Interface::ParameterValue,      ///< SetPacketNumber
    Interface::SetStartMode,
    Interface::ParameterValue,      ///< SetPeriod
    Interface::Polarity,            ///< SetPolarity
    Interface::SetManipulationMode, ///< SetModeManipulation
    Interface::Empty                ///< RequestData
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Interface::Init()
{
    GPIO_InitTypeDef isGPIOA =
    {   //  SCK         MI           MO
        GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,
        GPIO_MODE_AF_PP,
        GPIO_NOPULL,
        GPIO_SPEED_HIGH,
        GPIO_AF5_SPI1
    };
    HAL_GPIO_Init(GPIOA, &isGPIOA);

    HAL_NVIC_SetPriority(SPI1_IRQn, 1, 0);

    HAL_SPI_Init(&hSPI1);

    HAL_NVIC_EnableIRQ(SPI1_IRQn);

    free(recv);

    recv = (uint8 *)malloc(2);

    HAL_SPI_Receive_IT(&hSPI1, recv, 2);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
SPI_HandleTypeDef *Interface::HandleSPI()
{
    return &hSPI1;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::Enable()
{
    Chan ch = (Chan::E)recv[1];
    bool enable = recv[2] == 1;
      
    Generator::EnableChannel(ch, enable);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::Polarity()
{
    Chan ch = (Chan::E)recv[1];
    FPGA::SetPolarity(ch, recv[2]);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::ReadData()
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::SetManipulation()
{
    Chan ch = (Chan::E)recv[1];
    AD9952::Manipulation::SetEnabled(ch, Buffer2Float(recv + 2) != 0.0f);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::SetManipulationMode()
{
    Chan ch = (Chan::E)recv[1];
    AD9952::Manipulation::SetType(ch, recv[2]);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::SetStartMode()
{
    Chan ch = (Chan::E)recv[1];
    StartMode mode = (StartMode)recv[2];
    FPGA::SetStartMode(ch, mode);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::FormWave()
{
    Chan ch = (Chan::E)recv[1];
    Form form = (Form::E)recv[2];
    Generator::SetFormWave(ch, form);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::WriteRegister()
{
    Register reg = (Register::E)recv[1];

    BitSet64 set;
    for (int i = 0; i < 8; i++)
    {
        set.byte[i] = recv[i + 2];
    }

    uint64 value = set.dword;

    switch (reg.value)
    {
    case Register::OffsetA:
        AD5697::SetOffset(Chan::A, (float)value);
        break;

    case Register::OffsetB:
        AD5697::SetOffset(Chan::B, (float)value);
        break;

    case Register::FreqMeterLevel:
        AD5697::SetFreqLevel((float)value);
        break;

    case Register::FreqMeterHYS:
        AD5697::SetFreqHysteresys((float)value);
        break;

    case Register::FPGA_RG0_Control:
        FPGA::WriteRegister(FPGA::RG::_0_Control, value);
        break;

    case Register::FPGA_RG1_Freq:
        FPGA::WriteRegister(FPGA::RG::_1_Freq, value);
        break;

    case Register::FPGA_RG2_Amplitude:
        FPGA::WriteRegister(FPGA::RG::_2_Amplitude, value);
        break;

    case Register::FPGA_RG3_RectA:
        FPGA::WriteRegister(FPGA::RG::_3_RectA, value);
        break;

    case Register::FPGA_RG4_RectB:
        FPGA::WriteRegister(FPGA::RG::_4_RectB, value);
        break;

    case Register::FPGA_RG5_PeriodImpulseA:
        FPGA::WriteRegister(FPGA::RG::_5_PeriodImpulseA, value);
        break;

    case Register::FPGA_RG6_DurationImpulseA:
        FPGA::WriteRegister(FPGA::RG::_6_DurationImpulseA, value);
        break;

    case Register::FPGA_RG7_PeriodImpulseB:
        FPGA::WriteRegister(FPGA::RG::_7_PeriodImpulseB, value);
        break;

    case Register::FPGA_RG8_DurationImpulseB:
        FPGA::WriteRegister(FPGA::RG::_8_DurationImpulseB, value);
        break;

    case Register::FPGA_RG9_FreqMeter:
        FPGA::WriteRegister(FPGA::RG::_9_FreqMeter, value);
        freqForSend = MAX_UINT;
        break;

    case Register::FPGA_RG10_Offset:
        FPGA::WriteRegister(FPGA::RG::_10_Offset, value);
        break;

    case Register::FreqMeter_Resist:
        FreqMeter::SetResist((FreqResist::E)value);
        break;

    case Register::FreqMeter_Couple:
        FreqMeter::SetCouple((FreqCouple::E)value);
        break;

    case Register::FreqMeter_Filtr:
        FreqMeter::SetFiltr((FreqFiltr::E)value);
        break;

    case Register::Number:
    case Register::Multiplexor1:
    case Register::Multiplexor2:
    default:
        break;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::ParameterValue()
{
    Chan ch = (Chan::E)recv[1];
    CommandPanel command = (CommandPanel::E)recv[0];
    Generator::SetParameter(ch, command, Buffer2Float(recv + 2));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::CreateWave()
{
    Chan ch = (Chan::E)recv[1];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::Reset()
{
#ifndef WIN32

#define MAIN_PROGRAM_START_ADDRESS  (uint)0x8000000
    typedef void(*pFunction)();
    __disable_irq();
    pFunction JumpToApplication = (pFunction)(*(__IO uint *)(MAIN_PROGRAM_START_ADDRESS + 4));
    __enable_irq();
    JumpToApplication();

#endif
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::ModeDebug()
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::Empty()
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::SendFrequency(uint value)
{
    freqForSend = value;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::ReceiveCallbackOld()
{
    if (recv[0] < CommandPanel::Number)
    {
        if(recv[0] != 0)
        {
            commands[recv[0]].func();
        }
        if (Console::ExistString())
        {
//            trans[0] = CommandGenerator::Log;
//            Console::GetString((char *)(trans + 1));
        }
    }
    timeLastReceive = TIME_MS;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::ReceiveCallback()
{
    BitSet16 bs(&recv[0]);                              // ����� ���������� ����������� ����

    ResizeRecieveBuffer(bs.halfWord);                   // ������������� ������ �������� ������ ������ ����� ��������

    CPU::SetReady();

    HAL_SPI_Receive(&hSPI1, recv, bs.halfWord, 100);    // � ��������� ������

    CPU::SetBusy();

    if(recv[0] == CommandPanel::RequestData)
    {
        SendData();
    }
    else if(recv[0] < CommandPanel::Number)   /// \todo ����������� �������� �� ������
    {
        commands[recv[0]].func();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::SendData()
{
    if(freqForSend != MAX_UINT)
    {
        uint16 numBytes = 5;

        CPU::SetReady();

        HAL_SPI_Transmit(&hSPI1, (uint8 *)numBytes, 2, 100);

        CPU::SetBusy();

        uint8 *buffer = (uint8 *)malloc(5);
        buffer[0] = CommandGenerator::FreqMeasure;
        INIT_BIT_SET_32(bs, freqForSend);
        for(int i = 0; i < 4; i++)
        {
            buffer[i + 1] = bs.byte[i];
        }

        CPU::SetReady();

        HAL_SPI_Transmit(&hSPI1, buffer, 5, 100);

        CPU::SetBusy();

        free(buffer);
    }
    else
    {
        uint16 numBytes = 0;
        CPU::SetReady();
        HAL_SPI_Transmit(&hSPI1, (uint8 *)numBytes, 2, 100);
        CPU::SetBusy();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *)
{
    CPU::SetBusy();
    Interface::ReceiveCallback();
    HAL_SPI_Receive_IT(&hSPI1, recv, 2);
    CPU::SetReady();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hSPI)
{
    __IO uint error = hSPI->ErrorCode;

    if(error != HAL_SPI_ERROR_OVR)
    {
        error = error;
    }

    HAL_SPI_Init(&hSPI1);
    HAL_NVIC_EnableIRQ(SPI1_IRQn);
    HAL_SPI_Receive_IT(&hSPI1, recv, 2);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::ResizeRecieveBuffer(uint16 size)
{
    free(recv);

    recv = (uint8 *)malloc(size);
}
