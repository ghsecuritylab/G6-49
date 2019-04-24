#include "defines.h"
#include "SCPI.h"
#include "Command.h"
#include "Display/Console.h"
#include "SCPI/Commands.h"
#include "SCPI/Parser.h"
#include <cstdlib>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SCPI::Buffer SCPI::buffer;
uint8        SCPI::Buffer::data[SIZE_BUFFER];
int          SCPI::Buffer::used = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SCPI::Init()
{
    Buffer::Clear();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SCPI::Handler::Processing(SimpleMessage *msg)
{
    msg->ResetPointer();

    ::Command::E command = (::Command::E)msg->TakeByte();

    if (command == ::Command::SCPI_Data)
    {
        Buffer::AddData(msg);

        Parser::Parse();
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SCPI::Buffer::RemoveBadSymbols()
{
    while (data[0] != ':' && data[0] != '*')
    {
        if (!ShiftToLeft())
        {
            break;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SCPI::Buffer::ShiftToLeft()
{
    if (used == 0)
    {
        return false;
    }

    std::memmove(data, data + 1, SIZE_BUFFER - 1);
    used--;

    return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SCPI::Buffer::AddData(SimpleMessage *msg)
{
    Buffer::RemoveBadSymbols();                     // ������� ������� �������������� ������� �� ������ ������

    uint length = msg->TakeWord();

    for (uint i = 0; i < length; i++)
    {
        AddByte(msg->TakeByte());
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SCPI::Buffer::AddByte(uint8 byte)
{
    data[used++] = byte;
    if (used == SIZE_BUFFER)
    {
        ShiftToLeft();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SCPI::Buffer::RunAndCompress()
{
    static const StructCommand commands[] =
    {
        {"*IDN?",       SCPI::Process::IDN},
        {"*RST",        SCPI::Process::RST},
        {"CHANNEL",     SCPI::Process::Channel},
        {"FREQUENCY",   SCPI::Process::Frequency},
        {"FORM",        SCPI::Process::Form},
        {"AMPLITUDE",   SCPI::Process::Amplitude},
        {nullptr, nullptr}
    };

    return ProcessStructs(commands);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SCPI::Buffer::ProcessStructs(const StructCommand * /*commands*/)
{
    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SCPI::Result::E SCPI::Process::IDN()
{
    return Result::Count;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SCPI::Result::E SCPI::Process::RST()
{
    return Result::Count;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SCPI::Result::E SCPI::Process::Channel()
{
    return Result::Count;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SCPI::Result::E SCPI::Process::Frequency()
{
    return Result::Count;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SCPI::Result::E SCPI::Process::Form()
{
    return Result::Count;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SCPI::Result::E SCPI::Process::Amplitude()
{
    return Result::Count;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SCPI::Buffer::Clear()
{
    used = 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 SCPI::Buffer::GetByte(uint i)
{
    return data[i];
}
