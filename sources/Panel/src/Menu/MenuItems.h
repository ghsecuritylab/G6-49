#pragma once
#include "Display/Colors.h"
#include "Display/Display.h"
#include "Keyboard/Controls.h"
#include "MenuItemsDefs.h"
#include "common/Command.h"
#include "Utils/String.h"


typedef bool (*pFuncBKey)(const Key&);


/// ����� ����� ��� ���� ����� ��������� ����
#define COMMON_PART_MENU_ITEM                                                                                                               \
    uint8           type;           /* ��� ����� */                                                                                         \
    int8            num;            /* ����� ��������� ��� Choice ��� ����� ��������� ��� Page*/                                            \
    bool            isPageSB;       /* ���� true, �� ��� �������� ����� ������, ����� type == Item_Page */                                  \
                                    /* ���� type == Item_Choice, �� ������� �������� �������� Choice - ����� ������ �� ���� ��������� */    \
    uint8           nameOrNumBit;   /* ��� �� ������������ NamePage, ���� type == Item_Page */                                              \
                                    /* � ������, ���� type == Item_Choice,  ���������� ����� ���� */                                        \
    const PageBase  *keeper;        /* ����� ��������, ������� �����������. ��� Page_Main = 0 */                                            \
    const char      *title[2];      /* �������� �������� �� ������� � ���������� ������ */                                                  \
    const char      *hint[2];       /* ��������� �� ������� � ���������� ������ */                                                          \
    pFuncBV         funcOfActive    /* ������� �� ������ ������� */

class PageBase;
class Page;
class Form;

   
class Item
{
public:
    COMMON_PART_MENU_ITEM;
    /// ������ ���� ������� ����
    struct Type
    {
        enum E
        {
            None,
            Choice,             ///< ����� ������ - ��������� ������� ���� �� ���������� �������� ��������.
            Button,             ///< ������.
            Page,               ///< ��������.
            Governor,           ///< ��������� - ��������� ������� ����� ����� �������� �������� �� ������� ��������� ����������.
            GovernorColor,      ///< ��������� ������� ����.
            SmallButton,        ///< ������ ��� ������ ����� ������
            ChoiceParameter,
            Number
        } value;
        Type(E v) : value(v) {};
        operator uint8() const { return static_cast<uint8>(value); };
        bool Is(E v) const     { return (v == value); };
    };
    /// \brief ���������� true, ���� ������� ���� control ������� (��������� �� �� ����� ������� ����. ��� �������, ��� ��������, ��� ������� 
    /// �������������� ������� ���� ����� Choice ��� Governor
    bool IsShade() const;
    /// ���������� true, ���� ������, ��������������� �������� ���� control, ��������� � ������� ���������
    bool IsPressed() const;
    /// ���������� true, ���� ������� ���� �� ������ control ������
    bool IsOpened() const;

    void Open(bool open);
    /// ���������� �������� ��������, ��� ��� �������� �� ������� �������
    String GetTitle(uint lang) const;

    void Draw(bool opened, int x = -1, int y = -1) const;
    /// ������������ ������� ������. ���������� ��������� �� ����, ���� ��������� � �������� ��������� ����� �������, � 0 � ��������� ������
    void Press(const Key &control);

    Type GetType() const;

    /// ���������� ���������� ����� ������ ���� �� ��������
    int PositionOnPage() const;
    /// ���������� ������ ���� � �������� ����
    String FullPath();
    /// ������� ��������� � �������� ����� ������. ���������� ������ ���������� ����������� ������
    int DrawHint(int x, int y, int width, Color color = Color::NUMBER) const;

    Page *Keeper() const { return const_cast<Page *>(reinterpret_cast<const Page *>(keeper)); };

    bool IsActive() const { return funcOfActive(); };

    static const int WIDTH = 69;

    static const int HEIGHT = 55;

    struct Title
    {
        static const int HEIGHT = 12;
    };

    static bool FuncActive() { return true; }

