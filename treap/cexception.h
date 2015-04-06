#ifndef CEXCEPTION_H
#define CEXCEPTION_H

class CException
{
public:
    virtual void PrintError() const = 0;
};

class CExUnknownCommand : public virtual CException
{
public:
    virtual void PrintError() const;
};

#endif // CEXCEPTION_H
