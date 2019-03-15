#include "defines.h"
#pragma warning(push)
#pragma warning(disable:4018 4076 4091 4189 4365 4459 4571 4625 4668 5026 5027 5039)
#undef CRC

#include "Application.h"


#define uint    unsigned int
#define int8    signed char
#define uint8   unsigned char
#define int16   signed short
#define uint16  unsigned short
#define uint    unsigned int
#define uchar   unsigned char
#define pString const char * const

#include "Display/Painter.h"
#include "Display/Text.h"

#include <SDL.h>

#include <wx/display.h>

#pragma warning(pop)

#undef uint   
#undef int8   
#undef uint8  
#undef int16  
#undef uint16 
#undef uint   
#undef uchar  
#undef pString


#include "defines.h"
#include "Menu/Menu.h"
#include "Utils/Math.h"
#include "Settings/Settings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SDL_Renderer *renderer = nullptr;
static SDL_Window *window = nullptr;
static SDL_Texture *texture = nullptr;

/// ����� �������� ��������� �� ������
//static wxButton *buttons[Key::Number] = { nullptr };

/// �����
static uint colors[256];


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ������ ���� ����������. ���������� ����� ������� ��� ���������
static HANDLE CreateFrame();
/// ���������� ����������� ������� ��� ���� ����������
static void SetPosition(Frame *frame);
/// �������� ���������� ������������� ���������� � ������� ��������
static wxRect GetMaxDisplay();
/// ������ ��� ������
static void CreateButtons(Frame *frame);
/// ������ ���� ������
//static void CreateButton(Key::E key, Frame *frame, const wxPoint &pos, const wxSize &size, pString title);
/// ������ ������ ��� ���� ������
//static void CreateButtonsChannel(Frame *frame, const char *title, int x, int y, Key::E keyChannel, Key::E keyRangeLess, Key::E keyRangeMore, Key::E keyRShiftLess, Key::E keyRShiftMore);
/// ������ ������ ������ �������������
static void CreateButtonsTrig(Frame *frame, int x, int y);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::Init()
{
    Text::SetUpperCase(true);

    Font::SetType(Font::Type::_8);

    HANDLE handle = CreateFrame();

    window = SDL_CreateWindowFrom(handle);

    if (window == nullptr)
    {
        std::cout << "SDL_CreateWindowFrom() Error: " << SDL_GetError() << std::endl;
    }
    else
    {
        std::cout << "Create SDL window is ok" << std::endl;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::BeginScene(Color color)
{
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_RENDERER_ACCELERATED, 320, 240);

    SDL_SetRenderTarget(renderer, texture);
    Painter::SetColor(color);
    SDL_RenderClear(renderer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::EndScene()
{
    SDL_SetRenderTarget(renderer, NULL);

    SDL_Rect rect = {0, 0, 320, 240};

    SDL_RenderCopy(renderer, texture, NULL, &rect); //-V2001
    SDL_RenderPresent(renderer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
void Painter::SetColorValue(Color color, uint value)
{
    colors[color.value] = value;
}
*/

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//void Painter::SetColor(Color color)
//{
//    if (color != Color::NUMBER)
//    {
//        uint value = colors[color.value];
//        uint8 blue = (uint8)value;
//        uint8 green = (uint8)(value >> 8);
//        uint8 red = (uint8)(value >> 16);
//        SDL_SetRenderDrawColor(renderer, red, green, blue, 0x00);
//    }
//}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void SetPosition(Frame *frame)
{
    wxSize size = { 329, 560 };
    
    frame->SetSize(size);
    frame->SetMinSize(size);
    frame->SetMaxSize(size);

    wxRect rect = GetMaxDisplay();

    frame->SetPosition({ rect.width / 2 - size.GetWidth() / 2, rect.height / 2 - size.GetHeight() / 2 });
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static wxRect GetMaxDisplay()
{
    wxRect result = {0, 0, 0, 0};

    for (uint i = 0; i < wxDisplay::GetCount(); i++)
    {
        wxDisplay display(i);

        wxRect rect = display.GetClientArea();
        if (rect.width > result.width)
        {
            result.width = rect.width;
            result.height = rect.height;
        }
    }

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static HANDLE CreateFrame()
{
    Frame *frame = new Frame("");

    SetPosition(frame);

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    wxButton *button = new wxButton(frame, wxID_ANY, "", {10, 10}, {320, 240});
    button->SetMaxSize({320, 240});

    sizer->Add(button);

    frame->SetSizer(sizer);

    CreateButtons(frame);

    frame->Show(true);

    return button->GetHandle();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void CreateButtons(Frame * /*frame*/)
{
    /*
    // ������ ������ ���� � ��������������

    Key::E keys[2][5] = 
    {
        { Key::F1,       Key::F2,      Key::F3,     Key::F4,      Key::F5 },
        { Key::Function, Key::Display, Key::Memory, Key::Measure, Key::Service }
    };

    int x0 = 5;
    int y0 = 250;

    int dX = 5;
    int dY = 5;

    int width = 58;
    int height = 25;

    wxSize size = {width, height};

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            Key::E key = keys[j][i];
            CreateButton(key, frame, {x0 + (width + dX) * i, y0 + (height + dY) * j}, size, Key(key).Name());
        }
    }

    // ������ ������ ����������

    width = height = 25;
    x0 = 320 / 2 - width / 2;
    y0 = 240 + 100;

    size.SetWidth(width);
    size.SetHeight(height);

    CreateButton(Key::Enter, frame, {x0, y0}, size, "E");
    CreateButton(Key::Left, frame, {x0 - dX - width, y0}, size, "L");
    CreateButton(Key::Right, frame, {x0 + dX + width, y0}, size, "R");
    CreateButton(Key::Up, frame, {x0, y0 - height - dY}, size, "U");
    CreateButton(Key::Down, frame, {x0, y0 + height + dY}, size, "D");

    // ������ �������

    width = 51;
    x0 = 5;

    y0 = 240 + 100;

    size.SetWidth(width);

    CreateButton(Key::TBaseLess, frame, {x0, y0}, size, "�");
    CreateButton(Key::TBaseMore, frame, {x0 + width + dY, y0}, size, "��");
    y0 += height + dY;
    CreateButton(Key::TShiftLess, frame, {x0, y0}, size, "<-");
    CreateButton(Key::TShiftMore, frame, {x0 + width + dY, y0}, size, "->");

    int x = 5 + (2 * width + dX) / 2 - width / 2;

    CreateButton(Key::Time, frame, {x, y0 - height - dY - height - dY}, size, "��������");

    // ������ ������ A

    int y = 240 + 200;

    CreateButtonsChannel(frame, "����� 1", 5, y, Key::ChannelA, Key::RangeLessA, Key::RangeMoreA, Key::RShiftLessA, Key::RShiftMoreA);

    // ������ ������ B

    CreateButtonsChannel(frame, "����� 1", 120, y, Key::ChannelB, Key::RangeLessB, Key::RangeMoreB, Key::RShiftLessB, Key::RShiftMoreB);

    CreateButtonsTrig(frame, 235, y - 130);

    CreateButton(Key::Start, frame, { 230, 438 }, { 80, 25 }, "����/����");
    */
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
static void CreateButton(Key::E key, Frame *frame, const wxPoint &pos, const wxSize &size, pString title)
{
    wxButton *button = new wxButton(frame, (wxWindowID)key, title, pos, size);

    button->Connect((wxWindowID)key, wxEVT_LEFT_DOWN, wxCommandEventHandler(Frame::OnDown));
    button->Connect((wxWindowID)key, wxEVT_LEFT_UP, wxCommandEventHandler(Frame::OnUp));

    buttons[key] = button;
}
*/

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
static void CreateButtonsChannel(Frame *frame, const char *title, int x, int y, Key::E keyChannel, Key::E keyRangeLess, Key::E keyRangeMore, Key::E keyRShiftLess, Key::E keyRShiftMore)
{
    int width = 45;
    int height = 20;

    int dX = 5;
    int dY = 5;

    wxSize size = {width, height};

    CreateButton(keyRangeLess, frame, {x, y}, size, "��");
    CreateButton(keyRangeMore, frame, {x, y + height + dY}, size, "�");

    CreateButton(keyRShiftMore, frame, {x + width + 2 * dX, y}, size, "+");
    CreateButton(keyRShiftLess, frame, {x + width + 2 * dX, y + height + dY}, size, "-");

    size.SetHeight(25);
    size.SetWidth(width + width + dX * 2);

    wxPoint pos = {x, y - dY - size.GetHeight()};

    CreateButton(keyChannel, frame, pos, size, title);
}
*/

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
static void CreateButtonsTrig(Frame *frame, int x, int y)
{
    int width = 45;
    int height = 20;

    wxSize size = { width, height };

    CreateButton(Key::Trig, frame, { x, y }, size, "�����");
    CreateButton(Key::TrigLevMore, frame, { x, y + 30 }, size, "������");
    CreateButton(Key::TrigLevLess, frame, { x, y + 60 }, size, "������");
}
*/

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Frame::OnDown(wxCommandEvent & /*event*/)
{
    /*
    Key::E key = (Key::E)event.GetId();

    std::cout << "down " << Key(key).Name() << std::endl;
    event.Skip();

    Menu::ButtonEvent(KeyEvent(key, TypePress::Press));
    */
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Frame::OnUp(wxCommandEvent & /*event*/)
{
    /*
    Key::E key = (Key::E)event.GetId();

    std::cout << "up   " << Key(key).Name() << std::endl;
    event.Skip();

    Menu::ButtonEvent(KeyEvent(key, TypePress::Release));
    */
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::FillRegion(int x, int y, int width, int height, Color color /* = Color::NUMBER */)
{
    SetColor(color);
    SDL_Rect rect = { x, y, width + 1, height + 1 };
    SDL_RenderFillRect(renderer, &rect);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetPoint(int x, int y)
{
    SDL_RenderDrawPoint(renderer, x, y);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetColor(Color color)
{
    if (color != Color::NUMBER)
    {
        uint value = COLOR(color.value);
        uint8 blue = (uint8)value;
        uint8 green = (uint8)(value >> 8);
        uint8 red = (uint8)(value >> 16);
        SDL_SetRenderDrawColor(renderer, red, green, blue, 0x00);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawRectangle(int x, int y, int width, int height, Color color /* = Color::NUMBER */)
{
    SetColor(color);
    SDL_Rect rect = { x, y, width + 1, height + 1 };
    SDL_RenderDrawRect(renderer, &rect);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawVLine(int x, int y0, int y1, Color color /* = Color::NUMBER */)
{
    SetColor(color);
    SDL_RenderDrawLine(renderer, x, y0, x, y1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawHLine(int y, int x0, int x1, Color color /* = Color::NUMBER */)
{
    SetColor(color);
    SDL_RenderDrawLine(renderer, x0, y, x1, y);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawLine(int x0, int y0, int x1, int y1, Color color)
{
    SetColor(color);
    SDL_RenderDrawLine(renderer, x0, y0, x1, y1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawFilledRectangle(int x, int y, int width, int height, Color colorFill, Color colorRect)
{
    DrawRectangle(x, y, width, height, colorRect);
    FillRegion(x + 1, y + 1, width - 2, height - 2, colorFill);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::LoadPalette()
{

}
