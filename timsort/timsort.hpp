#ifndef TIMSORT_HPP
#define TIMSORT_HPP

#include <vector>
#include <iterator>
#include <algorithm>
#include <stack>

/*
 *interface for testing Timsort performance with different parameters
 */
class ITimsortParams
{
public:
    static const unsigned int MIN_RUN_SIZE_32 = 32;
    static const unsigned int MIN_RUN_SIZE_64 = 64;

    enum EWhatMerge
    {
        WM_NO_MERGE,
        WM_MERGE_XY,
        WM_MERGE_YZ
    };

    /*
     *returns the minimum acceptable length of the run for the array of the specified length
     *length - the length of the input array
     */
    virtual unsigned int MinRun(unsigned int length) const = 0;

    /*
     *if it is necessary to merge two last runs of lengths lengthX and lengthY
     */
    virtual bool NeedMerge(unsigned int lengthX, unsigned int lengthY) const = 0;

    /*
     *how to merge if there are three last runs of lengths lengthX, lengthY and lengthZ in the stack
     */
    virtual EWhatMerge WhatMerge(int const lengthX, int const lengthY, int const lengthZ) const = 0;

    /*
     *returns the minimum possible run size
     */
    virtual unsigned int MinRunSize() const = 0;
};

/*
 *default Timsort parameters
 */
class CTimsortParamsDefault : public ITimsortParams
{
    unsigned int MinRun(unsigned int length) const
    {
        unsigned int r = 0;
        while(length >= MIN_RUN_SIZE_32)
        {
            r |= (length & 1);
            length >>= 1;
        }
        return length + r;
    }

    unsigned int MinRunSize() const
    {
        return MIN_RUN_SIZE_32;
    }

    bool NeedMerge(unsigned int lengthX, unsigned int lengthY) const
    {
        if(lengthX < lengthY)
            return true;
        else
            return false;
    }

    EWhatMerge WhatMerge(const int lengthX, const int lengthY, const int lengthZ) const
    {
        if((lengthX != -1) && (lengthX <= lengthY + lengthZ))
            return (lengthX < lengthZ) ?  WM_MERGE_XY : WM_MERGE_YZ;
        else if(lengthY <= lengthZ)
            return WM_MERGE_YZ;
        else
            return WM_NO_MERGE;
    }

public:
    CTimsortParamsDefault() {}
};

template <typename RandomAccessIterator> struct Run
{
    RandomAccessIterator runBegin;
    typename std::iterator_traits<RandomAccessIterator>::difference_type runLength;
    Run();
    Run(RandomAccessIterator const b,
        typename std::iterator_traits<RandomAccessIterator>::difference_type const l) : runBegin(b), runLength(l) {}
};

/*
 *the compare function class
 *assuming lessFunction(x, y) returns true if x < y
 */
template<typename Value, typename CompareFunction> class CCompare
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

