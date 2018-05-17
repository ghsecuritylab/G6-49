#include "Menu.h"
#include "Menu/MenuItems.h"
#include "Display/InputWindow.h"
#include "Display/InputWindowStruct.h"
#include "Generator.h"
#include "Hardware/CPU.h"
#include "Utils/Math.h"
#include "Menu/Pages/PageSettings.h"
#include "Menu/Pages/PageSettings2.h"
#include "Menu/Pages/PageFrequencyCounter.h"
#include "Menu/Pages/PageService.h"
#include "Menu/Pages/PageDebug.h"
#include "Menu/Pages/PageUSB.h"
#include "Settings/Settings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Control *Menu::itemUnderKey = 0;
Control *Menu::openedItem = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Page *menu[NUM_PAGES] =
{
    PageSignals::pointer,
    PageSignals2::pointer,
    PageFrequencyCounter::pointer,
    PageUSB::pointer,
    PageService::pointer,
    PageDebug::pointer
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::Init(void)
{
    PageSignals::Init();
    PageSignals::OnPress_Form();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::Update(void)
{
    while (!CPU::Keyboard::BufferIsEmpty())
    {
        StructControl control = CPU::Keyboard::GetNextControl();
        if (ADDITION_PAGE_IS_INPUT)
        {
            InputWindow::ProcessContorl(control);
        }
        else
        {
            ProcessControl(control);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::ProcessControl(StructControl strContr)
{
    PanelControl key = strContr.control;
    TypePress pressed = strContr.typePress;

    if (openedItem)
    {
        if (key == REG_LEFT)
        {
            openedItem->Rotate(REG_LEFT);
        }
        else if (key == REG_RIGHT)
        {
            openedItem->Rotate(REG_RIGHT);
        }
        else if (key == REG_BTN)
        {
            openedItem->Press(pressed);
        }
    }

    if (key >= B_F1 && key <= B_F5)
    {
        openedItem = CurrentPage()->Item(key - B_F1)->Press(pressed);
    }
    else if (key == REG_LEFT)
    {
        if (RegIsControlPages())
        {
            if (CURRENT_PAGE > 0)
            {
                --CURRENT_PAGE;
            }
        }
    }
    else if (key == REG_RIGHT)
    {
        if (RegIsControlPages())
        {
            if (CURRENT_PAGE < NUM_PAGES - 1)
            {
                ++CURRENT_PAGE;
            }
        }
    }
    else if (pressed == TypePress_Release || pressed == TypePress_LongPress)
    {
        if (Menu::OpenedItem() && pressed == TypePress_LongPress)
        {
            if (Menu::OpenedItem()->ButtonForItem())
            {
                openedItem = 0;
            }
        }
        else if(key == B_ON1)
        {
            CHANNEL_ENABLED_A = !CHANNEL_ENABLED_A;
            Generator::EnableChannel(A, CHANNEL_ENABLED_A);
        }
        else if (key == B_ON2)
        {
            CHANNEL_ENABLED_B = !CHANNEL_ENABLED_B;
            Generator::EnableChannel(B, CHANNEL_ENABLED_B);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Menu::RegIsControlPages(void)
{
    return openedItem == 0 && ADDITION_PAGE_IS_NONE;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
const char *Menu::NameCurrentParameter(void)
{
    return PageSignals::cpParameters->NameCurrentSubItem();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Control *Menu::OpenedItem()
{
    return openedItem;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Control *Menu::CurrentItem()
{
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Page *Menu::CurrentPage(void)
{
    volatile int8 num = CURRENT_PAGE;

    if (num == 1)
    {
        num = num;
    }

    Page *page = (Page *)menu[num];

    return (ADDITION_PAGE_IS_NONE ? page : ADDITION_PAGE);
}
