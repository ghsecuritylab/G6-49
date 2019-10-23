#include "defines.h"
#include "SCPI/Errors.h"
#include "SCPI/VCP.h"



void SCPI::Error::Syntax::Invalid()
{
    VCP::Send("Invalid syntax");
}


void SCPI::Error::Syntax::MissingColon()
{
    VCP::Send("Error in command : missing colon");
}


void SCPI::Error::UnknownCommand()
{
    VCP::Send("Unknown command");
}

