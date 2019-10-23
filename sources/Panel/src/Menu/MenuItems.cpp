#include "stdafx.h"
#ifndef WIN32
#include "MenuItems.h"
#include "Display/Colors.h"
#include "Display/DisplayTypes.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include <Menu/Menu.h>
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Log.h"
#endif



#define NAME_FROM_INDEX(index) (names[index])



const char *Choice::NameCurrentSubItem()
{
    return NAME_FROM_INDEX(CurrentIndex());
}


String Choice::NameNextSubItem()
{
    int8 nextIndex = CurrentIndex() + 1;

    if (nextIndex == NumSubItems())
    {
        nextIndex = 0;
    }
    return String(NAME_FROM_INDEX(nextIndex));
}


const char *Choice::NamePrevSubItem()
{
    int8 prevIndex = CurrentIndex() - 1;

    if (prevIndex < 0)
    {
        prevIndex = NumSubItems() - 1;
    }
    return NAME_FROM_INDEX(prevIndex);
}


Item *Page::GetItem(const KeyEvent &key) const
{
    return (Item *)items[CurrentSubPage() * Menu::NUM_ITEMS_ON_DISPLAY + (key - KeyEvent::F1)];
}


String Choice::NameSubItem(int i) const
{
    return String(NAME_FROM_INDEX(i));
}


int8 Page::PosCurrentItem() const
{
    return MENU_POS_ACT_ITEM(nameOrNumBit) & 0x7f;
}


void Page::ChangeSubPage(int delta)
{
    if (delta > 0 && CurrentSubPage() < NumSubPages() - 1)
    {
        SetCurrentSubPage(CurrentSubPage() + 1);
    }
    else if (delta < 0 && CurrentSubPage() > 0)
    {
        SetCurrentSubPage(CurrentSubPage() - 1);
    }
    else
    {
        // ����� ������
    }
}


bool Item::IsShade() const
{
    return Menu::GetOpenedItem() &&  this != Menu::GetOpenedItem();
}


bool Item::IsPressed() const
{
    return this == Menu::pressedItem;
}


bool Item::IsOpened() const
{
    if (type == Item::Type::Page)
    {
        return ((Item *)this)->Keeper()->CurrentItemIsOpened();
    }
    return Menu::GetOpenedItem() == this;
}


void Item::Open(bool open)
{
    Keeper()->SetPosActItem(open ? (Keeper()->PosCurrentItem() | 0x80) : (Keeper()->PosCurrentItem() & 0x7f));
}


String Item::GetTitle() const
{
    return String(titleHint[0]);
}


#define CURRENT_ITEM_IS_OPENED return _GET_BIT(MENU_POS_ACT_ITEM(nameOrNumBit), 7) == 1


bool PageBase::CurrentItemIsOpened() const
{
    CURRENT_ITEM_IS_OPENED;
}


bool Page::CurrentItemIsOpened() const
{
    CURRENT_ITEM_IS_OPENED;
}


int Page::CurrentSubPage() const
{
    return MENU_CURRENT_SUBPAGE(nameOrNumBit);
}


void Page::SetPosActItem(int pos)
{
    MENU_POS_ACT_ITEM(nameOrNumBit) = (int8)pos;
}


void Page::SetCurrentSubPage(int pos)
{
    MENU_CURRENT_SUBPAGE(nameOrNumBit) = (int8)pos;
}


int Item::DrawHint(int x, int y, int width, Color color) const
{
    Painter::SetColor(color);

    return Text::DrawTextInColumnWithTransfers(x, y, width, titleHint[1], color);
}
