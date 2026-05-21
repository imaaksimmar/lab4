#include <gtest/gtest.h>
#include "lazy_sequence.hpp"
#include "exceptions.hpp"

TEST(LazySequenceMethods, GetFirst) {
    int items[] = {10,20,30};
    LazySequence<int> seq(items, 3);

    EXPECT_EQ(seq.GetFirst(), 10)
        << "input: LazySequence{10,20,30}.GetFirst()"
        << "\nexpected: 10";
}

TEST(LazySequenceMethods, GetFirstEmptyThrows) {
    LazySequence<int> seq;

    EXPECT_THROW(seq.GetFirst(), EmptyCollection)
        << "input: LazySequence{}.GetFirst()"
        << "\nexpected: throw EmptyCollection";
}

TEST(LazySequenceMethods, GetLast) {
    int items[] = {10,20,30};
    LazySequence<int> seq(items, 3);

    EXPECT_EQ(seq.GetLast(), 30)
        << "input: LazySequence{10,20,30}.GetLast()"
        << "\nexpected: 30";
}


TEST(LazySequenceMethods, GetSubsequence) {
    int items[] = {10,20,30,40,50};
    LazySequence<int> seq(items, 5);

    LazySequence<int>* sub = seq.GetSubsequence(1, 3);
    EXPECT_EQ(sub->GetLength().GetValue(), 3u)
        << "input: LazySequence{10,20,30,40,50}.GetSubsequence(1,3) -> GetLength()"
        << "\nexpected: 3";
    EXPECT_EQ(sub->Get(0), 20)
        << "input: GetSubsequence(1,3) -> Get(0)"
        << "\nexpected: 20";
    EXPECT_EQ(sub->Get(2), 40)
        << "input: GetSubsequence(1,3) -> Get(2)"
        << "\nexpected: 40";
    delete sub;
}

TEST(LazySequenceMethods, Append) {
    LazySequence<int> seq;
    seq.Append(2);
    seq.Append(3);

    EXPECT_EQ(seq.Get(0), 2)
        << "input: Append(2),Append(3) -> Get(0)"
        << "\nexpected: 2";
    EXPECT_EQ(seq.Get(1), 3)
        << "input: Append(2),Append(3) -> Get(1)"
        << "\nexpected: 3";
}


TEST(LazySequenceMethods, AppendPrepend) {
    LazySequence<int> seq;
    seq.Prepend(1);

    EXPECT_EQ(seq.Get(0), 1)
        << "input: Prepend(1) -> Get(0)"
        << "\nexpected: 1";
}


TEST(LazySequenceMethods, InsertAt) {
    int items[] = {1,2,4};
    LazySequence<int> seq(items, 3);
    seq.InsertAt(3, 2);

    EXPECT_EQ(seq.Get(2), 3)
        << "input: LazySequence{1,2,4}.InsertAt(3,2) -> Get(2)"
        << "\nexpected: 3";
    EXPECT_EQ(seq.Get(3), 4)
        << "input: LazySequence{1,2,4}.InsertAt(3,2) -> Get(3)"
        << "\nexpected: 4";
}

TEST(LazySequenceMethods, Concat) {
    int a[] = {1,2,3};
    int b[] = {4,5};
    LazySequence<int> seq1(a, 3);
    LazySequence<int> seq2(b, 2);
    seq1.Concat(&seq2);

    EXPECT_EQ(seq1.GetLength().GetValue(), 5u)
        << "input: LazySequence{1,2,3}.Concat({4,5}) -> GetLength()"
        << "\nexpected: 5";
    EXPECT_EQ(seq1.Get(3), 4)
        << "input: LazySequence{1,2,3}.Concat({4,5}) -> Get(3)"
        << "\nexpected: 4";
}