#include "PageDebug.h"
#include "Settings/Settings.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Display/Symbols.h"
#include "Display/WaveGraphics.h"
#include "Menu/Pages/PageService.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include "Utils/NumberBuffer.h"


#define X_INPUT      5
#define Y_INPUT      183
#define WIDTH_INPUT  240
#define HEIGHT_INPUT 52



extern const PageBase pRegisters;
extern const PageBase pDebug;
extern const ButtonBase bSend;


static Item emptyItem = {Item::Type::None};

Page *PageDebug::self = reinterpret_cast<Page *>(const_cast<PageBase *>(&pDebug));
/// �������, � ������� ����� ������������ ��������� �������� �� ������� ������ �������
Register::E currentRegister = Register::FreqMeterLevel;
/// ���� ������ ���� �����
static bool showInputWindow = false;
#define MAX_SIZE_BUFFER 14
/// ����� �������� �������� �������
static char buffer[MAX_SIZE_BUFFER + 1];

/// ��� �������� ����� � ���� �����
enum TypeInput
{
    Uint,           ///< ���������� �����. ����� ������ �������� �� 2^64.
    Binary,         ///< ������������������ ����� � ������
    Uint10_Uint10,  ///< ��� ������������ ����� � ���������� ����.
    Uint14_Uint14   ///< ��� �����, �������� 2^14, � ���������� ����
};

struct DescInput
{
    int size;
    TypeInput type;
    bool sending;       // true ��������, ��� �������� � ���� ������� ��� ����������
    uint8 nu[7];
    uint64 value;       // ��������� ��������
};

#define VALUE(i)   (desc[i].value)
#define SENDING(i) (desc[i].sending)

static DescInput desc[Register::Count] =
{
    {2,  Uint          , false, 0 }, // Multiplexor1,
    {2,  Uint          , false, 0 }, // Multiplexor2,
    {10, Uint          , false, 0 }, // OffsetA,
    {10, Uint          , false, 0 }, // OffsetB,
    {10, Uint          , false, 0 }, // FreqMeterLevel,
    {10, Uint          , false, 0 }, // FreqMeterHYS,
    {8,  Binary        , false, 0 }, // FPGA_RG0_Control,
    {13, Uint          , false, 0 }, // FPGA_RG1_Freq,
    {11, Uint10_Uint10 , false, 0 }, // FPGA_RG2_Amplitude,
    {11, Uint14_Uint14 , false, 0 }, // FPGA_RG3_RectA,
    {11, Uint14_Uint14 , false, 0 }, // FPGA_RG4_RectB,
    {10, Uint          , false, 0 }, // FPGA_RG5_PeriodImpulseA,
    {10, Uint          , false, 0 }, // FPGA_RG6_DurationImpulseA,
    {10, Uint          , false, 0 }, // FPGA_RG7_PeriodImpulseB,
    {10, Uint          , false, 0 }, // FPGA_RG8_DurationImpulseB,
    {12, Binary        , false, 0 }, // FPGA_RG9_FreqMeter
    {11, Uint14_Uint14 , false, 0 }, // FPGA_RG10_Offset
    {0},
    {0},
    {0}
};




/// ���������� ������ ������ ��� �������� i
static int SizeBuffer(Register::E name = Register::Count);
/// ���������� ��� ����� ��� �������� i
static TypeInput TypeBuffer(Register::E name = Register::Count);
/// ��������� true, ���� ������ �������� ���������� ��� ������� ���� �����
static bool AllowableSymbol(Key key);
/// ������� �������� �������� i
static void DrawValue(int x, int y, uint8 i);
/// ���������� �� ������ ��������, �������������� �����
static uint64 FirstValue();
/// ���������� �� ������ ��������, ��������� �� ������
static uint64 SecondValue();
/// ����������� ������ �� buffer � uint
static uint64 BufferToValue();

static void OnPress_Send();

static void OnPress_Cancel();




static void LoadRegister()
{
    volatile uint64 value = BufferToValue();

    VALUE(currentRegister) = value;
    SENDING(currentRegister) = true;
    PGenerator::LoadRegister(currentRegister, VALUE(currentRegister));
}


