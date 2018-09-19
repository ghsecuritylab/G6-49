#pragma once
#include "Menu/MenuItems.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Menu
{
public:
    /// ������� ��������� ���� ���������� �� ������� ������������
    static const int NUM_ITEMS_ON_DISPLAY = 4;

    static void Init();
        
    static void Update();

    static void Draw();
    /// ���������� true, ���� ����� ��������� �������������� ���������� ����
    static bool RegIsControlSubPages();
    /// ���������� ����� ��������� �������� ����
    static Item *GetOpenedItem();

    static void SetOpenedItem(Item *item);
    /// ���������� ����� �������� �������� ���� (�������, ��� �������, �������� �������, ������ �������� ���� ������ ���������
    static Item *CurrentItem();
    ///\brief  ����� �������� ����� �������� ����, ���������������� �������������� ������� [F1..F4], ���� ��� ��������� � ������ ���������, � 0, 
    /// ���� �� ���� ������ �� ������.
    static Item *pressedItem;
    /// ���������� ������� �������� �� ��������. ������ ���� �������� ����������� �������� ����. ����� -1
    static int GetPosition(Page *page);
    /// ������������� ��� ��������� ��������������� ��������
    static void SetAdditionPage(Page *page);
    /// ������� �������������� �������� � ���������
    static void ResetAdditionPage();
    /// ��������� �� ������� ��������
    static Page *mainPage;
private:
    /// ���������� �����
    static int DrawTitle();

    static void DrawPagesUGO(int x);

    static void ProcessControl(Control control);
    /// ���� �����-�� ���� �������, �� ����� ��� �����
    static Item *openedItem_;
    /// ���� ���������� ��������� �� �������� ��������� ���� � ������, ���� ����� ��������� �������������� ��������
    static Page *oldPage;
};
