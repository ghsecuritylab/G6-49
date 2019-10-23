#pragma once
#include "Menu/MenuItems.h"



namespace Menu
{
    /// ������� ��������� ���� ���������� �� ������� ������������
    const int NUM_ITEMS_ON_DISPLAY = 4;

    void Init();
    
    void Update();

    void Draw();
    /// ���������� �����
    int DrawTitle();

    void DrawPagesUGO(int x);
    /// ���������� true, ���� ����� ��������� �������������� ���������� ����
    bool RegIsControlSubPages();
    /// ���������� ����� ��������� �������� ����
    Item* GetOpenedItem();
    /// ���������� Item, ������� ����� ��������� ��������
    void SetOpenedItem(Item *item);
    /// ���������� ������� Item - ������ � ���� ����� ��������� ������� ����� � ESC
    void SetCurrentItem(Item *item);
    /// ����� �������� Item - �������� Item'a ������ ���
    void ResetCurrentItem();
    /// ���������� ������� Item
    Item* GetCurrentItem();
    /// �������� �������� ���� - ���� ������
    void ResetOpenedItem();
    /// ���������� ������� �������� �� ��������. ������ ���� �������� ����������� �������� ����. ����� -1
    int GetPosition(const Page *page);
    /// ������������� ��� ��������� ��������������� ��������
    void SetAdditionPage(Page *page);
    /// ������� �������������� �������� � ���������
    void ResetAdditionPage();
    /// ��������� �� ������� ��������
    extern Page *mainPage;
    ///\brief  ����� �������� ����� �������� ����, ���������������� �������������� ������� [F1..F4], ���� ��� ��������� � ������ ���������, � 0, 
    /// ���� �� ���� ������ �� ������.
    extern Item *pressedItem;
};