    static Item *Empty();
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Page ///

/// ��������� �������� ����.
class PageBase
{
public:
    COMMON_PART_MENU_ITEM;
    /// \brief ����� ��������� �� ������ ���� �������� (� ������� ��������) ��� �������� ����� ������  ����� �������� 6 ���������� �� SButton : 
    /// 0 - B_Menu, 1...5 - B_F1...B_F5
    Item **items;
    /// ����� ���������� ��� ������� �� �������� ��������
    pFuncVB funcEnter;
    /// ����� ���������� ����� ��������� ������
    pFuncVV funcOnDraw;

    pFuncBKey funcOnKey;

    bool CurrentItemIsOpened() const;
};


#define SMALL_BUTTON_FROM_PAGE(page, numButton)     ((SButton *)((Page *)page)->items[numButton])

class Page : public Item
{
public:

    enum E
    {
         NoPage,
         Settings,
         Service,
         Debug,
         FrequencyCounter,
         USB,
         SB_Input,
         Registers,
         Main,
         Settings_Load,
         Settings_CalibrationA,
         Settings_CalibrationB,
         Calibration,
         SubRanges,
         Colors,
         Color_ChanA,
         Color_ChanB,
         Color_Menu,
         Count
    };
    

    /// ���������� true, ���� ������� ������� �������� ������
    bool CurrentItemIsOpened() const;
    /// \brief ����� ��������� �� ������ ���� �������� (� ������� ��������) ��� �������� ����� ������  ����� �������� 6 ���������� �� SButton : 
    /// 0 - B_Menu,  1...5 - B_F1...B_F5
    const Item * const *items;   
    /// ����� ���������� ��� ������� �� �������� ��������
    pFuncVB  funcEnter;
    /// ����� ���������� ����� ��������� ������
    pFuncVV  funcOnDraw;
    /// � �������� ����� ������ ���������� ��� �������� ����� ���������
    pFuncBKey funcOnKey;
    /// D��������� ����� ���������� � �������� �� ������ page
    int NumSubPages() const;
    /// ���������� ���������� ��������� � �������� �� ������ page
    int NumItems() const;
    /// ���������� ����� ������� �����������
    int CurrentSubPage() const;
    /// ������ �������� ��������
    void DrawOpened() const;
    /// ������ �������� ��������
    void DrawClosed(int x, int y) const;

    void SetCurrentSubPage(int pos);
    /// ������������� ������� ��������� ������ ����
    void SetPosActItem(int pos);
    /// ���������� ����� �������� ����, ���������������� ������� �������������� �������
    Item *GetItem(const Key &key) const;
    /// ���������� ������� �������� �������� ������� page
    int8 PosCurrentItem() const;

    void ChangeSubPage(int delta);
    /// ������������ ������� ������. ���������� ��������� �� ����, ���� ��������� � �������� ��������� ����� �������, � 0 � ��������� ������
    bool Press(const Key &key);

    struct Title
    {
        static const int HEIGHT = 18;
    };

    static void FuncDraw() {};

    static void FuncEnter(bool) { };
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Button ///

/// ��������� ������.
class ButtonBase
{
public:
    COMMON_PART_MENU_ITEM;
    /// �������, ������� ���������� ��� ������� �� ������.
    pFuncVV     funcOnPress;
    /// ������� ����� ���������� �� ����� ��������� ������.
    pFuncVII    funcForDraw;
};

class Button : public Item
{
public:
    /// �������, ������� ���������� ��� ������� �� ������.
    pFuncVV     funcOnPress;
    /// ������� ����� ���������� �� ����� ��������� ������.
    pFuncVII    funcForDraw;
    /// ������������ ������� ������. ���������� ����, ������ ��� �� ����� ���������� � �������� ���������.
    Item *Press(Key::Action action);

    void Draw(int x, int y) const;

    static void FuncPress() { };
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// SButton ///

#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4623 4626 5027)
#endif
struct StructHelpSmallButton
{
    /// ��������� �� ������� ��������� ����������� �������� ������
    pFuncVII    funcDrawUGO;
    /// ������� � ������� �����������.
    pString     helpUGO[2];
};
#ifdef WIN32
#pragma warning(pop)
#endif


/// ��������� ������ ��� ��������������� ������ ����.
class SButtonBase
{
public:
    COMMON_PART_MENU_ITEM;
    /// ��� ������� ��������� ��� ��������� ������� ������.
    pFuncVV                         funcOnPress;
    /// ��� ������� ���������� ��� ��������� ������ � ����� � ������������ x, y.
    pFuncVII                        funcForDraw;