static bool OnRegulator(Key key)
{
    if (TypeBuffer(currentRegister) == Uint)
    {
        if (key.IsRotate())
        {
            NumberBuffer::ProcessKey(key);
            LoadRegister();
            return true;
        }
    }

    return false;
}


static bool OnKey_PageRegisters(const Key &key)
{
    if (!showInputWindow)
    {
        if (AllowableSymbol(key))
        {
            SENDING(currentRegister) = false;
            OnPress_Send();
            std::memset(buffer, 0, MAX_SIZE_BUFFER + 1);
            buffer[0] = key.ToChar();
            NumberBuffer::Set(buffer, MAX_SIZE_BUFFER, 1, (currentRegister == Register::FreqMeterLevel ||
                                                           currentRegister == Register::FreqMeterHYS) ? 4095 : 0);
            return true;
        }
    }
    else if (key.IsDown())
    {
        if (AllowableSymbol(key))
        {
            NumberBuffer::ProcessKey(key);
            return true;
        }
        else if (key.IsCursors())
        {
            NumberBuffer::ProcessKey(key);
            return true;
        }
        else if (key.Is(Key::Esc))
        {
            OnPress_Cancel();
            return true;
        }
        else
        {
            return OnRegulator(key);
        }
    }
    else
    {
        // ����� ������
    }

    return false;
}


static int SizeBuffer(Register::E name)
{
    name = (name == Register::Count) ? currentRegister : name;

    return desc[name].size;
}


static TypeInput TypeBuffer(Register::E name)
{
    name = (name == Register::Count) ? currentRegister : name;

    return desc[name].type;
}


uint64 FirstValue()
{
    char buff[20];

    for (uint i = 0; i < sizeof(buffer); i++)
    {
        if (buffer[i] == '.')
        {
            for (uint j = 0; j < i; j++)
            {
                buff[j] = buffer[j];
            }
            buff[i] = 0;

            uint64 result = 0;

            if (SU::String2UInt64(buff, &result))
            {
                return result;
            }
            break;
        }
    }

    return 0;
}


uint64 SecondValue()
{
    for (uint i = 0; i < sizeof(buffer); i++)
    {
        if (buffer[i] == '.')
        {
            uint64 result = 0;

            if (SU::String2UInt64(&buffer[i + 1], &result))
            {
                return result;
            }
            break;
        }
    }

    return 0;
}


static uint64 BufferToValue()
{
    TypeInput type = TypeBuffer(currentRegister);

    uint64 result = 0;

    if (type == Uint)
    {
        if (!SU::String2UInt64(buffer, &result))
        {
            result = 0;
        }
    }
    else if (type == Binary)
    {
        result = SU::StringToBin32(buffer);
    }
    //else if (type == Uint10_Uint10 || type == Uint14_Uint14)
    else
    {
        int numBits = type == Uint10_Uint10 ? 10 : 14;

        uint64 first = FirstValue();
        uint64 second = SecondValue();

        result = first + (second << numBits);
    }

    return result;
}


static bool AllowableSymbol(Key key)
{
    TypeInput type = TypeBuffer(currentRegister);

    if (type == Uint)
    {
        return key.IsDigit();
    }
    else if (type == Binary)
    {
        return key.Is(Key::_0) || key.Is(Key::_1);
    }
    else // if (type == Uint10_Uint10 || type == Uint14_Uint14)
    {
        if (key.IsDigit())
        {
            return true;
        }

        if (key.Is(Key::Dot))
        {
            for (int i = 0; i < sizeof(buffer); i++)
            {
                if (buffer[i] == '.')
                {
                    return false;
                }
            }
            return true;
        }
    }

    return false;
}


void PageDebug::PageRegisters::Draw()
{
    if (CURRENT_PAGE != reinterpret_cast<Page *>(const_cast<PageBase *>(&pRegisters)))
    {
        return;
    }

    Painter::FillRegion(WaveGraphics::X(), WaveGraphics::Y(Chan::A) + 1, WaveGraphics::Width() - 1, WaveGraphics::Height() * 2, Color::BACK);

    DrawRegisters(WaveGraphics::X() + 4, WaveGraphics::Y(Chan::A) + 3);

    DrawInputWindow();
}


