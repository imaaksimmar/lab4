#include <gtest/gtest.h>
#include "lazy_sequence.hpp"
#include "exceptions.hpp"

static int doubleIt(int x) { return x*2; }
static bool isEven(int x) { return x%2==0; }
static int sumReducer(int acc, int x) { return acc+x; }


TEST(LazySequenceFunctions, Map) {
    int items[] = {1,2,3,4,5};
    LazySequence<int> seq(items, 5);

    LazySequence<int>* res = seq.Map<int>(doubleIt, 5);
    EXPECT_EQ(res->Get(0), 2)
        << "input: LazySequence{1,2,3,4,5}.Map(x*2, 5) -> Get(0)"
        << "\nexpected: 2";
    EXPECT_EQ(res->Get(4), 10)
        << "input: LazySequence{1,2,3,4,5}.Map(x*2, 5) -> Get(4)"
        << "\nexpected: 10";
    delete res;
}

TEST(LazySequenceFunctions, MapOriginalUnchanged) {
    int items[] = {1,2,3};
    LazySequence<int> seq(items, 3);

    LazySequence<int>* res = seq.Map<int>(doubleIt, 3);
    EXPECT_EQ(seq.Get(0), 1)
        << "input: LazySequence{1,2,3}.Map(x*2, 3) -> seq.Get(0)"
        << "\nexpected: 1 ";
    delete res;
}


TEST(LazySequenceFunctions, Where) {
    int items[] = {1,2,3,4,5,6};
    LazySequence<int> seq(items, 6);

    LazySequence<int>* res = seq.Where(isEven, 6);
    EXPECT_EQ(res->GetLength().value, 3u)
        << "input: LazySequence{1,2,3,4,5,6}.Where(чётные, 6) -> GetLength()"
        << "\nexpected: 3";
    EXPECT_EQ(res->Get(0), 2)
        << "input: Where(чётные) -> Get(0)"
        << "\nexpected: 2";
    EXPECT_EQ(res->Get(2), 6)
        << "input: Where(чётные) -> Get(2)"
        << "\nexpected: 6";
    delete res;
}

TEST(LazySequenceFunctions, WhereOriginalUnchanged) {
    int items[] = {1,2,3};
    LazySequence<int> seq(items, 3);

    LazySequence<int>* res = seq.Where(isEven, 3);
    EXPECT_EQ(seq.GetLength().value, 3u)
        << "input: LazySequence{1,2,3}.Where(чётные, 3) -> seq.GetLength()"
        << "\nexpected: 3 (Where не изменяет оригинал)";
    delete res;
}

TEST(LazySequenceFunctions, Reduce) {
    int items[] = {1,2,3,4,5};
    LazySequence<int> seq(items, 5);

    int sum = seq.Reduce<int>(sumReducer, 0, 5);
    EXPECT_EQ(sum, 15)
        << "input: LazySequence{1,2,3,4,5}.Reduce(a+b, 0, 5)"
        << "\nexpected: 15";
}

