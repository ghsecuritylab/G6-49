#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "Display/Text.h"
#include "Generator/ParameterPainter.h"
#include "Generator/Signals.h"
#include "Menu/Menu.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"
#include "PageTuneParameter.h"


static ParameterPainterSupporting support;


static void OnDraw_TuneParameter();


void PageTuneParameter::SetParameter(Parameter *parameter)
{
    support.SetParameter(parameter);
}


static void Draw_Left(int, int)
{
}

static void OnPress_OrderUp()
{

}

DEF_SMALL_BUTTON(sbOrderUp,                                                                                                                               //--- ���� ����� - ������ ����� ---
    "������� ������",
    "��������� ������� ������� ���������",
    *PageTuneParameter::self, Item::FuncActive, OnPress_OrderUp, Draw_Left
)


static void Draw_Right(int, int)
{
}

static void OnPress_OrderDown()
{

}

DEF_SMALL_BUTTON(sbOrderDown,                                                                                                                            //--- ���� ����� - ������ ������ ---
    "������� �������",
    "��������� ������� ������� ���������",
    *PageTuneParameter::self, Item::FuncActive, OnPress_OrderDown, Draw_Right
)



static void OnPress_Cancel()
{
    Menu::ResetAdditionPage();
}

static void Draw_Cancel(int x, int y)
{
    String("��������").Draw(x + 13, y + 12);
    Text::Draw4SymbolsInRect(x + 25, y + 28, Ideograph::_8::Delete);
}

DEF_SMALL_BUTTON(sbCancel,                                                                                                                                      //--- ���� ����� - ������ ---
    "������� ���������",
    "���������� �� ����� ������ ��������",
    *PageTuneParameter::self, Item::FuncActive, OnPress_Cancel, Draw_Cancel
)


static void Draw_Enter(int x, int y)
{
    String("�����������").Draw(x + 3, y + 12);
    Text::Draw4SymbolsInRect(x + 25, y + 28, Ideograph::_8::Save);
}

static void OnPress_Enter()
{

}

DEF_SMALL_BUTTON(sbEnter,                                                                                                                                         //--- ���� ����� - ���� ---
    "����",
    "����������� ���� ������ ��������",
    *PageTuneParameter::self, Item::FuncActive, OnPress_Enter, Draw_Enter
)


static bool OnControl_TuneParameter(Key &)
{
    return false;
}


DEF_PAGE_SB( pTuneParameter,   //-V641
    "���� ��������", //-V641
    "",
    &sbOrderUp,         ///< ���� ����� - ������ �����
    &sbOrderDown,       ///< ���� ����� - ������ ������
    &sbCancel,          ///< ���� ����� - ������
    &sbEnter,           ///< ���� ����� - ����
    Page::SB_Input, 0, Item::FuncActive, FuncEnter, OnDraw_TuneParameter, OnControl_TuneParameter
)

Page *PageTuneParameter::self = reinterpret_cast<Page *>(const_cast<PageBase *>(&pTuneParameter));


/// ���������� �������� ���������
static void DrawNameParameter(int x, int y, int width)
{
    int length = Font::GetLengthText(support.parameter->Name());

    int pos = x + width / 2 - length / 2;

    Text::Draw(pos, y + 15, support.parameter->Name(), Color::WHITE);
}


/// ���������� �������� ���������
static void DrawParameter(int y)
{
    Color::WHITE.SetAsCurrent();

    char buffer[2] = { 0, 0 };

    for(uint i = 0; i < support.NumSymbols(); i++)
    {
        buffer[0] = support.Symbol(i);
        Text::Draw(support.X(i), y, buffer);
    }
}


static void OnDraw_TuneParameter()
{
    if(support.parameter == nullptr)
    {
        return;
    }

    Font::Store();
    Font::Set(TypeFont::_GOSTB20);
    Text::SetUpperCase(false);

    int height = Wave::Graphics::Height() - 4;

    Painter::FillRegion(support.X0(), support.Y0(), support.Width(), height, Color::BLACK);

    DrawNameParameter(support.X0(), support.Y0(), support.Width());

    DrawParameter(support.Y0() + 60);

    Font::Restore();
}