template<typename RandomAccessIterator, typename CompareFunction> class CTimsort
{
    static const int MIN_GALLOP = 7;
    typename std::iterator_traits<RandomAccessIterator>::difference_type min_Gallop;

    //a stack for runs to be merged
    std::vector<Run<RandomAccessIterator> > runs;

    //a temporary stack for runs
    std::vector<typename std::iterator_traits<RandomAccessIterator>::value_type> tempVector;

    CCompare<const typename std::iterator_traits<RandomAccessIterator>::value_type&, CompareFunction> compare;
    const ITimsortParams * const timsortParameters;

    /*
     *firstElement - the first element in the range to be sorted
     *lastElement - the last element in the range to be sorted
     *start - the first element in the range that is not already known to be sorted
     *comp - comparator
     */
    static void InsertionSort(RandomAccessIterator const firstElement, RandomAccessIterator const lastElement, RandomAccessIterator start,
                              CCompare<const typename std::iterator_traits<RandomAccessIterator>::value_type&, CompareFunction> comp);

    /*
     *returns the length of the specified range and reverses the run if it is descending
     *firstElement - first element in the specified range
     *lastElement - last element in the specified range
     *comp - comparator
     */
    static typename std::iterator_traits<RandomAccessIterator>::difference_type CountRunAndMakeAscending(RandomAccessIterator const firstElement,
                     RandomAccessIterator const lastElement,
                     CCompare<const typename std::iterator_traits<RandomAccessIterator>::value_type&, CompareFunction> comp);

    /*
     *merges adjacent runs
     */
    void MergeCollapse();

    /*
     *merges all runs onto the stack until only one remains
     */
    void MergeForceCollapse();

    /*
     *merges the two runs at stack (run and run+1)
     *run = runs.size() - 2 || runs.size() - 3
     */
    void MergeAt(typename std::iterator_traits<RandomAccessIterator>::difference_type const run);

    /*
     *locates the position at which to insert the specified key at specified sorted range
     *key - the key whose insertion point to search for
     *firstRunElement - the position of the first element in run
     *runLength - the length of the run
     *hint - the index at which to begin the search
     */
    typename std::iterator_traits<RandomAccessIterator>::difference_type GallopLeft(typename std::iterator_traits<RandomAccessIterator>::reference const key,
                                     RandomAccessIterator const firstRunElement,
                                     typename std::iterator_traits<RandomAccessIterator>::difference_type const runLength,
                                     typename std::iterator_traits<RandomAccessIterator>::difference_type const hint);

    /*
     *locates the position at which to insert the specified key at specified sorted range
     *key - the key whose insertion point to search for
     *firstRunElement - the position of the first element in run
     *runLength - the length of the run
     *hint - the index at which to begin the search
     */
    typename std::iterator_traits<RandomAccessIterator>::difference_type GallopRight(typename std::iterator_traits<RandomAccessIterator>::reference const key,
                                     RandomAccessIterator const firstRunElement,
                                     typename std::iterator_traits<RandomAccessIterator>::difference_type const runLength,
                                     typename std::iterator_traits<RandomAccessIterator>::difference_type const hint);

    /*
     *merges two adjacent runs, should be called when length1 <= length2
     *beginFirstRun - the position of the first element of the first run
     *endFirstRun - the position of the last element of the first run
     *beginSecondRun - the position of the first element of the second run
     *endSecondRun - the position of the last element of the second run
     */
    void MergeLo(RandomAccessIterator const beginFirstRun, typename std::iterator_traits<RandomAccessIterator>::difference_type lengthFirstRun,
                 RandomAccessIterator const beginSecondRun, typename std::iterator_traits<RandomAccessIterator>::difference_type lengthSecondRun);

    /*
     *merges two adjacent runs, should be called when length1 >= length2
     *beginFirstRun - the position of the first element of the first run
     *endFirstRun - the position of the last element of the first run
     *beginSecondRun - the position of the first element of the second run
     *endSecondRun - the position of the last element of the second run
     */
    void MergeHi(RandomAccessIterator const beginFirstRun, typename std::iterator_traits<RandomAccessIterator>::difference_type lengthFirstRun,
                 RandomAccessIterator const beginSecondRun, typename std::iterator_traits<RandomAccessIterator>::difference_type lengthSecondRun);

    /*
     *copies specified run into the temp array
     *beginRun - the beginning of the run
     *runLength - the length of the run
     */
    void CopyToTempArray(RandomAccessIterator const beginRun, typename std::iterator_traits<RandomAccessIterator>::difference_type const runLength);

public:
    CTimsort(CCompare<const typename std::iterator_traits<RandomAccessIterator>::value_type&,
             CompareFunction> comp, const ITimsortParams * const params) : compare(comp),
        min_Gallop(MIN_GALLOP), timsortParameters(params) {}
    ~CTimsort() {}

    /*
     *API for the class
     *firstElement - the position of the first element in sorted range
     *lastElement - the position of the last element in sorted range
     *comp - comparator
     *params - the realisation of ITimsortParams interface, sorting parameters
     */
    static void Sort(RandomAccessIterator const firstElement,
                     RandomAccessIterator const lastElement,
                     CCompare<const typename std::iterator_traits<RandomAccessIterator>::value_type&, CompareFunction> const comp,
                     const ITimsortParams * const params);
};

