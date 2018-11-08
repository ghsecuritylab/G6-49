#pragma once
#include "Utils/Buffer.h"


class Interface
{
public:

    /// ��������� ��� ������ � �����������
    struct Data
    {
    public:

        Data(uint size);
        /// ���������������� ��������� ��� �������� size ���� ������
        bool Init(uint size);
        /// ���������� ������, �������������� ����������. ����������� ����� �� ����� �������������
        void Release();
        /// ���������� true, ���� ����� ���� - ������� ������
        bool IsEmpty() const;
        /// ���������� ��������� �� ������
        uint8 *GetData();
        /// ���������� ������ ������ ������
        uint GetSize() const;
    private:
        uint8 *data;
        uint size;
    };

    /// ��� ����� ��������� �� ����������
    static void Update();
    /// ������� � ��������� ������
    static void Send(const uint8 *buffer, uint size);

    static void Send(const Buffer &buffer);
    /// ������� ������ � ������� ������.
    static void Request(Data *request, Data *answer);

private:
    /// ������� � ���������� ������ �� ����
    static void ProcessDataFPGA();
    /// ������� numBytes ���� �� ���� � ��������� ��
    static void ReceiveAndRun(uint16 numBytes);
};
