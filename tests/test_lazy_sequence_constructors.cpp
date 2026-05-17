#include <gtest/gtest.h>
#include "lazy_sequence.hpp"
#include "array_sequence.hpp"
#include "exceptions.hpp"

TEST(LazySequenceConstructors, DefaultConstructor) {
    LazySequence<int> seq;

    EXPECT_EQ(seq.GetLength().value, 0u)
        << "input: LazySequence<int> seq -> seq.GetLength().value"
        << "\nexpected: 0";
    EXPECT_FALSE(seq.GetLength().isInfinite)
  << "input: LazySequence<int> seq -> seq.GetLength().isInfinite"
        << "\nexpected: false";
}

TEST(LazySequenceConstructors, FromArrayConstructor) {
    int items[] = {1,2,3,4,5};
    LazySequence<int> seq(items, 5);

    EXPECT_EQ(seq.GetLength().value, 5u)
        << "input: LazySequence<int> seq({1,2,3,4,5}, 5) -> seq.GetLength().value"
        << "\nexpected: 5";
    EXPECT_EQ(seq.Get(0), 1)
        << "input: LazySequence<int> seq({1,2,3,4,5}, 5) -> seq.Get(0)"
        << "\nexpected: 1";
    EXPECT_EQ(seq.Get(4), 5)
        << "input: LazySequence<int> seq({1,2,3,4,5}, 5) -> seq.Get(4)"
        << "\nexpected: 5";
}

TEST(LazySequenceConstructors, FromSequenceConstructor) {
    int items[] = {10,20,30};
    ArraySequence<int> arr(items, 3);
    LazySequence<int> seq(&arr);

    EXPECT_EQ(seq.GetLength().value, 3u)
        << "input: LazySequence<int> seq(&ArraySequence{10,20,30}) -> seq.GetLength().value"
        << "\nexpected: 3";
    EXPECT_EQ(seq.Get(1), 20)
        << "input: LazySequence<int> seq(&ArraySequence{10,20,30}) -> seq.Get(1)"
        << "\nexpected: 20";
}

TEST(LazySequenceConstructors, CopyConstructor) {
    int items[] = {1,2,3};
    LazySequence<int> seq(items, 3);
    LazySequence<int> copy(seq);

    EXPECT_EQ(copy.GetLength().value, 3u)
        << "input: LazySequence copy(seq{1,2,3}) -> copy.GetLength().value"
        << "\nexpected: 3";
    EXPECT_EQ(copy.Get(0), 1)
        << "input: LazySequence copy(seq{1,2,3}) -> copy.Get(0)"
        << "\nexpected: 1";
}

TEST(LazySequenceConstructors, CopyConstructorDeep) {
    int items[] = {1,2,3};
    LazySequence<int> seq(items, 3);
    LazySequence<int> copy(seq);
    copy.Append(99);

    EXPECT_EQ(seq.GetLength().value, 3u)
        << "input: copy(seq) -> copy.Append(99) -> seq.GetLength().value"
        << "\nexpected: 3 (оригинал не изменился)";
}

TEST(LazySequenceConstructors, NullPointerThrows) {
    EXPECT_THROW(LazySequence<int>(nullptr, 5), NullPointer)
        << "input: LazySequence<int>(nullptr, 5)"
        << "\nexpected: throw NullPointer";

    EXPECT_THROW(LazySequence<int>((Sequence<int>*)nullptr), NullPointer)
        << "input: LazySequence<int>((Sequence<int>*)nullptr)"
        << "\nexpected: throw NullPointer";
}