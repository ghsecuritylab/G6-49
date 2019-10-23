#pragma once
#include "Message.h"


class String;



class File
{
public:
    File();
    ~File();
    /// ��������� ����
    void Open(int num);
    /// ���������� ����������
    void Draw(int x, int y);
    /// ��������� ���� � ����������� ������
    void Close();

    static bool Handler(SimpleMessage *msg);
    /// ��������� ������ ��� ������ �����
    static void SetDataToWave();
};


struct FileRequests
{
    /// ������� ������ �� �����
    static void SendRequestForString(const String *name, int numString);
};
