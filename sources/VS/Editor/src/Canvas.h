#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)
#include "Colors.h"


class Canvas : public wxPanel
{
public:
    Canvas(wxWindow *parent);
    ~Canvas();
    void Draw();
    void SetColor(const Color &color = Color::NUMBER);
    void SetPoint(int x, int y, const Color &color = Color::NUMBER);
    void SetPoint(int x, int y, int size, const Color &color = Color::NUMBER);
    void DrawLine(int x0, int y0, int x1, int y1, const Color &color = Color::NUMBER);
    void Redraw();

private:
    /// true, ���� ��� ��������� � ������� ���������
    bool mouseIsDown = false;
    int mouseX = 0;
    int mouseY = 0;
    wxWindow *parent;

    void BeginScene();
    void EndScene();
    void DrawGrid();
    void OnPaint(wxPaintEvent &);
    void OnResize(wxSizeEvent &);
    void OnMouseMove(wxMouseEvent &);
    void OnMouseLeftDown(wxMouseEvent &);
    void OnMouseRightDown(wxMouseEvent &);
    void OnMouseUp(wxMouseEvent &);
    /// ���������� ����������� ����� ������� ����
    void SetMouseCursor();
};


extern Canvas *TheCanvas;
