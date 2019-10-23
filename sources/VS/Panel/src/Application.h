#pragma once
#include "wx/wx.h"



class Application : public wxApp
{
public:
    virtual bool OnInit() wxOVERRIDE;
};



class Frame : public wxFrame
{
public:
    Frame(const wxString& title);

    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnTimer(wxTimerEvent& event);

private:
    wxTimer timer;
    wxDECLARE_EVENT_TABLE(); //-V2504

    void DrawFPS();
    void HandlerEvents();

public:
    void OnDown(wxCommandEvent &event);
    void OnUp(wxCommandEvent &event);
};
