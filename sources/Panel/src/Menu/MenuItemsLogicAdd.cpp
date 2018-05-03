#include "Menu/MenuItems.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Choice_Press(Choice *choice, TypePress press)
{
    if (press == TypePress_Release)
    {
        CircleIncrease<int8>(choice->cell, 0, (int8)(choice->NumSubItems() - 1));
        if (choice->funcOnChanged)
        {
            choice->funcOnChanged(true);
        }
    }
    else if (press == TypePress_LongPress)
    {
        OPENED_ITEM = choice;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ChoiceWaveParameter_Press(ChoiceParameter *choice, TypePress press)
{
    if (press == TypePress_Release)
    {
        volatile bool allow = false;
        do
        {
            CircleIncrease<uint8>(choice->numParameter, 0, (uint8)(NumParameters - 1));
            allow = (choice->allowParameters).allow[*choice->numParameter];
        } while (!allow);
    }
    else if (press == TypePress_LongPress)
    {
        OPENED_ITEM = choice;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Button_Press(Button *button)
{
    if (button && button->funcOnPress)
    {
        button->funcOnPress();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void SButton_Press(SButton *button)
{
    if (button && button->funcOnPress)
    {
        button->funcOnPress();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Item_Rotate(void *item, PanelControl control)
{
    if (ItemIsChoice(item))
    {
        Choice *choice = (Choice*)item;

        if (control == REG_LEFT)
        {
            CircleIncrease<int8>(choice->cell, 0, (int8)(choice->NumSubItems() - 1));
        }
        else if (control == REG_RIGHT)
        {
            if(*(choice->cell) == 0)
            {
                *(choice->cell) = (uint8)(choice->NumSubItems() - 1);
            }
            else
            {
                --(*(choice->cell));
            }
        }
        if (choice->funcOnChanged)
        {
            choice->funcOnChanged(true);
        }
    }
}