void PageDebug::PageRegisters::DrawRegisters(int x, int y)
{
    for (uint8 i = 0; i < Register::Count; i++)
    {
        Register reg(i);
        Color color = Color::FILL;
        if (i == currentRegister)
        {
            Painter::FillRegion(x - 1, y + i * 10, 132, 8, Color::FILL);
            color = Color::BACK;
        }
        String(reg.Name()).Draw(x, y + i * 10, color);
        DrawValue(x + 135, y + i * 10, i);
    }
}


void PageDebug::PageRegisters::DrawInputWindow()
{
    if (!showInputWindow)
    {
        return;
    }

    Painter::DrawRectangle(X_INPUT, Y_INPUT, WIDTH_INPUT, HEIGHT_INPUT, Color::FILL);
    Painter::FillRegion(X_INPUT + 1, Y_INPUT + 1, WIDTH_INPUT - 2, HEIGHT_INPUT - 2, Color::BACK);
    Register reg(currentRegister);
    Text::DrawBigText(X_INPUT + 3, Y_INPUT + 2, 2, reg.Name(), Color::FILL);

    int x = X_INPUT + 5;

    int position = NumberBuffer::PositionCursor();

    int size = static_cast<int>(std::strlen(buffer));

    for (int i = 0; i < size; i++)
    {
        if (i == position)
        {
            Painter::DrawFilledRectangle(x - 2, Y_INPUT + 19, 19, 31, Color::GRAY_10, Color::BLUE);
            Color::FILL.SetAsCurrent();
        }
        x = Text::DrawBigChar(x, Y_INPUT + 20, 4, buffer[i]) + 3;
    }
    if (position == static_cast<int>(std::strlen(buffer)) && position < SizeBuffer())
    {
        Painter::DrawFilledRectangle(x - 2, Y_INPUT + 19, 19, 31, Color::GRAY_10, Color::BLUE);
    }
}


static void DrawValue(int x, int y, uint8 i)
{
    if (!SENDING(i))
    {
        return;
    }

    Color::FILL.SetAsCurrent();

    Register name(i);

    TypeInput type = TypeBuffer(name);

    if (type == Uint)
    {
        Text::DrawFormatText(x, y, SU::UInt64_2String(VALUE(i)));
    }
    else if (type == Binary)
    {
        char buf[33];

        Text::DrawFormatText(x, y, SU::Bin2StringN((uint)VALUE(i), buf, SizeBuffer(name)));
    }
    else // if (type == Uint10_Uint10 || type == Uint14_Uint14)
    {
        uint mask = type == Uint10_Uint10 ? 0x3ffU : 0x3fffU;
        int numBits = type == Uint10_Uint10 ? 10 : 14;

        uint first = VALUE(i) & mask;
        uint second = (VALUE(i) >> numBits) & mask;
        x = Text::DrawFormatText(x, y, SU::UInt2String(first));
        x = Text::DrawFormatText(x, y, ".");
        Text::DrawFormatText(x, y, SU::UInt2String(second));
    }
}


static void OnPress_DebugMode(bool)
{
    PGenerator::SetDebugMode(DEBUG_MODE_ENABLED);
}

DEF_CHOICE_2( cConsole,                                                                                                                                           //--- ������� - ������� ---
    "�������", "CONSOLE",
    "�������� � ��������� ����������� ���������� �������", "Turns the debug console display on or off",
    DISABLED_RU,                     DISABLED_EN,
    "����������� ������� ���������", "Console display off",
    ENABLED_RU,                      ENABLED_EN,
    "����������� ������� ��������",  "Console Display Enabled",
    FLAG, BIT_CONSOLE, pDebug, Item::FuncActive, OnPress_DebugMode, FuncDraw
)


static void OnPress_SaveSettings()
{
    set.SaveToMemory();
}

DEF_BUTTON( bSaveSettings,                                                                                                                            //--- ������� - ��������� ��������� ---
    "��������� ���������", "SAVE SETTINGS",
    "��������� ������� ���������", "Save current settings",
    pDebug, Item::FuncActive, OnPress_SaveSettings, FuncDraw
)


