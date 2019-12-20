#include "defines.h"
#include "Message_p.h"


Message::LoadFormDDS::LoadFormDDS(uint8 ch, uint16 data[Generator::DDS_NUM_POINTS]) : SimpleMessage(2 + Generator::DDS_NUM_POINTS * 2, Command::LoadFormDDS)
{
    PutByte(ch);
    PutData(reinterpret_cast<uint8*>(data), sizeof(data[0]) * Generator::DDS_NUM_POINTS);
}
