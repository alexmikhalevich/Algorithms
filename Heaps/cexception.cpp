#include "cexception.h"

void CExInvalidKey::PrintException() const
{
    std::cerr << "Invalid key!" << std::endl;
}

void CExEmptyHeap::PrintException() const
{
    std::cerr << "Access violation: empty heap!" << std::endl;
}

void CExNoExtraction::PrintException() const
{
    std::cerr << "Extraction from a null node asked!" << std::endl;
}
