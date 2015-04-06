#ifndef CEXCEPTION_H
#define CEXCEPTION_H
#include <iostream>

class CException
{
public:
    virtual void PrintError() const = 0;
};

class CExOutOfRange : public virtual CException
{
public:
    virtual void PrintError() const;
};

class CExAlienKey : public virtual CException
{
public:
    virtual void PrintError() const;
};

class CExKeyRepetition : public virtual CException
{
public:
    virtual void PrintError() const;
};

class CExInitError : public virtual CException
{
public:
    virtual void PrintError() const;
};

class CExBigTest : public virtual CException
{
public:
    virtual void PrintError() const;
};

class CExPossibleKeyError : public virtual CException
{
public:
    virtual void PrintError() const;
};

class CExInsertError : public virtual CException
{
public:
    virtual void PrintError() const;
};

class CExHasError : public virtual CException
{
public:
    virtual void PrintError() const;
};

class CExEraseError : public virtual CException
{
public:
    virtual void PrintError() const;
};
#endif // CEXCEPTION_H
