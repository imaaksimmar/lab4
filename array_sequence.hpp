#pragma once
#include "sequence.hpp"
#include "dynamic_array.hpp"
#include "linked_list.hpp"
#include "exceptions.hpp"


template <typename T>
class ArraySequence : public Sequence<T> {
private:
    DynamicArray<T>* array;

public:
    ArraySequence() : array(new DynamicArray<T>(0)) {}

    ArraySequence(T* items, size_t count) : array(new DynamicArray<T>(items, count)) {}

    ArraySequence(const ArraySequence<T>& other) : array(new DynamicArray<T>(*other.array)) {}

    ArraySequence(const LinkedList<T>& list) 
        : array(new DynamicArray<T>(list.GetLength())) {
        for(size_t i=0; i<list.GetLength(); i++) {
            array->Set(i, list.Get(i));
        }
    }
    
    ~ArraySequence() { delete array; }

    ArraySequence<T>& operator=(const ArraySequence<T>& other) {
        if(this == &other) {
            return *this;
        }
        delete array;
        array = new DynamicArray<T>(*other.array);
        return *this;
    }

    T& operator[](size_t index) { 
        return (*array)[index]; 
    }

    size_t GetLength() const override {
        return array->GetSize();
    }

    T GetFirst() const override { 
        if(array->GetSize() == 0)
            throw EmptyCollection();
        return array->Get(0);
    }

    T GetLast() const override {
        if(array->GetSize() == 0)
            throw EmptyCollection();
        return array->Get(array->GetSize() - 1);
    }

    T Get(size_t index) const override {
        if(index>=array->GetSize())
            throw IndexOutOfRange(index, array->GetSize());
        return array->Get(index);
    }

    Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) const override {
        if(endIndex>=array->GetSize()) {
            throw IndexOutOfRange(endIndex, array->GetSize());
        }
        
        if(startIndex > endIndex) {
            throw InvalidArgument("startIndex (" + std::to_string(startIndex) + 
                        ") is strictly greater than endIndex (" + std::to_string(endIndex) + ")");
        }
        
        ArraySequence<T>* result = new ArraySequence<T>();
        for(size_t i=startIndex; i<=endIndex; i++) {
            result->Append(array->Get(i));
        }
        return result;
    }

    Sequence<T>* Append(T item) override {
        array->Resize(array->GetSize() + 1);
        array->Set(array->GetSize() - 1, item);
        return this;
    }

    Sequence<T>* Prepend(T item) override {
        size_t oldSize = array->GetSize();
        array->Resize(oldSize + 1);
        
        for(size_t i=oldSize; i>0; i--) {
            array->Set(i, array->Get(i - 1));
        }
        array->Set(0, item);
        return this;
    }

    Sequence<T>* InsertAt(T item, size_t index) override {
        if(index>array->GetSize())
            throw IndexOutOfRange(index, array->GetSize());
        
        size_t oldSize = array->GetSize();
        array->Resize(oldSize + 1);
        
        for(size_t i = oldSize; i>index; i--) {
            array->Set(i, array->Get(i -1)); 
        }
        array->Set(index, item);
        return this;
    }

    Sequence<T>* Concat(Sequence<T>* otherSequence) override {
        if(otherSequence == nullptr) {
            throw NullPointer();
        }
        for(size_t i=0; i<otherSequence->GetLength(); i++) {
            this->Append(otherSequence->Get(i));
        }
        return this;
    }

    template <typename U>
    ArraySequence<U>* Map(U (*func)(T)) const {
        ArraySequence<U>* res = new ArraySequence<U>();
        res->array->Resize(array->GetSize()); 
        for(size_t i=0; i<array->GetSize(); i++) {
            res->array->Set(i, func(array->Get(i))); 
        }
        return res;
    }

    ArraySequence<T>* Where(bool (*predicate)(T)) const {
        ArraySequence<T>* res = new ArraySequence<T>();
        for(size_t i=0; i<array->GetSize(); i++) {
            if(predicate(array->Get(i))) {
                res->Append(array->Get(i));
            }
        }
        return res;
    }

    template <typename U>
    U Reduce(U (*func)(U, T), U initial) const {
        U res = initial;
        for(size_t i=0; i<array->GetSize(); i++) {
            res = func(res, array->Get(i));
        }
        return res;
    }
};
