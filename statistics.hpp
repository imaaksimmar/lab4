#pragma once
#include "stream.hpp"
#include "exceptions.hpp"

/* template <typename T>
class Statistics {
private:
    size_t count;    
    T sum;          
    T min;          
    T max;    
    double average;     

public:
    Statistics()  
    : count(0), sum(T()), min(T()), max(T()), average(0.0) {}

    size_t GetCount() const { return count; }
    T GetSum() const { return sum; }
    T GetMin() const { return min; }
    T GetMax() const { return max; }
    double GetAverage() const { return average; }

    void Reset() 
    {
        count = 0;
        sum = T();
        min = T();
        max = T();
        average = 0.0;
    }

    void Update(T item) 
    {
        if(count==0) {
            min = item;
            max = item;
        }
        count++;
        sum += item;

        if(item<min) { min = item; }
        if(item>max) { max = item; }

        average = static_cast<double>(sum)/count;
    }
// по констурктуру статистики 
    void Process(ReadOnlyStream<T>& stream, size_t n) 
    {
        for(size_t i=0; i<n; i++) {
            if(stream.IsEndOfStream()) { break; }
            Update(stream.Read());
        }
    }
};
*/


template <typename T>
class Statistics {
private:
    size_t count;    
    T sum;          
    T min;          
    T max;    
    double average;

    void Update(T item) 
    {
        if(count==0) {
            min = item;
            max = item;
        }
        count++;
        sum += item;
        if(item<min) { min = item; }
        if(item>max) { max = item; }
        average = static_cast<double>(sum)/count;
    }

public:
    Statistics()  
    : count(0), sum(T()), min(T()), max(T()), average(0.0) {}


    Statistics(LazySequence<T>& seq, size_t n)
    : count(0), sum(T()), min(T()), max(T()), average(0.0)
    {
        ReadOnlyStream<T> stream(&seq);
        for(size_t i=0; i<n; i++) {
            if(stream.IsEndOfStream()) { break; }
            Update(stream.Read());
        }
    }

    size_t GetCount() const { return count; }
    T GetSum() const { return sum; }
    T GetMin() const { return min; }
    T GetMax() const { return max; }
    double GetAverage() const { return average; }

    void Reset() 
    {
        count = 0;
        sum = T();
        min = T();
        max = T();
        average = 0.0;
    }
};
