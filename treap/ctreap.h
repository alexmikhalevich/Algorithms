#ifndef CTREAP_H
#define CTREAP_H

#include <vector>
#include <random>
#include <ctime>
#include "ipermutationstructure.h"

struct Vertice
{
    Vertice* vert_left_child;
    Vertice* vert_right_child;
    Vertice* vert_leftmost;
    Vertice* vert_rightmost;
    std::size_t vert_size;
    int vert_value;
    int vert_priority;
    int vert_maxrand;
    int vert_increasing_length;
    int vert_decreasing_length;
    long long vert_sum;
    bool vert_reversed;

    Vertice(const int value, const std::size_t size, const int maxrand = RAND_MAX) : vert_size(size), vert_value(value)
    {
        vert_maxrand = maxrand;
        vert_priority = rand() % maxrand;
        vert_left_child = NULL;
        vert_right_child = NULL;
        vert_leftmost = this;
        vert_rightmost = this;
        vert_decreasing_length = 1;
        vert_increasing_length = 1;
        vert_reversed = false;
        vert_sum = value;
    }

private:
    Vertice(const int maxrand = RAND_MAX);
    Vertice(const Vertice& v);
};

class CTreap : public virtual IPermutationStructure
{
private:
    Vertice* tr_root;
    std::size_t tr_elements;

    inline static std::size_t GetSize(Vertice* vert);
    inline static std::size_t GetDecreasingSuffixLength(Vertice* vert);
    inline static std::size_t GetIncreasingSuffixLength(Vertice* vert);
    inline static int GetValue(Vertice* vert);
    inline static long long GetSubsegmentSum(Vertice* vert);
    inline static Vertice* GetRightmostVert(Vertice* vert);
    inline static Vertice* GetLeftmostVert(Vertice* vert);
    inline static void Update(Vertice*& vert);
    inline static void Push(Vertice* vert);
    static void Merge(Vertice*& resultTreapRoot, Vertice* leftSubtreeRoot, Vertice* rightSubtreeRoot);
    static void Split(Vertice* rootTreapRoot, Vertice*& leftSubtreeRoot, Vertice*& rightSubtreeRoot, const std::size_t key);
    static void Set(Vertice*& rootVertice, const int value, const size_t index);
    static void BinarySearch(Vertice*& vert, Vertice* targetVert);
    void Print(Vertice* rootVertice) const;
public:
    CTreap();
    CTreap(Vertice* root, Vertice* leftChild, Vertice* rightChild);
    virtual ~CTreap();
    static CTreap* build_treap(std::vector<int>& inputData, const int maxrand = RAND_MAX);
    virtual void print() const;
    virtual void insert(const int value, const std::size_t index, const int maxrand = RAND_MAX);
    virtual void assign(const int value, const std::size_t index);
    virtual long long subsegment_sum(const std::size_t left, const std::size_t right);
    virtual void next_permutation(const std::size_t left, const std::size_t right);
    virtual std::size_t size() const;
};

#endif // CTREAP_H
