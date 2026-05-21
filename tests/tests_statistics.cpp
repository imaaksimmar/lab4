#include <gtest/gtest.h>
#include "statistics.hpp"
#include "array_sequence.hpp"

TEST(Statistics, BasicStatistics) {
    int items[] = {1,2,3,4,5};
    LazySequence<int> seq(items,5);   
    Statistics<int> stats(seq);

    EXPECT_EQ(stats.GetCount(), 5u)
        << "input: Statistics(LazySeq{1,2,3,4,5}, 5) -> GetCount()"
        << "\nexpected: 5";
    EXPECT_EQ(stats.GetSum(), 15)
        << "input: Statistics(LazySeq{1,2,3,4,5}, 5) -> GetSum()"
        << "\nexpected: 15";
    EXPECT_EQ(stats.GetMin(), 1)
        << "input: Statistics(LazySeq{1,2,3,4,5}, 5) -> GetMin()"
        << "\nexpected: 1";
    EXPECT_EQ(stats.GetMax(), 5)
        << "input: Statistics(LazySeq{1,2,3,4,5}, 5) -> GetMax()"
        << "\nexpected: 5";
    EXPECT_DOUBLE_EQ(stats.GetAverage(), 3.0)
        << "input: Statistics(LazySeq{1,2,3,4,5}, 5) -> GetAverage()"
        << "\nexpected: 3.0";
}

TEST(Statistics, NegativeNumbers) {
    int items[] = {-5,-3,0,3,5};
    LazySequence<int> seq(items,5);   
    Statistics<int> stats(seq);

    EXPECT_EQ(stats.GetMin(), -5)
        << "input: Statistics(LazySeq{-5,-3,0,3,5}, 5) -> GetMin()"
        << "\nexpected: -5";
    EXPECT_EQ(stats.GetMax(), 5)
        << "input: Statistics(LazySeq{-5,-3,0,3,5}, 5) -> GetMax()"
        << "\nexpected: 5";
    EXPECT_DOUBLE_EQ(stats.GetAverage(), 0.0)
        << "input: Statistics(LazySeq{-5,-3,0,3,5}, 5) -> GetAverage()"
        << "\nexpected: 0.0";
}

TEST(Statistics, SingleElement) {
    int items[] = {100};
    LazySequence<int> seq(items,1);   
    Statistics<int> stats(seq);

    EXPECT_EQ(stats.GetCount(), 1u)
        << "input: Statistics(LazySeq{42}, 1) -> GetCount()"
        << "\nexpected: 1";
    EXPECT_EQ(stats.GetMin(), 100)
        << "input: Statistics(LazySeq{42}, 1) -> GetMin()"
        << "\nexpected: 42";
    EXPECT_EQ(stats.GetMax(), 100)
        << "input: Statistics(LazySeq{42}, 1) -> GetMax()"
        << "\nexpected: 42";
    EXPECT_DOUBLE_EQ(stats.GetAverage(), 100.0)
        << "input: Statistics(LazySeq{42}, 1) -> GetAverage()"
        << "\nexpected: 42.0";
}

TEST(Statistics, Reset) {
    int items[] = {1,2,3};
    LazySequence<int> seq(items,3);   
    Statistics<int> stats(seq);
    stats.Reset();

    EXPECT_EQ(stats.GetCount(), 0u)
        << "input: Statistics(LazySeq{1,2,3}, 3) -> Reset() -> GetCount()"
        << "\nexpected: 0";
    EXPECT_EQ(stats.GetSum(), 0)
        << "input: Statistics(LazySeq{1,2,3}, 3) -> Reset() -> GetSum()"
        << "\nexpected: 0";
    EXPECT_DOUBLE_EQ(stats.GetAverage(), 0.0)
        << "input: Statistics(LazySeq{1,2,3}, 3) -> Reset() -> GetAverage()"
        << "\nexpected: 0.0";
}

TEST(Statistics, FromInfiniteSequence) {
    int seed[] = {1};
    LazySequence<int> seq(
        [](const ArraySequence<int>& c) { return c.Get(c.GetLength()-1) + 1; },
        seed, 1
    );
    Statistics<int> stats(seq,5);

    EXPECT_EQ(stats.GetCount(), 5u)
        << "input: Statistics(InfiniteSeq{1,2,3,...}, 5) -> GetCount()"
        << "\nexpected: 5";
    EXPECT_EQ(stats.GetMin(), 1)
        << "input: Statistics(InfiniteSeq{1,2,3,...}, 5) -> GetMin()"
        << "\nexpected: 1";
    EXPECT_EQ(stats.GetMax(), 5)
        << "input: Statistics(InfiniteSeq{1,2,3,...}, 5) -> GetMax()"
        << "\nexpected: 5";
    EXPECT_DOUBLE_EQ(stats.GetAverage(), 3.0)
        << "input: Statistics(InfiniteSeq{1,2,3,...}, 5) -> GetAverage()"
        << "\nexpected: 3.0";
}

//////

TEST(Statistics, DefaultConstructor) {
    Statistics<int> stats;

    EXPECT_EQ(stats.GetCount(), 0u)
        << "input: Statistics() -> GetCount()"
        << "\nexpected: 0";
    EXPECT_EQ(stats.GetSum(), 0)
        << "input: Statistics() -> GetSum()"
        << "\nexpected: 0";
    EXPECT_EQ(stats.GetMin(), 0)
        << "input: Statistics() -> GetMin()"
        << "\nexpected: 0";
    EXPECT_EQ(stats.GetMax(), 0)
        << "input: Statistics() -> GetMax()"
        << "\nexpected: 0";
    EXPECT_DOUBLE_EQ(stats.GetAverage(), 0.0)
        << "input: Statistics() -> GetAverage()"
        << "\nexpected: 0.0";
}