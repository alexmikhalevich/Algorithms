#include "cexception.h"

void CExKeyRepetition::PrintError() const
{
    std::cerr << "Key repetition in the begin array found!" << std::endl;
}

void CExOutOfRange::PrintError() const
{
    std::cerr << "Index is out of range!" << std::endl;
}

void CExAlienKey::PrintError() const
{
    std::cerr << "Alien key found!" << std::endl;
}

void CExInitError::PrintError() const
{
    std::cerr << "Key from the initial array was not found in the table!" << std::endl;
}

void CExBigTest::PrintError() const
{
    std::cerr << "Too big test!" << std::endl;
}

void CExPossibleKeyError::PrintError() const
{
    std::cerr << "IsPossibleKey() returns true on alien key!" << std::endl;
}

void CExInsertError::PrintError() const
{
    std::cerr << "Insert error!" << std::endl;
}

void CExEraseError::PrintError() const
{
    std::cerr << "Erase error!" << std::endl;
}

void CExHasError::PrintError() const
{
    std::cerr << "Has error!" << std::endl;
}
