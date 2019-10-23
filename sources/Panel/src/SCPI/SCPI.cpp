#include "defines.h"
#include "SCPI.h"
#include "Command.h"
#include "Display/Console.h"
#include "SCPI/Parser.h"
#include "SCPI/Runner.h"
#include <cstdlib>



uint8        SCPI::Buffer::data[SIZE_BUFFER];
int          SCPI::Buffer::used = 0;


void SCPI::Init()
{
    Buffer::Clear();
}


bool SCPI::Handler::Processing(SimpleMessage *msg)
{
    msg->ResetPointer();

    ::Command command(msg->TakeByte());

    if (command == ::Command::SCPI_Data)
    {
        Buffer::AddData(msg);

        do 
        {
            Parser::Parse();
        } while (Runner::Execute());
    }

    return false;
}


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


void SCPI::Buffer::AddData(SimpleMessage *msg)
{
    Buffer::RemoveBadSymbols();                     // ������� ������� �������������� ������� �� ������ ������

    uint length = msg->TakeWord();

    for (uint i = 0; i < length; i++)
    {
        AddByte(msg->TakeByte());
    }
}


void SCPI::Buffer::AddByte(uint8 byte)
{
    data[used++] = byte;
    if (used == SIZE_BUFFER)
    {
        ShiftToLeft();
    }
}


void SCPI::Buffer::Clear()
{
    used = 0;
}


uint8 SCPI::Buffer::GetByte(uint i)
{
    return data[i];
}