template<typename RandomAccessIterator, typename CompareFunction>
void CTimsort<RandomAccessIterator, CompareFunction>::InsertionSort(RandomAccessIterator const firstElement,
                          RandomAccessIterator const lastElement, RandomAccessIterator start,
                          CCompare<const typename std::iterator_traits<RandomAccessIterator>::value_type&, CompareFunction> comp)
{
    if(lastElement - firstElement < 2)
        return;
    if(start == firstElement)
        ++start;

    for(; start != lastElement; ++start)
        for(RandomAccessIterator iter = start; iter != firstElement && comp.Less(*iter, *(iter - 1)); --iter)
            std::iter_swap(iter, iter - 1);
}

template<typename RandomAccessIterator, typename CompareFunction>
typename std::iterator_traits<RandomAccessIterator>::difference_type
CTimsort<RandomAccessIterator, CompareFunction>::CountRunAndMakeAscending(RandomAccessIterator const firstElement,
                 RandomAccessIterator const lastElement,
                 CCompare<const typename std::iterator_traits<RandomAccessIterator>::value_type&, CompareFunction> comp)
{
    RandomAccessIterator runHi = firstElement + 1;
    if(runHi == lastElement) return 1;

    if(comp.Less(*(runHi++), *firstElement)) //descending
    {
        while(runHi < lastElement && comp.Less(*runHi, *(runHi - 1)))
            ++runHi;
        std::reverse(firstElement, runHi);
    }
    else
        while(runHi < lastElement && comp.GreaterEqual(*runHi, *(runHi - 1)))
            ++runHi; //ascending

    return (runHi - firstElement);
}

template<typename RandomAccessIterator, typename CompareFunction>
void CTimsort<RandomAccessIterator, CompareFunction>::MergeCollapse()
{
    while(runs.size() > 1)
    {
        typename std::iterator_traits<RandomAccessIterator>::difference_type mergeRun = runs.size() - 2;

        CTimsortParamsDefault::EWhatMerge merge;
        if(mergeRun > 0)
            merge = timsortParameters->WhatMerge(runs.at(mergeRun - 1).runLength,
                                                              runs.at(mergeRun).runLength, runs.at(mergeRun + 1).runLength);
        else
            merge = timsortParameters->WhatMerge(-1, runs.at(mergeRun).runLength, runs.at(mergeRun + 1).runLength);

        if(merge == CTimsortParamsDefault::WM_MERGE_XY)
        {
            --mergeRun;
            MergeAt(mergeRun);
        }
        else if(merge == CTimsortParamsDefault::WM_MERGE_YZ)
            MergeAt(mergeRun);
        else if(merge == CTimsortParamsDefault::WM_NO_MERGE)
            break;
    }
}

template<typename RandomAccessIterator, typename CompareFunction>
void CTimsort<RandomAccessIterator, CompareFunction>::MergeForceCollapse()
{
    while(runs.size() > 1)
    {
        typename std::iterator_traits<RandomAccessIterator>::difference_type subtrahend = runs.size() - 2;
        if(subtrahend > 0 && !timsortParameters->NeedMerge(runs.at(subtrahend - 1).runLength,
                                                           runs.at(subtrahend + 1).runLength))
            --subtrahend;
        MergeAt(subtrahend);
    }
}

