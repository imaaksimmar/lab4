#pragma once
#include "sequence.hpp"
#include "lazy_sequence.hpp"
#include "exceptions.hpp"

template <typename T>
class ReadOnlyStream {
private:
    LazySequence<T>* source;
    size_t position;
    bool isFinite;
    bool ownsSource; 

public:
    ReadOnlyStream(Sequence<T>* sequence) : position(0), isFinite(true), ownsSource(true)
    {
        if(sequence==nullptr) { throw NullPointer(); }
        source = new LazySequence<T>(sequence);
    }

    ReadOnlyStream(LazySequence<T>* lazySeq) : position(0), ownsSource(false)
    {
        if(lazySeq==nullptr) { throw NullPointer(); }
        source = lazySeq;
        isFinite = !lazySeq->GetLength().isInfinite;
    }

    ~ReadOnlyStream() { if(ownsSource) { delete source; } }

    bool IsEndOfStream() const 
    {
        if(!isFinite) { return false; }
        return position >= source->GetMaterializedCount();
    }

    T Read() 
    {
        if(IsEndOfStream()) { throw EndOfStream(); }
        T item = source->Get(position);
        position++;
        return item;
    }

    size_t GetPosition() const { return position; }

    bool IsCanSeek() const { return true; }

    size_t Seek(size_t index) 
    {
        if(isFinite && index>=source->GetMaterializedCount()) {
            throw IndexOutOfRange(index, source->GetMaterializedCount());
        }
        position = index;
        return position;
    }

    bool IsCanGoBack() const { return true; }

    void Open() {}
    void Close() { position = 0; }
};


template <typename T>
class WriteOnlyStream {
private:
    LazySequence<T>* target;
    size_t position;
    bool ownsTarget; 

public:
    WriteOnlyStream(Sequence<T>* sequence) : position(0), ownsTarget(true)
    {
        if(sequence==nullptr) { throw NullPointer(); }
        target = new LazySequence<T>(sequence);
    }

    WriteOnlyStream(LazySequence<T>* lazySeq) : position(0), ownsTarget(false)
    {
        if(lazySeq==nullptr) { throw NullPointer(); }
        target = lazySeq;
    }

    ~WriteOnlyStream() 
    { 
        if(ownsTarget) { delete target; }
    }

    size_t GetPosition() const { return position; }

    size_t Write(T item) 
    {
        target->Append(item);
        position++;
        return position;
    }

    void Open() {}
    void Close() { position = 0; }
};