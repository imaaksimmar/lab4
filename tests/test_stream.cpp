#include <gtest/gtest.h>
#include "stream.hpp"
#include "array_sequence.hpp"
#include "exceptions.hpp"

TEST(ReadOnlyStream, FromSequence) {
    int items[] = {1,2, };
    ArraySequence<int> arr(items, 3);
    ReadOnlyStream<int> stream(&arr);

    EXPECT_EQ(stream.GetPosition(), 0u)
        << "input: ReadOnlyStream(arr{1,2,3}) -> GetPosition()"
        << "\nexpected: 0";
    EXPECT_FALSE(stream.IsEndOfStream())
        << "input: ReadOnlyStream(arr{1,2,3}) -> IsEndOfStream()"
        << "\nexpected: false";
}

TEST(ReadOnlyStream, Read) {
    int items[] = {10,20,30};
    ArraySequence<int> arr(items, 3);
    ReadOnlyStream<int> stream(&arr);

    EXPECT_EQ(stream.Read(), 10)
        << "input: ReadOnlyStream{10,20,30}.Read()"
        << "\nexpected: 10";
    EXPECT_EQ(stream.Read(), 20)
        << "input: ReadOnlyStream{10,20,30}.Read()"
        << "\nexpected: 20";
    EXPECT_EQ(stream.Read(), 30)
        << "input: ReadOnlyStream{10,20,30}.Read()"
        << "\nexpected: 30";
    EXPECT_TRUE(stream.IsEndOfStream())
        << "input: после 3 Read() -> IsEndOfStream()"
        << "\nexpected: true";
}

TEST(ReadOnlyStream, EndOfStreamThrows) {
    int items[] = {1};
    ArraySequence<int> arr(items, 1);
    ReadOnlyStream<int> stream(&arr);

    stream.Read();
    EXPECT_THROW(stream.Read(), EndOfStream)
        << "input: ReadOnlyStream{1}.Read() после конца"
        << "\nexpected: throw EndOfStream";
}

TEST(ReadOnlyStream, GetPosition) {
    int items[] = {1,2,3};
    ArraySequence<int> arr(items, 3);
    ReadOnlyStream<int> stream(&arr);

    stream.Read();
    stream.Read();
    EXPECT_EQ(stream.GetPosition(), 2u)
        << "input: после двух Read() -> GetPosition()"
        << "\nexpected: 2";
}

TEST(ReadOnlyStream, Seek) {
    int items[] = {10,20,30,40,50};
    ArraySequence<int> arr(items, 5);
    ReadOnlyStream<int> stream(&arr);

    stream.Seek(3);
    EXPECT_EQ(stream.GetPosition(), 3u)
        << "input: ReadOnlyStream{10,20,30,40,50}.Seek(3) -> GetPosition()"
        << "\nexpected: 3";
    EXPECT_EQ(stream.Read(), 40)
        << "input: после Seek(3) -> Read()"
        << "\nexpected: 40";
}

TEST(ReadOnlyStream, Close) {
    int items[] = {1,2,3};
    ArraySequence<int> arr(items, 3);
    ReadOnlyStream<int> stream(&arr);

    stream.Read();
    stream.Read();
    stream.Close();
    EXPECT_EQ(stream.GetPosition(), 0u)
        << "input: после двух Read() -> Close() -> GetPosition()"
        << "\nexpected: 0 (Close сбрасывает позицию)";
}

TEST(WriteOnlyStream, Write) {
    LazySequence<int> target;
    WriteOnlyStream<int> stream(&target);

    EXPECT_EQ(stream.Write(10), 1u)
        << "input: WriteOnlyStream.Write(10)"
        << "\nexpected: 1 (первая запись)";
    EXPECT_EQ(stream.Write(20), 2u)
        << "input: WriteOnlyStream.Write(20)"
        << "\nexpected: 2 (вторая запись)";
    EXPECT_EQ(stream.GetPosition(), 2u)
        << "input: после двух Write() -> GetPosition()"
        << "\nexpected: 2";
}