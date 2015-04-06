#ifndef CCOMPARE_HPP
#define CCOMPARE_HPP

template<typename Value, typename CompareFunction>
class CCompare
{
public:
    CCompare(CompareFunction l) : lessFunction(l) {}
    CompareFunction& GetLessFunction()
    {
        return lessFunction;
    }

    bool Greater(Value x, Value y)
    {
        return lessFunction(y, x);
    }

    bool GreaterEqual(Value x, Value y)
    {
        return !lessFunction(x, y);
    }

    bool Less(Value x, Value y)
    {
        return lessFunction(x, y);
    }

    bool LessEqual(Value x, Value y)
    {
        return !lessFunction(y, x);
    }

private:
    CompareFunction lessFunction;
};

#endif // CCOMPARE_HPP