template<typename RandomAccessIterator, typename CompareFunction>
void CTimsort<RandomAccessIterator, CompareFunction>::MergeAt(typename std::iterator_traits<RandomAccessIterator>::difference_type const run)
{
    RandomAccessIterator begin1 = runs.at(run).runBegin;
    typename std::iterator_traits<RandomAccessIterator>::difference_type length1 = runs.at(run).runLength;
    RandomAccessIterator begin2 = runs.at(run + 1).runBegin;
    typename std::iterator_traits<RandomAccessIterator>::difference_type length2 = runs.at(run + 1).runLength;

    runs.at(run).runLength = length1 + length2;

    if(run == runs.size() - 3)
        runs[run + 1] = runs[run + 2];

    runs.pop_back();

    typename std::iterator_traits<RandomAccessIterator>::difference_type const k = GallopRight(*begin2, begin1, length1, 0);
    begin1 += k;
    length1 -= k;

    if(length1 == 0)
        return;

    length2 = GallopLeft(*(begin1 + (length1 - 1)), begin2, length2, length2 - 1);

    if(length2 == 0)
        return;

    if(length1 <= length2)
        MergeLo(begin1, length1, begin2, length2);
    else
        MergeHi(begin1, length1, begin2, length2);
}

template<typename RandomAccessIterator, typename CompareFunction>
typename std::iterator_traits<RandomAccessIterator>::difference_type
CTimsort<RandomAccessIterator, CompareFunction>::GallopLeft(typename std::iterator_traits<RandomAccessIterator>::reference const key,
                                 RandomAccessIterator const firstRunElement,
                                 typename std::iterator_traits<RandomAccessIterator>::difference_type const runLength,
                                 typename std::iterator_traits<RandomAccessIterator>::difference_type const hint)
{
    typename std::iterator_traits<RandomAccessIterator>::difference_type lastOffset = 0;
    typename std::iterator_traits<RandomAccessIterator>::difference_type offset = 1;

    if(compare.GreaterEqual(key, *(firstRunElement + hint)))
    {
        typename std::iterator_traits<RandomAccessIterator>::difference_type const maxOffset = runLength - hint;
        while(offset < maxOffset && compare.LessEqual(*(firstRunElement + (hint + offset)), key))
        {
            lastOffset = offset;
            offset = (offset << 1) + 1;
            if(offset <= 0)
                offset = maxOffset;
        }

        if(offset > maxOffset)
            offset = maxOffset;

        //make offsets relative to firstRunElement
        lastOffset += hint;
        offset += hint;
    }
    else
    {
        typename std::iterator_traits<RandomAccessIterator>::difference_type const maxOffset = hint + 1;
        while(offset < maxOffset && compare.GreaterEqual(*(firstRunElement + (hint - offset)), key))
        {
            lastOffset = offset;
            offset = (offset << 1) + 1;
            if(offset <= 0)
                offset = maxOffset;
        }

        if(offset > maxOffset)
            offset = maxOffset;

        //make offsets relative to firstRunElement
        typename std::iterator_traits<RandomAccessIterator>::difference_type const temp = lastOffset;
        lastOffset = hint - offset;
        offset = hint - temp;
    }

    return std::lower_bound(firstRunElement + (lastOffset + 1), firstRunElement + offset, key, compare.GetLessFunction()) - firstRunElement;
}

