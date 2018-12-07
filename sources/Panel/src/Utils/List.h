#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T> class List;


template <class T>
class ListElement
{
friend class List<T>;

public:

    ListElement(T *v) : value(v) {};

    ~ListElement();

    T *Get();
    /// ���������� ��������� �� ��������� �������
    ListElement *Next();
private:
    /// ��������� �� ������ - ��������
    List<T> *owningList;
    /// ��������� �� ��������� �������. 0, ���� ��� ��� ��������� ������� � ������
    ListElement *next;
    /// ��������� �� ���������� �������. 0, ���� ��� ������ ������� � ������
    ListElement *prev;

    T* value;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
class List
{
public:
    List();
    /// ��������� ����� ������� � ����� ������
    void Append(ListElement<T> *element);
    /// ������� ������� �� ������
    void Remove(ListElement<T> *element);
    /// ���������� true, ���� ������ �������� element
    bool Member(ListElement<T> *element);
    /// ���������� ���������
    ListElement<T> *First();

private:
    /// ���������� ��������� �� ��������� ������� ������
    ListElement<T> *Last();
    /// ��������� �� ������ ������� ������. 0, ���� ������ ����
    ListElement<T> *head;
};
