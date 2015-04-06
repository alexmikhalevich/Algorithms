#ifndef CEXCEPTION_H
#define CEXCEPTION_H

#include <iostream>

class CException
{
public:
    virtual void PrintException() const = 0;
};

class CExInvalidKey : public virtual CException
{
public:
    virtual void PrintException() const;
};

class CExNoExtraction : public virtual CException
{
public:
    virtual void PrintException() const;
};

class CExEmptyHeap : public virtual CException
{
public:
    virtual void PrintException() const;
};

#endif // CEXCEPTION_H