template<typename RandomAccessIterator, typename CompareFunction>
typename std::iterator_traits<RandomAccessIterator>::difference_type
CTimsort<RandomAccessIterator, CompareFunction>::GallopRight(typename std::iterator_traits<RandomAccessIterator>::reference const key,
                                 RandomAccessIterator const firstRunElement,
                                 typename std::iterator_traits<RandomAccessIterator>::difference_type const runLength,
                                 typename std::iterator_traits<RandomAccessIterator>::difference_type const hint)
{
    typename std::iterator_traits<RandomAccessIterator>::difference_type lastOffset = 0;
    typename std::iterator_traits<RandomAccessIterator>::difference_type offset = 1;

    if(compare.Less(key, *(firstRunElement + hint)))
    {
        typename std::iterator_traits<RandomAccessIterator>::difference_type const maxOffset = hint + 1;
        while(offset < maxOffset && compare.Less(key, *(firstRunElement + (hint - offset))))
        {
            lastOffset = offset;
            offset = (offset << 1) + 1;
            if(offset <= 0)
                offset = maxOffset; //overflow
        }

        if(offset > maxOffset)
            offset = maxOffset;

        //make offsets relative to firstRunElement
        typename std::iterator_traits<RandomAccessIterator>::difference_type const temp = lastOffset;
        lastOffset = hint - offset;
        offset = hint - temp;
    }
    else
    {
        typename std::iterator_traits<RandomAccessIterator>::difference_type const maxOffset = runLength - hint;
        while(offset < maxOffset && compare.GreaterEqual(key, *(firstRunElement + (hint + offset))))
        {
            lastOffset = offset;
            offset = (offset << 1) + 1;
            if(offset <= 0)
                offset = maxOffset; //overflow
        }

        if(offset > maxOffset)
            offset = maxOffset;

        //make offsets relative to firstRunElement
        lastOffset += hint;
        offset += hint;
    }

    return std::upper_bound(firstRunElement + (lastOffset + 1), firstRunElement + offset, key, compare.GetLessFunction()) - firstRunElement;
}

template<typename RandomAccessIterator, typename CompareFunction>
void CTimsort<RandomAccessIterator, CompareFunction>::MergeLo(RandomAccessIterator const beginFirstRun,
             typename std::iterator_traits<RandomAccessIterator>::difference_type lengthFirstRun,
             RandomAccessIterator const beginSecondRun, typename std::iterator_traits<RandomAccessIterator>::difference_type lengthSecondRun)
{
    CopyToTempArray(beginFirstRun, lengthFirstRun);

    typename std::vector<typename std::iterator_traits<RandomAccessIterator>::value_type>::iterator cursor1 = tempVector.begin(); //indexes into temp array
    RandomAccessIterator cursor2 = beginSecondRun;
    RandomAccessIterator destination = beginFirstRun;

    *(destination++) = *(cursor2++);
    if(--lengthSecondRun == 0)
    {
        std::copy(cursor1, cursor1 + lengthFirstRun, destination);
        return;
    }

    if(lengthFirstRun == 1)
    {
        std::copy(cursor2, cursor2 + lengthSecondRun, destination);
        *(destination + lengthSecondRun) = *cursor1;
        return;
    }

    int minGallop(min_Gallop);

    bool breakCycle = false;
    while(!breakCycle)
    {
        typename std::iterator_traits<RandomAccessIterator>::difference_type count1 = 0; //number of times in a row that first run won
        typename std::iterator_traits<RandomAccessIterator>::difference_type count2 = 0; //number of times in a row that second run won

        do
        {
            if(compare.Less(*cursor2, *cursor1))
            {
                *(destination++) = *(cursor2++);
                ++count2;
                count1 = 0;
                if(--lengthSecondRun == 0)
                {
                    breakCycle = true;
                    break;
                }
            }
            else
            {
                *(destination++) = *(cursor1++);
                ++count1;
                count2 = 0;
                if(--lengthFirstRun == 1)
                {
                    breakCycle = true;
                    break;
                }
            }
        } while((count1 | count2) < minGallop);

        if(breakCycle) break;

        do
        {
            count1 = GallopRight(*cursor2, cursor1, lengthFirstRun, 0);
            if(count1 != 0)
            {
                std::copy_backward(cursor1, cursor1 + count1, destination + count1);
                destination += count1;
                cursor1 += count1;
                lengthFirstRun -= count1;
                if(lengthFirstRun <= 1)
                {
                    breakCycle = true;
                    break;
                }
            }

            *(destination++) = *(cursor2++);
            if(--lengthSecondRun == 0)
            {
                breakCycle = true;
                break;
            }

            count2 = GallopLeft(*cursor1, cursor2, lengthSecondRun, 0);

            if(count2 != 0)
            {
                std::copy(cursor2, cursor2 + count2, destination);
                destination += count2;
                cursor2 += count2;
                lengthSecondRun -= count2;
                if(lengthSecondRun == 0)
                {
                    breakCycle = true;
                    break;
                }
            }
            *(destination++) = *(cursor1++);
            if(--lengthFirstRun == 1)
            {
                breakCycle = true;
                break;
            }
            --minGallop;
        } while((count1 >= MIN_GALLOP) | (count2 >= MIN_GALLOP));

        if(breakCycle)
            break;

        if(minGallop < 0)
            minGallop = 0;
        minGallop += 2;
    }

    min_Gallop = std::min(minGallop, 1);

    if(lengthFirstRun == 1)
    {
        std::copy(cursor2, cursor2 + lengthSecondRun, destination);
        *(destination + lengthSecondRun) = *cursor1;
    }

    else std::copy(cursor1, cursor1 + lengthFirstRun, destination);
}

