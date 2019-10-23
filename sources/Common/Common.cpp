#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4514)   // unreferenced inline function has been removed
#endif

#include "Common.h"
#include <cmath>

#ifdef WIN32
#pragma warning(pop)
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ParamValue::ParamValue(float v)
{
    FromFloat(v);
}


void ParamValue::FromFloat(float v)
{
    int sign = (v < 0.0F) ? -1 : 1;

    value = static_cast<uint64>(std::fabsf(v) * 1.E9F);

    if (sign < 0)
    {
        SetSign(-1);
    }
}


float ParamValue::ToFloat() const
{
    return Abs() / 1E9F * static_cast<float>(Sign());
}


int ParamValue::Sign() const
{
    //    fedcba9876543210
    return (value & 0x8000000000000000U) ? -1 : 1;
}


uint64 ParamValue::Abs() const
{               //    fedcba9876543210
    return (value & 0x7fffffffffffffff);
}


void ParamValue::Divide(uint div)
{
    value /= div;
}


void ParamValue::Multiplie(uint mul)
{
    value *= mul;
}


void ParamValue::SetSign(int sign)
{
    if (sign > 0)
    {
        value &= 0x8000000000000000U;
    }
    else
    {
        //         fedcba9876543210
        value |= 0x8000000000000000U;           // ������������� ������� �������������� �����
    }
}


void ParamValue::Add(float v)
{
    FromFloat(ToFloat() + v);
}
