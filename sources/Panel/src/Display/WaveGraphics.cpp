#include "Display/Painter.h"
#include "Display/Text.h"
#include "Display/WaveGraphics.h"
#include "Generator/Signals.h"
#include "Generator/Viewer.h"
#include "Menu/Pages/PageSignals/PageSignals.h"
#include "Settings/Settings.h"


void WaveGraphics::Draw(Chan::E ch)
{
    if(FREQ_METER_ENABLED && ch != CURRENT_CHANNEL)
    {
        return;
    }

    int x0 = X();
    int y0 = Y(ch);
    Painter::FillRegion(x0 + 1, y0 + 1, Width() - 2, Height() - 2, Color::GREEN_5);
    if (CHANNEL_ENABLED(ch))
    {
        Painter::DrawRectangle(x0, y0, Width(), Height(), Color::FILL);

        Font::Store();
        Font::Set(TypeFont::_GOSTB20);
        Text::Draw(x0 + 5, y0 + 5, (ch == Chan::A) ? "A" : "B", Color::Chan(ch));
        Font::Restore();

        FORM(ch)->DrawUGO(ch, y0);
        DrawParameters(ch, y0);
    }
}


int WaveGraphics::X()
{
    return 0;
}


int WaveGraphics::Y(Chan::E ch)
{
    return (ch == Chan::A || FREQ_METER_ENABLED) ? Page::Title::HEIGHT : Page::Title::HEIGHT + SIGNAL_HEIGHT;
}


int WaveGraphics::Width()
{
    return SIGNAL_WIDTH;
}


int WaveGraphics::Height()
{
    return SIGNAL_HEIGHT;
}


void WaveGraphics::DrawParameters(Chan::E ch, int y0)
{
    Form *form = FORM(ch);

    int x0 = 107;

    y0 += 5;

    String(form->Name(LANGUAGE)).Draw(22, y0 + 3, Color::Chan(ch));
    
    for (int i = 0; i < form->NumParameters(); i++)
    {
        form->GetParameter(i)->viewer.Draw(x0, y0);

        y0 += 11;
    }
}
