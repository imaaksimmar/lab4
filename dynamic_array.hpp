#pragma once 
#include "exceptions.hpp"

template <typename T> 
class DynamicArray {
private:
    T* data;
    size_t size;
    
public:
    DynamicArray() : data(nullptr), size(0) {}

    DynamicArray(size_t count) : data(new T[count]), size(count) {}
    
    DynamicArray(T* items, size_t count) : DynamicArray(count) {
        if(count>0 && items == nullptr) {
            throw InvalidArgument("DynamicArray: items pointer is null but count > 0");
        }
        for(size_t i=0; i<size; i++) {
            data[i] = items[i];
        }
    }
    
    DynamicArray(const DynamicArray<T>& otherDynamicArray) 
    : DynamicArray(otherDynamicArray.data, otherDynamicArray.size) {}
    
    ~DynamicArray() { delete[] data; }
    
    DynamicArray<T>& operator=(const DynamicArray<T>& otherDynamicArray ) {
        if(this == &otherDynamicArray) { return *this; }
        
        delete[] data;
        size = otherDynamicArray.size;
        data = new T[size];
        for(size_t i=0; i<size; i++) {
            data[i] = otherDynamicArray.data[i];
        }
        return *this;
    }
    
    T& operator[](size_t index) {
        if(index>=size) {
            throw IndexOutOfRange(index, size);
        }
        return data[index];
    }

    const T& operator[](size_t index) const {
        if(index>=size) { 
            throw IndexOutOfRange(index, size);
         }
        return data[index];
    }   
    
    T Get(size_t index) const { 
        if(index >= size) { 
            throw IndexOutOfRange(index, size);
        }
        return data[index];
    }
    
    size_t GetSize() const {  
        return size;
    }
    
    void Set(size_t index, T value) {
        if(index>= size) {  
            throw IndexOutOfRange(index, size);
        }
        data[index] = value;
    }
    
    void Resize(size_t newSize) {
        T* newData = new T[newSize];
        size_t copyLen = (newSize<size) ? newSize : size;  
        
        for(size_t i=0; i<copyLen; i++) {
            newData[i] = data[i];
        }
        
        delete[] data;   
        data = newData; 
        size = newSize;  
    }
}; 