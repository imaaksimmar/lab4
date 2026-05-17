#pragma once
#include "sequence.hpp"
#include "linked_list.hpp"
#include "exceptions.hpp"

template <typename T>
class ListSequence : public Sequence<T> {
private:
    LinkedList<T>* list;  
public:
    ListSequence() : list(new LinkedList<T>()) {}
    
    ListSequence(T* items, size_t count) 
    : list(new LinkedList<T>(items, count)) {}

    ListSequence(const ListSequence<T>& other) 
    : list(new LinkedList<T>(*other.list)) {}  

    ListSequence(const LinkedList<T>& sourceList) 
    : list(new LinkedList<T>(sourceList)) {}

    ~ListSequence() { delete list; }

    ListSequence<T>& operator=(const ListSequence<T>& other) {
    if(this == &other) {
        return *this;  
    }
    delete list;                          
    list = new LinkedList<T>(*other.list); 
    return *this;
    }

    T& operator[](size_t index) {
        return (*list)[index];  
    }


    T GetFirst() const override { 
        return list->GetFirst();  
    }

    T GetLast() const override {
        return list->GetLast();
    }

    T Get(size_t index) const override {
        return list->Get(index);
    }

    size_t GetLength() const override {
        return list->GetLength();
    }

    Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) const override {
        size_t currentLength = list->GetLength();
        if(endIndex >= currentLength) {
            throw IndexOutOfRange(endIndex, currentLength);
        }
    
        if(startIndex > endIndex) {
            throw InvalidArgument("GetSubsequence: startIndex (" + std::to_string(startIndex) + 
                                  ") > endIndex (" + std::to_string(endIndex) + ")");
        }
        
        LinkedList<T>* subList = list->GetSubList(startIndex, endIndex);
        ListSequence<T>* result = new ListSequence<T>(*subList);
        delete subList; 
        return result;
    }

    Sequence<T>* Append(T item) override {
        list->Append(item);  
        return this;        
    }

    Sequence<T>* Prepend(T item) override {
        list->Prepend(item);
        return this;
    }

    Sequence<T>* InsertAt(T item, size_t index) override {
        list->InsertAt(item, index);
        return this;
    }

    Sequence<T>* Concat(Sequence<T>* otherSequence) override {
        if(otherSequence == nullptr) {
            throw NullPointer();
        }
        for(size_t i=0; i<otherSequence->GetLength(); i++) {
            list->Append(otherSequence->Get(i));
        }
        return this;
    }
    
    template <typename U>
    ListSequence<U>* Map(U (*func)(T)) const {
        ListSequence<U>* res = new ListSequence<U>();
        for(size_t i=0; i<list->GetLength(); i++) {
            res->Append(func(list->Get(i)));
        }
        return res;
    }

    ListSequence<T>* Where(bool (*predicate)(T)) const {
        ListSequence<T>* res = new ListSequence<T>();
        for(size_t i=0; i<list->GetLength(); i++) {
            if(predicate(list->Get(i))) {
                res->Append(list->Get(i));
            }
        }
        return res;
    }

    template <typename U>
    U Reduce(U (*func)(U, T), U initial) const {
        U res = initial;
        for(size_t i=0; i<list->GetLength(); i++) {
            res = func(res, list->Get(i));
        }
        return res;
    }

};