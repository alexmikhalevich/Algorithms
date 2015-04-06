#include <iostream>
#include "cexception.h"

void CExUnknownCommand::PrintError() const
{
    std::cerr << "EE: Unknown command" << std::endl;
}
