#include "defines.h"
#include "Log.h"
#include "Display/Display.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Generator/Generator.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"


/** @defgroup Panel
 *  @{
 */


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(void)
{
    CPU::Config();
   
    Display::Init();

    Timer_Init();

    Keyboard::Init();

    uint time = TIME_MS;

    while (TIME_MS - time < 200)
    {
    };    // �������� �������, ������ ��� ��� �� �� ����������� ���������. ������, �� ���������������� �������
                                                // ������� ������

    Generator::Reset();

    time = TIME_MS;

    while (TIME_MS - time < 200) { };

    Settings_Load();

    gMenu.Init();

    while (1)
    {
        gMenu.Update();
        Display::Update();
    }
}


/** @} */
