#include "defines.h"
#include "Buffer.h"
#include <cstdlib>



Buffer::Buffer(uint _size) : data(nullptr), size(0)
{
    data = (uint8 *)std::malloc(_size);
    size = _size;
}


Buffer::~Buffer()
{
    std::free(data);
}

