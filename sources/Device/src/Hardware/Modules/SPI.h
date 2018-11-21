#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SPI1_
{
    friend class CPU;
private:
    static void Init();

public:
    static bool Receive(void *buffer, uint size, uint timeout = 200);

    static bool Transmit(void *buffer, uint size, uint timeout = 200);

    static bool Transmit(uint value, uint timeout);
};