DEF_CHOICE_2( cStatistics,                                                                                                                                     //--- ������� - ���������� ---
    "����������", "STATISTICS",
    "��������/��������� ����� �� ������ ���������� �� ������� ������ � ���� ��� / ����� ��������� ������ ����� / �����, ���������� �� ��������� �� ���� ����",
    "Enables / disables the display on the screen of statistics on the frame rate in the form of FPS / time for rendering one frame / time spent on rendering for the entire frame",
    DISABLED_RU,                 DISABLED_EN,
    "����� ���������� ��������", "Show statistics disabled",
    ENABLED_RU,                  ENABLED_EN,
    "����� ���������� �������",  "Show statistics enabled",
    FLAG, BIT_STATISTICS, pDebug, Item::FuncActive, FuncChangedChoice, FuncDraw
)


DEF_CHOICE_2( cShowSends,                                                                                                                            //--- ������� - ���������� ��������� ---
    "���������� ���������", "Show options",
    "���������� ���������, ���������� � ����", "Shows parameters sent to FPGA",
    DISABLED_RU,                 DISABLED_EN,
    "����� ���������� ��������", "Parameter display disabled",
    ENABLED_RU,                  ENABLED_EN,
    "����� ���������� �������",  "Parameter display enabled",
    FLAG, BIT_SHOW_SENDS, pDebug, Item::FuncActive, FuncChangedChoice, FuncDraw
)


static void OnPress_Prev()
{
    Math::CircleDecrease<uint8>(reinterpret_cast<uint8 *>(&currentRegister), 0, Register::Count - 1);
}

DEF_BUTTON( bPrev,                                                                                                                                            //--- �������� - ���������� ---
    "����������", "Previous",
    "������� � ����������� ��������", "Go to previous register",
    pRegisters, Item::FuncActive, OnPress_Prev, FuncDraw
)


static void OnPress_Next()
{
    Math::CircleIncrease<uint8>(reinterpret_cast<uint8 *>(&currentRegister), 0, Register::Count - 1);
}

DEF_BUTTON( bNext,                                                                                                                                             //--- �������� - ��������� ---
    "���������", "Next",
    "������� � ���������� ��������", "Go to the next register",
    pRegisters, Item::FuncActive, OnPress_Next, FuncDraw
)


static void OnPress_Cancel()
{
    showInputWindow = false;
    std::memset(buffer, 0, MAX_SIZE_BUFFER + 1);
    pRegisters.items[0] = reinterpret_cast<Item*>(const_cast<ButtonBase*>(&bPrev));
    pRegisters.items[1] = reinterpret_cast<Item*>(const_cast<ButtonBase*>(&bNext));
    pRegisters.items[2] = reinterpret_cast<Item*>(const_cast<ButtonBase*>(&bSend));
}

static void OnDraw_Cancel(int x, int y)
{
    Text::Draw4SymbolsInRect(x + 15, y + 30, Ideograph::_8::Delete, Color::FILL);
}

DEF_BUTTON( bCancel,                                                                                                                               //--- �������� - Input window - ������ ---
    "������", "Cancel",
    "�������� ������� �������� � ������� � ��������� ���� �����", "Cancels registering a value and closes the input window",
    pRegisters, Item::FuncActive, OnPress_Cancel, OnDraw_Cancel
)


static void OnDraw_Backspace(int x, int y)
{
    Text::Draw4SymbolsInRect(x + 15, y + 30, Ideograph::_8::Left, Color::FILL);
}

DEF_BUTTON( bBackspace,                                                                                                                         //--- �������� - Input window - Backspace ---
    "Backspace", "Backspace",
    "������� ��������� �������� ������", "Deletes the last character entered",
    pRegisters, Item::FuncActive, NumberBuffer::PressBackspace, OnDraw_Backspace
)


static void OnPress_Save()
{
    LoadRegister();
    OnPress_Cancel();
}

static void OnDraw_Save(int x, int y)
{
    Text::Draw4SymbolsInRect(x + 15, y + 30, Ideograph::_8::Save, Color::FILL);
}

