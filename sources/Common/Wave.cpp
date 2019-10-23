#include "CommonTypes.h"
#include "defines.h"
#include "Wave.h"


pString Form::Name() const
{
    static const pString names[Form::Count] =
    {
        "�����",
        "����+",
        "����-",
        "�����������",
        "������",
        "�������",
        "�����",
        "������������"
    };

    return names[value];
}


