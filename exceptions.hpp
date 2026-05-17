#pragma once
#include <exception>
#include <string>


class Exceptions : public std::exception {
protected:
    std::string message;
public:
    Exceptions(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

class IndexOutOfRange : public Exceptions {
public:
    IndexOutOfRange(size_t index, size_t size) 
        : Exceptions("IndexOutOfRange: requested index " + std::to_string(index) + 
                    ", BUT actual size is " + std::to_string(size)) {}
                    
};

class InvalidArgument : public Exceptions {
public:    
    InvalidArgument(size_t row, size_t col) 
        : Exceptions("InvalidArgument: access to upper triangle is forbidden (Row: " + 
                    std::to_string(row) + ", Column: " + std::to_string(col) + ")") {}
                    
    InvalidArgument(const std::string& details) 
        : Exceptions("InvalidArgument: " + details) {}
        
};

class SizeMismatch : public Exceptions {
public:   
    SizeMismatch(size_t size1, size_t size2) 
        : Exceptions("SizeMismatch: sizes do not match (" + 
                    std::to_string(size1) + " vs " + std::to_string(size2) + ")") {}
                    
};

class NullPointer : public Exceptions {
public:
    NullPointer() : Exceptions("NullPointer: data pointer is null") {}
};

class EmptyCollection : public Exceptions {
public:    
    EmptyCollection() : Exceptions("EmptyCollection: sequence is empty") {}
};

class EndOfStream : public Exceptions {
public:
    EndOfStream() : Exceptions("EndOfStream: no more elements to read") {}
};