template<typename RandomAccessIterator, typename CompareFunction>
void CTimsort<RandomAccessIterator, CompareFunction>::MergeHi(RandomAccessIterator const beginFirstRun,
             typename std::iterator_traits<RandomAccessIterator>::difference_type lengthFirstRun,
             RandomAccessIterator const beginSecondRun, typename std::iterator_traits<RandomAccessIterator>::difference_type lengthSecondRun)
{
    CopyToTempArray(beginSecondRun, lengthSecondRun);

    RandomAccessIterator cursor1 = beginFirstRun + (lengthFirstRun - 1);
    typename std::vector<typename std::iterator_traits<RandomAccessIterator>::value_type>::iterator cursor2 = tempVector.begin()
            + (lengthSecondRun - 1); //index into temp array
    RandomAccessIterator destination = beginSecondRun + (lengthSecondRun - 1);

    *(destination--) = *(cursor1--);
    if(--lengthFirstRun == 0)
    {
        std::copy(tempVector.begin(), tempVector.begin() + lengthSecondRun, destination - (lengthSecondRun - 1));
        return;
    }

    if(lengthSecondRun == 1)
    {
        destination -= lengthFirstRun;
        cursor1 -= lengthFirstRun;
        std::copy_backward(cursor1 + 1, cursor1 + (1 + lengthFirstRun), destination + (1 + lengthFirstRun));
        *destination = *cursor2;
        return;
    }

    int minGallop(min_Gallop);

    bool breakCycle = false;
    while(!breakCycle)
    {
        typename std::iterator_traits<RandomAccessIterator>::difference_type count1 = 0; //number of times in a row that first run won
        typename std::iterator_traits<RandomAccessIterator>::difference_type count2 = 0; //number of times in a row that second run won

        do
        {
            if(compare.Less(*cursor2, *cursor1))
            {
                *(destination--) = *(cursor1--);
                ++count1;
                count2 = 0;
                if(--lengthFirstRun == 0)
                {
                    breakCycle = true;
                    break;
                }
            }
            else
            {
                *(destination--) = *(cursor2--);
                ++count2;
                count1 = 0;
                if(--lengthSecondRun == 1)
                {
                    breakCycle = true;
                    break;
                }
            }
        } while((count1 | count2) < minGallop);

        if(breakCycle) break;

        do
        {
            count1 = lengthFirstRun - GallopRight(*cursor2, beginFirstRun, lengthFirstRun, lengthFirstRun - 1);
            if(count1 != 0)
            {
                destination -= count1;
                cursor1 -= count1;
                lengthFirstRun -= count1;
                std::copy_backward(cursor1 + 1, cursor1 + (1 + count1), destination + (1 + count1));
                if(lengthFirstRun == 0)
                {
                    breakCycle = true;
                    break;
                }
            }

            *(destination--) = *(cursor2--);
            if(--lengthSecondRun == 1)
            {
                breakCycle = true;
                break;
            }

            count2 = lengthSecondRun - GallopLeft(*cursor1, tempVector.begin(), lengthSecondRun, lengthSecondRun - 1);
            if(count2 != 0)
            {
                destination -= count2;
                cursor2 -= count2;
                lengthSecondRun -= count2;
                std::copy(cursor2 + 1, cursor2 + (1 + count2), destination + 1);
                if(lengthSecondRun <= 1)
                {
                    breakCycle = true;
                    break;
                }
            }
            *(destination--) = *(cursor1--);
            if(--lengthFirstRun == 0)
            {
                breakCycle = true;
                break;
            }
            minGallop--;
        } while((count1 >= MIN_GALLOP) | (count2 >= MIN_GALLOP));

        if(breakCycle)
            break;

        if(minGallop < 0)
            minGallop = 0;
        minGallop += 2;
    }

    min_Gallop = std::min(minGallop, 1);

    if(lengthSecondRun == 1)
    {
        destination -= lengthFirstRun;
        cursor1 -= lengthFirstRun;
        std::copy_backward(cursor1 + 1, cursor1 + (1 + lengthFirstRun), destination + (1 + lengthFirstRun));
        *destination = *cursor2;
    }
    else std::copy(tempVector.begin(), tempVector.begin() + lengthSecondRun, destination - (lengthSecondRun - 1));
}

