#pragma once
#include "Utils/String.h"
#include <ff.h>



class FatFS
{
public:
    static String ErrorString(FRESULT result);
};

