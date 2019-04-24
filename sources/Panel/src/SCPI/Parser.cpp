#include "defines.h"
#include "SCPI/Parser.h"
#include "SCPI/SCPI.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
List<String> SCPI::Parser::words;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SCPI::Parser::Parse()
{
    ClearList();

    if (Buffer::GetByte(0) == '*')
    {
        ParseStar();
    }
    else if (Buffer::GetByte(0) == ':')
    {
        ParseColon();
    }
    else
    {
        // ����� ������
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SCPI::Parser::ClearList()
{
    words.Clear();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String *SCPI::Parser::GetWord()
{
    return words.First()->Get();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SCPI::Parser::RemoveWord(String *string)
{
    words.Remove(string);
    delete string;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SCPI::Parser::ParseStar()
{
    for (uint i = 0; i < Buffer::Size(); i++)
    {
        if (Buffer::GetByte(i) == 0x0d)
        {
            String *string = CreateString(0, i - 1);
            words.Append(string);
            Buffer::MarkFirstSymbolAsBad();
            Buffer::RemoveBadSymbols();
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SCPI::Parser::ParseColon()
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String *SCPI::Parser::CreateString(uint first, uint last)
{
    String *result = new String(last - first + 1);

    int count = 0;

    for (uint i = first; i <= last; i++)
    {
        (*result)[count++] = (char)Buffer::GetByte(i);
    }

    (*result)[count] = '\0';

    return result;
}
