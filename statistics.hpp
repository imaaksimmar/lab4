#pragma once
#include "stream.hpp"
#include "exceptions.hpp"

template <typename T>
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

    Statistics(LazySequence<T>& seq)
    : count(0), sum(T()), min(T()), max(T()), average(0.0)
    {
        ReadOnlyStream<T> stream(seq);
        while(!stream.IsEndOfStream()) {  
            Update(stream.Read());
        }
    }

    Statistics(LazySequence<T>& seq, size_t n)
    : count(0), sum(T()), min(T()), max(T()), average(0.0)
    {
        ReadOnlyStream<T> stream(seq);
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

    void SetCount(size_t cnt) { count = cnt; }
    void SetSum(T s) { sum = s; }
    void SetMin(T mn) { min = mn; }
    void SetMax(T mx) { max = mx; }
    void SetAverage(double avg) { average = avg; }

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
};