    const StructHelpSmallButton    *hintUGO;

    int                             numHints;
};


class SButton : public Item
{
public:
    /// ��� ������� ��������� ��� ��������� ������� ������.
    pFuncVV                         funcOnPress;
    /// ��� ������� ���������� ��� ��������� ������ � ����� � ������������ x, y.
    pFuncVII                        funcForDraw;

    const StructHelpSmallButton    *hintUGO; 

    int                             numHints;
    /// ������������ ������� ������. ���������� 0, ������ ��� �� ����� ���������� � �������� ���������
    Item *Press(Key::Action action);

    void Draw(int x, int y) const;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Governor ///

/// ��������� ���������.
class GovernorBase
{
public:
    COMMON_PART_MENU_ITEM;
    int16   minValue;
    int16   maxValue;
    int16  *cell;
    pFuncVV funcOfChanged;
    pFuncVV funcBeforeDraw;
    pFuncVB funcPress;
};

class Governor : public Item
{
public:
    /// ���������� ��������, ������� ����� ��������� ���������.
    int16   minValue;
    /// ������������ ��������.
    int16   maxValue;

    int16  *cell;
    /// �������, ������� ����� �������� ����� ����, ��� �������� ���������� ����������.
    pFuncVV funcOfChanged;
    /// �������, ������� ���������� ����� ����������
    pFuncVV funcBeforeDraw;
    /// ������� ���������� ��� ������� �� item. �������� bool == true ��������, ��� item ��������������� ��������
    pFuncVB funcPress;
    /// ��������� �������� �������� ���������� ��� ���������� �������� ���� ���� Governor (� ����������� �� ����� delta).
    void StartChange(int detla);
    /// ���������� ��������� ������� ��������, ������� ����� ������� governor.
    int16 NextValue() const;
    /// ���������� ��������� ������� ��������, ������� ����� ������� governor.
    int16 PrevValue() const;
    /// ������������ ��������� ���� ��������.
    float Step();
    /// �������� �������� � ������� ������� ��� ��������� ��������.
    void ChangeValue(int delta);
    /// ��� �������� �������� ������������ ������ �� ��������� �������.
    void NextPosition();
    /// ���������� ����� ��������� � ���� ��� ����� �������� governor. ������� �� ������������� ��������, ������� ����� ��������� governor.
    int  NumDigits() const;
    /// ���������� ����������� ����������, ��������������� ��� �������� ���������
    static char GetSymbol(int value);

    void Draw(int x, int y) const;

    void DrawValue(int x, int y) const;
    /// ��������� �����������
    void Press(const Key &key);
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Choice ///
class ChoiceBase
{
public:
    COMMON_PART_MENU_ITEM;

    int8    *cell;
    /// �������� ������ �� ������� � ���������� ������.
    pString *namesRU;
    pString *namesEN;
    /// ��������� ��� ������� �������� �� ������� �����
    pString *hintsRU;
    pString *hintsEN;
    /// ������� ������ ���������� ����� ��������� �������� ��������.
    pFuncVB	 _funcOnChanged;
    /// ������� ���������� ����� ��������� ��������. 
    pFuncVII funcForDraw;

    int8 CurrentIndex() const;
};

class Choice : public Item
{
public:

    int8 *cell;
    /// �������� ������ �� ������� � ���������� ������.
    pString *names;
    /// ��������� ��� ������� �������� �� ������� �����
    pString *hints;
    /// ������� ������ ���������� ����� ��������� �������� ��������.
    pFuncVB	 funcOnChanged;
    /// ������� ���������� ����� ��������� ��������. 
    pFuncVII funcForDraw;

    void StartChange(int delta) const;
    /// ������������ ��������� ���� ��������.
    float Step();
    /// ���������� ���������� ��������� ������ � �������� �� ������ choice
    int8 NumSubItems() const;

    void Draw(bool opened, int x = -1, int y = -1) const;
    /// ���������� ��� �������� �������� ������ �������� choice, ��� ��� �������� � �������� ���� ���������
    const char *NameCurrentSubItem() const;
    /// ���������� ��� ���������� �������� ������ �������� choice, ��� ��� �������� � �������� ���� ���������
    String NameNextSubItem() const;
    /// ���������� ������ ����������
    int GetHeightOpened() const;

