#include "defines.h"
#include "log.h"
#include "common/Transceiver.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"


void Transceiver::Transmit(SimpleMessage *message)
{
    uint timeout = (message->Size() > 1000U) ? 100U : 10U;

    bool result = false;
    
    uint failed = 0;

    while (!result)
    {
        HAL_SPI4::WaitFalling();                                                // ������� �������� ����� ���������� ������� � ��������� "��������"

        for (int i = 0; i < 2; i++)
        {
            HAL_SPI4::Transmit(message->Size(), 10);                               // ������� ������ ������������ ������

            HAL_SPI4::Transmit(message->Data(), message->Size(), timeout);         // ������� ��������������� ������

            uint newSize = 0;
            HAL_SPI4::Receive(&newSize, 4, 10);                                    // ������ ��������� ������ ������, ������� ����� �������� ��� ����������

            uint trashedBytes = HAL_SPI4::ReceiveAndCompare(message->Data(), message->Size());

            result = (trashedBytes == 0);
        }

        if (!result)
        {
            /// \todo ������� �� ��, ��� ��� ������� ������� ������ ����� ���������� ���������. �� �������� ���� �� ����������� � ��� ����.
            failed++;
            if(failed > 5)
            {
                break;
            }
        }
    };
}


bool Transceiver::Receive(SimpleMessage *message)
{
    HAL_SPI4::WaitFalling();

    uint size = 0;
    HAL_SPI4::Receive(&size, 4, 10);

    if (message->AllocateMemory(size))
    {
        HAL_SPI4::Receive(message->Data(), message->Size(), 50);
        return true;
    }

    return message->Size() != 0;
}
