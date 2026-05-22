#pragma once
#include "sequence.hpp"
#include "lazy_sequence.hpp"
#include "exceptions.hpp"

template <typename T>
class ReadOnlyStream {
private:
    LazySequence<T>& source;
    size_t position;
    bool isFinite;

public:
    ReadOnlyStream(LazySequence<T>& lazySeq)
    : source(lazySeq), position(0)
    {
        isFinite = !lazySeq.GetLength().IsInfinite();
    }

    bool IsEndOfStream() const
    {
        if(!isFinite) { return false; }
        return position >= source.GetMaterializedCount();
    }

    T Read()
    {
        if(IsEndOfStream()) { throw EndOfStream(); }
        T item = source.Get(position);
        position++;
        return item;
    }

    T& operator[](size_t index)
    {
        if(isFinite && index >= source.GetMaterializedCount()) {
            throw IndexOutOfRange(index, source.GetMaterializedCount());
        }
        return source[index];
    }

    size_t GetPosition() const { return position; }

    size_t Seek(size_t index)
    {
        if(isFinite && index >= source.GetMaterializedCount()) {
            throw IndexOutOfRange(index, source.GetMaterializedCount());
        }
        position = index;
        return position;
    }

    void Open() {} 
    void Close() { position = 0; }
};



template <typename T>
class WriteOnlyStream {
private:
    LazySequence<T>& target;
    size_t position;

public:
    WriteOnlyStream(LazySequence<T>& lazySeq)
    : target(lazySeq), position(0) {}

    size_t GetPosition() const { return position; }

    size_t Write(T item)
    {
        target.Append(item);
        position++;
        return position;
    }

    void Open() {}
    void Close() { position = 0; }
};