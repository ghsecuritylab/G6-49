#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "structs.h"
#include "Message.h"
#include "Hardware/CPU.h"
#include <cstring>
#include <cstdlib>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Message::Message() : allocated(0), buffer(0), used(0), taken(0)
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Message::~Message()
{
    FreeMemory();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Message::Message(uint size, uint8 value0, uint8 value1) : allocated(0), buffer(0), used(0), taken(0)
{   
    AllocateMemory(size);
    PutByte(value0);
    PutByte(value1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Message::Message(uint size, uint8 value0, uint8 value1, uint8 value2) : allocated(0), buffer(0), used(0), taken(0)
{
    AllocateMemory(size);
    PutByte(value0);
    PutByte(value1);
    PutByte(value2);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Message::CreateAllocate(uint8 *_buffer, uint _size)
{
    if (AllocateMemory(_size))
    {
        std::memcpy(buffer, _buffer, _size);
        used = _size;
    }

    return buffer != 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Message::CreateFromMessage(Message *message)
{
    if (AllocateMemory(message->Size()))
    {
        std::memcpy(buffer, message->Data(), message->Size());
        used = message->Size();
    }

    return buffer != 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Message::PutByte(uint8 data)
{
     buffer[used] = data;
     used += sizeof(data);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Message::PutHalfWord(int16 data)
{
    BitSet16 bs((uint16)data);
    bs.WriteToBuffer(buffer + used);
    used += sizeof(data);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Message::PutWord(uint data)
{
     BitSet32 bs(data);
     bs.WriteToBuffer(buffer + used);
     used += sizeof(data);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Message::PutFloat(float data)
{
    BitSet32 bs(data);
    bs.WriteToBuffer(buffer + used);
    used += sizeof(data);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 Message::TakeByte()
{
    uint8 result = buffer[taken];
    taken += sizeof(result);

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint16 Message::TakeHalfWord()
{
    BitSet16 bs(buffer + taken);
    uint16 result = bs.halfWord;
    taken += sizeof(result);

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint Message::TakeWord()
{
    BitSet32 bs(buffer + taken);
    uint result = bs.word;
    taken += sizeof(result);

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint64 Message::TakeDoubleWord()
{
    BitSet64 bs(buffer + taken);
    uint64 result = bs.dword;
    taken += sizeof(result);

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float Message::TakeFloat()
{
    BitSet32 bs(buffer + taken);
    float result = bs.floatValue;
    taken += sizeof(result);

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Message::AllocateMemory(uint size)
{
    FreeMemory();

    if (size == 0)
    {
        return false;
    }

    buffer = (uint8 *)std::malloc(size);
    if (buffer)
    {
        allocated = size;
    }
    else
    {
        LOG_WRITE("�� ���� �������� %d ����", size);
    }

    return buffer != 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Message::FreeMemory()
{
    allocated = 0;
    used = 0;
    std::free(buffer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 *Message::Data()
{
    return buffer;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint Message::Size() const
{
    return allocated;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Message::IsEmpty() const
{
    return buffer != 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Message::IsEquals(const Message *message) const
{
    if (Size() != message->Size())
    {
        return false;
    }

    return std::memcmp(((Message *)message)->Data(), ((Message *)this)->Data(), Size()) == 0;
}
