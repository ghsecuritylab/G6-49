#pragma once
#include "Font.h"



struct AdvancedFont
{
    static void Set(TypeFont::E type);
    /// ���������� true, ���� � ������ row ���� �������
    static bool RowNotEmpty(uint8 symbol, int row);
    /// ���������� ������ �������
    static uint8 GetWidth(uint8 symbol);
    /// ���������� ������ �������
    static uint8 GetHeight(uint8 symbol);
    /// ���������� ������ ������ �������� �������
    static uint8 GetHeight();

    static bool BitIsExist(uint8 symbol, int row, int bit);

private:
    static TypeFont::E currentType;
};
