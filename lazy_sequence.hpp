#pragma once
#include <functional>
#include "sequence.hpp"
#include "array_sequence.hpp"
#include "exceptions.hpp"

class Cardinal {
    bool isInfinite; 
    size_t value; 

    Cardinal(bool isInfinite, size_t val) : isInfinite(isInfinite), value(val) {}

    public:

    Cardinal(size_t val) : Cardinal(false, val) {}
    static Cardinal Infinite() { return Cardinal(true, 0); }

    bool IsInfinite() const { return isInfinite; }
    size_t GetValue() const { return value; }
};


template <typename T>
class LazySequence {
private:
    ArraySequence<T> cache;
    std::function<T(const ArraySequence<T>&)> generator; 
    bool isInfinite;

    void materializeUpTo(size_t index) 
    {
        if(index<cache.GetLength()) { return; }

        if(!isInfinite) { 
            throw IndexOutOfRange(index, cache.GetLength()); 
        }

        while(cache.GetLength()<=index) {     
            cache.Append(generator(cache));       
        }
    }

public:
    LazySequence() : isInfinite(false) {}

    LazySequence(T* items, size_t count) : isInfinite(false) 
    {
        if(items==nullptr) { 
            throw NullPointer(); 
        }
        for(size_t i=0; i<count; i++) { cache.Append(items[i]); }
    }

    LazySequence(Sequence<T>* sequence) : isInfinite(false) 
    {
        if(sequence==nullptr) { 
            throw NullPointer(); 
        }
        for(size_t i=0; i<sequence->GetLength(); i++) { 
            cache.Append(sequence->Get(i)); 
        }
    }

    LazySequence(const LazySequence<T>& other)
    : cache(other.cache), generator(other.generator), isInfinite(other.isInfinite) {}

    LazySequence(std::function<T(const ArraySequence<T>&)> gen,
                T* initialItems, size_t initialCount)
    : generator(gen), isInfinite(true) 
    {
        if(initialItems==nullptr) { 
            throw NullPointer(); 
        }
        if(initialCount==0) {
            throw InvalidArgument("LazySequence: generator requires at least 1 initial element, got 0");
        }
        for(size_t i=0; i<initialCount; i++) { cache.Append(initialItems[i]); }
    }

   T& operator[](size_t index) 
    {
        if(index>=cache.GetLength()) { materializeUpTo(index); }
        return cache[index]; 
    }

    T Get(size_t index) 
    {
        if(index>=cache.GetLength()) { materializeUpTo(index); }
        return (*this)[index];
    }

    T GetFirst() 
    {
        if(cache.GetLength()==0) { throw EmptyCollection(); }
        return (*this)[0];
    }

    T GetLast() 
    {
        if(isInfinite) { 
            throw InvalidArgument("GetLast: cannot get last element of infinite sequence"); 
        }
        if(cache.GetLength()==0) { throw EmptyCollection(); }
        return (*this)[cache.GetLength()-1];
    }   

    LazySequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) 
    {
        if(startIndex>endIndex) { 
            throw InvalidArgument(
                "GetSubsequence: startIndex (" + std::to_string(startIndex) + 
                ") > endIndex (" + std::to_string(endIndex) + ")"
            ); 
        }
        if(!isInfinite && endIndex>=cache.GetLength()) {
            throw IndexOutOfRange(endIndex, cache.GetLength());
        }
        Get(endIndex);
        LazySequence<T>* result = new LazySequence<T>();
        for(size_t i=startIndex; i<=endIndex; i++) {
            result->cache.Append(this->cache.Get(i));
        }
        return result;
    }

    size_t GetMaterializedCount() const { return cache.GetLength(); }

    Cardinal GetLength() const 
    {
        if(isInfinite) { return Cardinal::Infinite(); }
        return Cardinal(cache.GetLength());
    }

    LazySequence<T>* Append(T item) 
    {
        if(isInfinite) { 
            throw InvalidArgument("Append: cannot append to infinite sequence"); 
        }
        cache.Append(item);
        return this;
    }

    LazySequence<T>* Prepend(T item) 
    {
        cache.Prepend(item);
        return this;
    }

    LazySequence<T>* InsertAt(T item, size_t index) 
    {
        if(index>cache.GetLength()) { 
            throw IndexOutOfRange(index, cache.GetLength()); 
        }
        cache.InsertAt(item, index);
        return this;
    }

    LazySequence<T>* Concat(LazySequence<T>* other) 
    {
        if(other==nullptr) { throw NullPointer(); }
        if(isInfinite) { 
            throw InvalidArgument("Concat: cannot concat from infinite sequence"); 
        }
        for(size_t i=0; i<other->cache.GetLength(); i++) {
            cache.Append(other->cache.Get(i));
        }
        return this;
    }

    template <typename U>
    LazySequence<U>* Map(U (*func)(T), size_t count) 
    {
        if(func==nullptr) {
            throw NullPointer();
        }
        LazySequence<U>* result = new LazySequence<U>();
        for(size_t i=0; i<count; i++) {
            result->cache.Append(func(this->Get(i)));
        }
        return result;
    }

    LazySequence<T>* Where(bool (*pred)(T), size_t count) 
    {
        if(pred==nullptr) {
            throw NullPointer();
        }
        LazySequence<T>* result = new LazySequence<T>();
        for(size_t i=0; i<count; i++) {
            T item = this->Get(i);
            if(pred(item)) { result->cache.Append(item); }
        }
        return result;
    }

    template <typename U>
    U Reduce(U (*func)(U, T), U initial, size_t count) 
    {
        if(func==nullptr) {
            throw NullPointer();
        }
        U result = initial;
        for(size_t i=0; i<count; i++) {
            result = func(result, this->Get(i));
        }
        return result;
    }
};