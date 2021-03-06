#include "defines.h"
#include "Debug.h"
#include "log.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/Settings.h"
#include "Utils/Debug.h"
#include <cstdlib>


volatile static uint timeStart = 0;
static uint timeCounter = 0;
static uint timeStartCounter = 0;
char *Debug::file = 0;
int   Debug::line = 0;



void Debug::StartProfiling()
{
    timeStart = TIME_US;
}


void Debug::PointProfiling(char *)
{
    //LOG_WRITE("%s %d", point, TIME_US - timeStart);
}


void Debug::ClearTimeCounter()
{
    timeCounter = 0;
}


void Debug::StartIncreaseCounter()
{
    timeStartCounter = TIME_US;
}


void Debug::StopIncreaseCounter()
{
    timeCounter += (TIME_US - timeStartCounter);
}


uint Debug::GetTimeCounterUS()
{
    return timeCounter;
}


bool Debug::ShowSends()
{
    return DEBUG_SHOW_SENDS;
}


void Debug::LogBuffer16(uint16 *buffer)
{
    LOG_WRITE("%04x %04x %04x %04x %04x %04x %04x", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6]);
}


void Debug::LogBufferF(const float *buffer)
{
    LOG_WRITE("%5.4f %5.4f %5.4f %5.4f %5.4f %5.4f %5.4f", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6]);
}


uint Debug::FreeHeap()
{
    /*
    uint max = 1024 * 1024;
    uint min = 0;

    while (min < max)
    {
        void *buffer = 0;
        buffer = std::malloc(max);            // �������� �������� ������
        if (buffer == 0)                            // ���� �� ����������
        {
            max = (max + min) / 2;                  // �� ��������� ������� �������
            continue;
        }

        min = max;
        max = (uint)(max * 1.5);
        std::free(buffer);
    }
    
    return min;
    */

    //char buffer[100];
    //
    //__heapstats

    return 0;
}