    const char *NamePrevSubItem();
    /// ���������� ��� �������� ������ �������� choice � ������� i ��� ��� �������� � �������� ���� ���������
    String NameSubItem(int i) const;
    /// ���������� ��������� �� ����, ���� ��������� � �������� ���������, � 0, ���� � ��������
    Item *Press(const Key &key);

    int8 CurrentIndex() const;
};




///
class ChoiceParameterBase
{
public:
    COMMON_PART_MENU_ITEM;
    pFuncVV     funcOnPress;
    Form  *form;
};


class ChoiceParameter : public Item
{
public:
    pFuncVV     funcOnPress;
    Form  *form;
    /// ������������ ������� ������. ���������� ��������� �� ����, ���� ��������� � �������� ��������� � 0 � ���������.
    Item *Press(Key::Action action);

    pString NameSubItem(int num) const;

    pString NameCurrentSubItem() const;

    void Draw(bool opened, int x, int y) const;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Formula ////

/// ��������� ������� ���� ��� ��������� ������������� � ������ �������������� �������
#define FIELD_SIGN_MEMBER_1_ADD 0
#define FIELD_SIGN_MEMBER_1_MUL 1
#define FIELD_SIGN_MEMBER_2_ADD 2
#define FIELD_SIGN_MEMBER_2_MUL 3
#define POS_SIGN_MEMBER_1       0
#define POS_KOEFF_MEMBER_1      1
#define POS_SIGN_MEMBER_2       2
#define POS_KOEFF_MEMBER_2      3

class FormulaBase
{
public:
    COMMON_PART_MENU_ITEM;
    /// ����� ������, ��� �������� Function, �� ������� ������ ���� ��������
    int8 *function;
    /// ����� ������������ ��� ������ ����� ��� ��������
    int8 *koeff1add;
    /// ����� ������������ ��� ������ ����� ��� ��������
    int8 *koeff2add;
    /// ����� ������������ ��� ������ ����� ��� ���������
    int8 *koeff1mul;
    /// ����� ������������ ��� ������ ����� ��� ���������
    int8 *koeff2mul;
    /// ������� ������ : 0 - ���� ������� �����, 1 - ����������� ������� �����, 2 - ���� ������� �����, 3 - ����������� ������� �����
    int8 *curDigit;
    /// ��� ������� ���������� ����� ��������� ��������� �������� ����������.
    pFuncVV funcOfChanged;
};

class Formula : public Item
{
public:
    /// ����� ������, ��� �������� Function, �� ������� ������ ���� ��������
    int8 *function;
    /// ����� ������������ ��� ������ ����� ��� ��������
    int8 *koeff1add;
    /// ����� ������������ ��� ������ ����� ��� ��������
    int8 *koeff2add;
    /// ����� ������������ ��� ������ ����� ��� ���������
    int8 *koeff1mul;
    /// ����� ������������ ��� ������ ����� ��� ���������
    int8 *koeff2mul;
    /// ������� ������ : 0 - ���� ������� �����, 1 - ����������� ������� �����, 2 - ���� ������� �����, 3 - ����������� ������� �����
    int8 *curDigit;
    /// ��� ������� ���������� ����� ��������� ��������� �������� ����������.
    pFuncVV funcOfChanged;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Time ///

#define iEXIT   0
#define iDAY    1
#define iMONTH  2
#define iYEAR   3
#define iHOURS  4
#define iMIN    5
#define iSEC    6
#define iSET    7

class TimeBase
{
public:
    COMMON_PART_MENU_ITEM;
    /// ������� ���� ���������. 0 - �����, 1 - ���, 2 - ���, 3 - ����, 4 - ����, 5 - �����, 6 - ���, 7 - ����������.
    int8 *curField;

    int8 *hours;

    int8 *minutes;

    int8 *seconds;

    int8 *month;

    int8 *day;

    int8 *year;
};

typedef void * pVOID;
#define MAX_NUM_ITEMS_IN_PAGE 15
typedef pVOID arrayItems[MAX_NUM_ITEMS_IN_PAGE];