template<typename RandomAccessIterator, typename CompareFunction>
void CTimsort<RandomAccessIterator, CompareFunction>::CopyToTempArray(RandomAccessIterator const beginRun,
          typename std::iterator_traits<RandomAccessIterator>::difference_type const runLength)
{
    tempVector.clear();
    tempVector.reserve(runLength);
    std::copy(beginRun, beginRun + runLength, std::back_inserter(tempVector));
}

template<typename RandomAccessIterator, typename CompareFunction>
void CTimsort<RandomAccessIterator, CompareFunction>::Sort(RandomAccessIterator const firstElement,
                 RandomAccessIterator const lastElement,
                 CCompare<const typename std::iterator_traits<RandomAccessIterator>::value_type&, CompareFunction> const comp,
                 const ITimsortParams * const params)
{
    unsigned int nRemaining = (lastElement - firstElement);
    if(nRemaining < 2)
        return;

    if(nRemaining < params->MinRunSize())
    {
        typename std::iterator_traits<RandomAccessIterator>::difference_type const initRunlength = CountRunAndMakeAscending(firstElement, lastElement, comp);
        InsertionSort(firstElement, lastElement, firstElement + initRunlength, comp);
        return;
    }

    CTimsort ts(comp, params);

    unsigned int const minRun = params->MinRun(nRemaining);
    RandomAccessIterator currentPosition = firstElement;
    do
    {
        unsigned int runLength = CountRunAndMakeAscending(currentPosition, lastElement, comp);
        if(runLength < minRun)
        {
            typename std::iterator_traits<RandomAccessIterator>::difference_type force = std::min(nRemaining, minRun);
            InsertionSort(currentPosition, currentPosition + force, currentPosition + runLength, comp);
            runLength = force;
        }

        ts.runs.push_back(Run<RandomAccessIterator>(currentPosition, runLength));
        ts.MergeCollapse();

        currentPosition += runLength;
        nRemaining -= runLength;
    } while(nRemaining != 0);

    ts.MergeForceCollapse();
}

template<typename RandomAccessIterator, typename CompareFunction>
void Timsort(RandomAccessIterator const firstElement, RandomAccessIterator const lastElement,
             CompareFunction const compareFunction = std::less<int>(),
             const ITimsortParams * const params = &CTimsortParamsDefault())
{
    CTimsort<RandomAccessIterator, CompareFunction>::Sort(firstElement, lastElement, compareFunction, params);
}

#endif // TIMSORT_H
