#include "defines.h"
#include "Console.h"
#include "FreqMeter/FreqMeter_p.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Display/WaveGraphics.h"
#include "Generator/Wave.h"
#include "Hardware/CPU.h"
#include "Hardware/HAL/HAL.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Hint.h"
#include "Menu/Menu.h"
#include "Menu/Pages/PageDebug/PageDebug.h"
#include "Utils/Statistics.h"
#include <cstdlib>


uint8 Display::frontBuffer[BUFFER_WIDTH * BUFFER_HEIGHT];

uint8 Display::backBuffer[BUFFER_WIDTH * BUFFER_HEIGHT];



uint8 *Display::GetBuffer()
{
    return backBuffer;
}


void Display::Update()
{
    Statistics::BeginFrame();
    Painter::BeginScene(Color::BACK);
    WaveGraphics::Draw(Chan::A);
    WaveGraphics::Draw(Chan::B);
    Menu::Draw();
    PageDebug::PageRegisters::Draw();
    Painter::DrawRectangle(0, 0, 318, 238, Color::FILL);
    PFreqMeter::Draw();
    Hint::Draw();
    Statistics::Show();
    Console::Draw();
    Keyboard::Draw();
    Painter::EndScene();
    Statistics::EndFrame();
}
