#include "Generator/Signals.h"
#include "InputWindow.h"
#include "DisplayTypes.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Display/Symbols.h"
#include "Utils/IntValue.h"
#include <cstdio>


ParameterValue *InputWindow::param = nullptr;
bool            InputWindow::isVisible = false;
StructValue     InputWindow::iws;


void InputWindow::Init()
{
    param = (ParameterValue *)PARAM_CURRENT;
    iws.Set(param);
    isVisible = true;
}


void InputWindow::DeInit()
{
    isVisible = false;
}


void InputWindow::Draw()
{
    if (!isVisible)
    {
        return;
    }

    Font::Set(TypeFont::_7);

    int x = 0;
    int y = Page::Title::HEIGHT;
    int width = SIGNAL_WIDTH;
    int height = SIGNAL_HEIGHT;

    if (CURRENT_CHANNEL_IS_A)
    {
        y += SIGNAL_HEIGHT;
    }

    Painter::FillRegion(x, y, width, height, Color::BACK);
    Painter::DrawRectangle(x, y, width, height, Color::FILL);

    struct StructFunc
    {
        pFuncVII func;
        StructFunc(pFuncVII f) : func(f) {};
    }
    func[ParameterValue::Count] =
    {
        DrawFrequency,
        DrawPeriod,
        DrawAmplitude,
        DrawOffset,
        DrawDuration,
        DrawDutyRatio,
        DrawPhase,
        DrawDelay,
        DrawDurationRise,
        DrawDurationFall,
        DrawDurationStady,
        DrawDutyFactor,
        DrawRampSineDuration,
        DrawRampSineAmplitude,
        DrawPacketPeriod,
        DrawPacketNumber,
        DrawExit
    };

    x += 40;

    y += 10;

    DrawDigits(x, y);

    func[param->value].func(x + 10, y + 10);

    Font::Set(TypeFont::_8);
}


// ������������� ������ ��������
static void TransformOffset(char *buffer, int *posComma)
{
    IntValue value(buffer, *posComma);

    value.Sub5(buffer, posComma);
}


void InputWindow::DrawDigits(int x, int y)
{
#define SIZE_TEXT       5
#define WIDTH_DIGIT     (4 * SIZE_TEXT - 1)
#define HEIGHT_DIGIT    (7 * SIZE_TEXT - 1)

    if (!FREQ_METER_ENABLED)
    {
        Font::Store();
        Font::Set(TypeFont::_GOSTB20);
        Text::SetUpperCase(false);
        Text::Draw(x, y, param->Name());
        Font::Restore();
    }

    y += 40;

    char text[10];
    std::strcpy(text, param->GetStringDigits());

    int posComma = param->posComma;

    int8 hightLightDigit = param->hightLightDigit;

    char *pointer = text;

    if (param->Is(ParameterValue::Offset))
    {
        TransformOffset(text, &posComma);
        pointer++;
        posComma--;
        Text::DrawBigChar(x - 24, y - 1, SIZE_TEXT, text[0]);
    }
    else
    {
        if (param->sign != ' ')
        {
            Text::DrawBigChar(x - 24, y - 1, SIZE_TEXT, param->sign);
        }
    }

    for (int i = 0; i < param->numDigits; i++)
    {
        Painter::FillRegion(x, y, WIDTH_DIGIT, HEIGHT_DIGIT, Color::BACK);

        if (i == hightLightDigit)
        {
            HighlightPosition(x + 5, y);
        }

        char buf[2] = { pointer[i], 0 };

        Font::Set(TypeFont::_GOST28);
        Text::Draw(x + 2, y + 4, buf, Color::FILL);

        x += 18;

        if (posComma == i)
        {
            Painter::FillRegion(x, y + 26, 2, 2);
            x += 4;
        }
    }

    DrawUnits(x, y);
}


void InputWindow::DrawUnits(int x, int y)
{
    char buffer[10] = { 0 };

    Font::Set(TypeFont::_GOST28);
    Text::SetUpperCase(false);
    Text::Draw(x + 5, y + 4, param->NameUnit(buffer));
}


void InputWindow::HighlightPosition(int x, int y)
{
    Font::Set(TypeFont::_8);
    Text::Draw4SymbolsInRect(x, y - 7, Ideograph::_8::FillDown, Color::GRAY_75);
    Text::Draw4SymbolsInRect(x, y + 30, Ideograph::_8::FillUp);
    Font::Set(TypeFont::_7);
}


void InputWindow::DrawDurationRise(int, int)
{
}


void InputWindow::DrawDurationFall(int, int)
{
}


void InputWindow::DrawDutyFactor(int, int)
{
}


void InputWindow::DrawDurationStady(int, int)
{
}


void InputWindow::DrawAmplitude(int, int)
{

}


void InputWindow::DrawFrequency(int, int)
{

}


void InputWindow::DrawOffset(int, int)
{

}


void InputWindow::DrawPhase(int, int)
{

}


void InputWindow::DrawDelay(int, int)
{

}


void InputWindow::DrawPeriod(int, int)
{

}


void InputWindow::DrawPacketNumber(int, int)
{
}


void InputWindow::DrawPacketPeriod(int, int)
{
}


void InputWindow::DrawDutyRatio(int, int)
{

}


void InputWindow::DrawDuration(int, int)
{

}


void InputWindow::DrawRampSineDuration(int, int)
{
}


void InputWindow::DrawRampSineAmplitude(int, int)
{
}


void InputWindow::DrawExit(int, int)
{
}


void InputWindow::KeyLeft()
{
    iws.KeyLeft();
}


void InputWindow::KeyRight()
{
    iws.KeyRight();
}


void InputWindow::OrderUp()
{
    param->IncreaseOrder();
}


void InputWindow::OrderDown()
{
    param->DecreaseOrder();
}


void InputWindow::KeyEnter()
{
    iws.SaveValue();
}


void InputWindow::ProcessContorl(Key &key)
{
    if (key.IsDigit() || key.Is(Key::Dot) || key.Is(Key::Minus))
    {
        if (key.action.Is(Key::Action::Down))
        {
            iws.PressKey(key);
        }
    }
    else if (key.Is(Key::RegLeft))
    {
        iws.RegLeft();
    }
    else if (key.Is(Key::RegRight))
    {
        iws.RegRight();
    }
    else if (key.IsFunctional() && key.action.Is(Key::Action::Up))
    {
        if(CURRENT_PAGE)
        {
            CURRENT_PAGE->GetItem(key)->Press(key);
        }
    }
    else if(key.Is(Key::Left, Key::Action::Up))
    {
        KeyLeft();
    }
    else if(key.Is(Key::Right, Key::Action::Up))
    {
        KeyRight();
    }
    else if(key.Is(Key::Esc, Key::Action::Up))
    {
        Menu::ResetAdditionPage();
    }
    else
    {
        // ����� ������ ����
    }
}
