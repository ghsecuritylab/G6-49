#include "defines.h"
#include "globals.h"
#include "Settings.h"
#include "Display/Colors.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Generator.h"
#include "Hardware/CPU.h"


#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4310)
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const Settings defSet =
{
    sizeof(Settings),
    {
    },
    {
        MAKE_COLOR(0x00, 0x00, 0x00),   // BLACK
        MAKE_COLOR(0xff, 0xff, 0xff),   // WHITE
        MAKE_COLOR(25,   25,   25),     // GRAY_10
        MAKE_COLOR(0x40, 0x40, 0x40),   // GRAY_25
        MAKE_COLOR(0x80, 0x80, 0x80),   // GRAY_50
        MAKE_COLOR(0xc0, 0xc0, 0xc0),   // GRAY_75
        MAKE_COLOR(0x00, 0x00, 0xff),   // BLUE
        MAKE_COLOR(0x00, 0x00, 25),     // BLUE_10
        MAKE_COLOR(0x00, 0x00, 0x40),   // BLUE_25
        MAKE_COLOR(0x00, 0x00, 0x80),   // BLUE_50
        MAKE_COLOR(0x00, 0x00, 0xc0),   // BLUE_75
        MAKE_COLOR(0x00, 0xff, 0x00),   // GREEN
        MAKE_COLOR(0x00, 13,   0x00),   // GREEN_5
        MAKE_COLOR(0x00, 25,   0x00),   // GREEN_10
        MAKE_COLOR(0x00, 0x40, 0x00),   // GREEN_25
        MAKE_COLOR(0x00, 0x80, 0x00),   // GREEN_50
        MAKE_COLOR(0x00, 0xc0, 0x00),   // GREEN_75
        MAKE_COLOR(0xff, 0x00, 0x00),   // RED
        MAKE_COLOR(0x40, 0x00, 0x00),   // RED_25
        MAKE_COLOR(0x80, 0x00, 0x00),   // RED_50
        MAKE_COLOR(0xc0, 0x00, 0x00)    // RED_75
    },
    { Sine, Sine },                     // sig_form[NumChannels]
    { 
        Frequency,                      // sig_parameter[NumForms]
        Frequency,
        Frequency
    },
    0,                   // menu_currentPage
    (Page*)0,            // menu_page
    {0},                 // menu_posActItem[]
    {0},                 // menu_currentSubPage[]
    BillingTime_1ms,     // freq_billingTime

    BINARY_U8(00001100), // flag1
//            ||||||||------ 0 BIT_PARITY         Parity_Off
//            |||||||------- 1 BIT_FREQ_INTERVAL  Interval_1
//            ||||||-------- 2 BIT_CONSOLE      
//            |||||--------- 3 BIT_BACK_BLACK   
//            ||||---------- 4 BIT_TUNE_FULL    
//            |||----------- 5 BIT_DBG_MODE     
//            ||------------ 6 BIT_FREQ_ENABLED 
//            |------------- 7 BIT_STATISTICS   

    BINARY_U8(00010110)  // flag2
//              ||||||------ 0 BIT_LANGUAGE       RU
//              |||||------- 1 BIT_CHAN_A   
//              ||||-------- 2 BIT_CHAN_B   
//              |||--------- 3 BIT_CHANNEL        A
//              ||---------- 4 BIT_SIZE_BYTE      8bit
//              |----------- 5 BIT_STOP_BIT       StopBit_1
};

#ifdef WIN32
#pragma warning(pop)
#endif

Settings set = defSet;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const float minMax[NumParameters][2] =
{
    {1.0f,    50e6f},       // Frequency
    {2e-08f,  1.0f},        // Period
    {0.000f,  65535.0f},    // Amplitude
    {0.0f,    4095.0f},     // Offset
    {2e-08f,  10.0f},       // Duration
    {0.001f,  1.0f},        // DutyRatio
    {-180.0f, 180.0f},      // Phase
    {2e-08f,  10e08f}       // Delay
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float MinValue(Type_WaveParameter param)
{
    return minMax[param][0];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
float MaxValue(Type_WaveParameter param)
{
    return minMax[param][1];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Settings::Save()
{
    CPU::FLASH_::SaveSettings();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Settings::Load(bool _default)
{
    set = defSet;

    if(_default)
    {
        set = defSet;
    }
    else
    {
        CPU::FLASH_::LoadSettings();
    }

    TuneGenerator(A);
    TuneGenerator(B);
    
    Painter::LoadPalette();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void TuneGenerator(Channel ch)
{
    Generator::EnableChannel(ch, CHANNEL_ENABLED(ch));
    WaveForm form = WAVE_FORM_CH(ch);
    Generator::SetFormWave(ch, WAVE_FORM_CH(ch));
    Generator::SetParameter(ch, Frequency, (&INPUT_WINDOW_STRUCT(ch, form, Frequency))->Value());
    Generator::SetParameter(ch, Amplitude, (&INPUT_WINDOW_STRUCT(ch, form, Amplitude))->Value());
    Generator::SetParameter(ch, Offset, (&INPUT_WINDOW_STRUCT(ch, form, Offset))->Value());
    if (form.type == Impulse)
    {
        Generator::SetParameter(ch, Duration, (&INPUT_WINDOW_STRUCT(ch, form, Duration))->Value());
    }
    Generator::SetParameter(ch, Offset,    (&INPUT_WINDOW_STRUCT(ch, WAVE_FORM_CH(ch), Offset))->Value());
}