DEF_BUTTON( bSave,                                                                                                                                //--- �������� - ���� ����� --- ������� ---
    "�������", "Send",
    "���������� �������� � ��������� �������", "Writes a value to the selected register",
    pRegisters, Item::FuncActive, OnPress_Save, OnDraw_Save
)


static void OnPress_Send()
{
    showInputWindow = true;
    std::memset(buffer, 0, MAX_SIZE_BUFFER + 1);

    pRegisters.items[0] = reinterpret_cast<Item*>(const_cast<ButtonBase*>(&bBackspace));
    pRegisters.items[1] = reinterpret_cast<Item*>(const_cast<ButtonBase*>(&bCancel));
    pRegisters.items[2] = reinterpret_cast<Item*>(const_cast<ButtonBase*>(&bSave));

    int position = 0;

    if (SENDING(currentRegister))
    {
        TypeInput type = TypeBuffer();

        if (type == Uint)
        {
            SU::UInt64_2String(VALUE(currentRegister), buffer);
            position = static_cast<int>(std::strlen(buffer));
        }
        else if (type == Binary)
        {
            SU::Bin2StringN((uint)VALUE(currentRegister), buffer, SizeBuffer(currentRegister));
            position = static_cast<int>(std::strlen(buffer));
        }
        else // if (type == Uint10_Uint10 || type == Uint14_Uint14)
        {
            uint mask = type == Uint10_Uint10 ? 0x3ffU : 0x3fffU;
            int numBits = type == Uint10_Uint10 ? 10 : 14;

            uint first = VALUE(currentRegister) & mask;
            uint second = (VALUE(currentRegister) >> numBits)& mask;

            std::strcpy(buffer, SU::UInt2String(first));
            std::strcat(buffer, ".");
            std::strcat(buffer, SU::UInt2String(second));
        }
    }
    else
    {
        position = 0;
        std::memset(buffer, 0, MAX_SIZE_BUFFER + 1);
        VALUE(position) = 0;
    }

    NumberBuffer::Set(buffer, desc[currentRegister].size, position, (currentRegister == Register::FreqMeterLevel ||
        currentRegister == Register::FreqMeterHYS) ? 4095 : 0);
}

DEF_BUTTON( bSend,                                                                                                                                               //--- �������� - ������� ---
    "�������", "Send",
    "��������� ���� ����� �������� ��������", "Opens a register value input window",
    pRegisters, Item::FuncActive, OnPress_Send, FuncDraw
)


DEF_PAGE_4_VAR( pRegisters,                                                                                                                                       //--- �������� --- //-V641
    "��������", "REGISTERS",   //-V641
    "", "",
    bPrev,
    bNext,
    bSend,
    emptyItem,
    Page::Registers, &pDebug, Item::FuncActive, Page::FuncEnter, OnKey_PageRegisters, FuncBeforeDraw
)


DEF_PAGE_8( pDebug,                                                                                                                                                 //--- ������� --- //-V641
    "�������", "DEBUG",   //-V641
    "", "",
    PageDebug::Calibartion::self,
    &pRegisters,                                ///< ������� - ��������
    PageDebug::SubRange::self,
    PageDebug::Colors::self,
    &cConsole,                                  ///< ������� - �������
    &cStatistics,                               ///< ������� - ����������
    &cShowSends,                                ///< ������� - ���������� ���������
    &bSaveSettings,                             ///< ������� - ��������� ���������
    Page::Debug, PageService::self, Item::FuncActive, Page::FuncEnter, FuncOnKey, Page::FuncDraw
)



/*
static void OnPress_BigSymbols(bool)
{
    Font::ToggleCharacterSize();
}

DEF_CHOICE_2 (  cBigSymbols,                                                                                                                              //--- ������� - ������� ������� ---
    "������� �������", "Large characters",
    "����������� ���������� ����� �������� ���������",
    "Display information with larger characters",
    DISABLED_RU, DISABLED_EN,   "����������� ���������� ������������ ���������",
                                "Displaying information with standard symbols",
    ENABLED_RU, ENABLED_EN,     "����������� ���������� ������������ ���������",
                                "Displaying information with enlarged symbols",
    FLAG, BIT_BIG_SYMBOLS, pDebug, FuncActive, OnPress_BigSymbols, FuncDraw
)
*/
