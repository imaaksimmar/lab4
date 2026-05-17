#pragma once
#include "exceptions.hpp"
#include <cstddef> 

template <typename T>  
class LinkedList {
private:
    struct Node {
        T value;
        Node* next;
        Node(const T& val) : value(val), next(nullptr) {} 
    };
    Node* head; 

public:
    LinkedList() : head(nullptr) {}

    LinkedList(T* items, size_t count) : head(nullptr) {
        if(items==nullptr && count> 0) 
            throw InvalidArgument("LinkedList: items pointer is null but count > 0");
            
        for(size_t i=0; i<count; i++) {
            Append(items[i]);  
        }
    }

    LinkedList(const LinkedList<T>& otherLinkedList) : head(nullptr) {
        Node* current = otherLinkedList.head;
        while(current != nullptr) {
            Append(current->value);  
            current = current->next;
        }
    }

    LinkedList<T>& operator=(const LinkedList<T>& other) { 
        if(this == &other) {
            return *this;
        }

        while(head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }   
        Node* current = other.head;
        while(current != nullptr) {
            Append(current->value);  
            current = current->next;
        }
        return *this;
    }

    T& operator[](size_t index) {
        size_t currentLength = GetLength();
        if(index>=currentLength) {
            throw IndexOutOfRange(index, currentLength);
        }
        Node* current = head;
        for(size_t i=0; i<index; i++)
            current = current->next;
        return current->value; 
    }

    const T& operator[](size_t index) const {
        size_t currentLength = GetLength();
        if(index>=currentLength) {
            throw IndexOutOfRange(index, currentLength);
        }
        Node* current = head;
        for(size_t i=0; i<index; i++)
            current = current->next;
        return current->value;
    }

    ~LinkedList() {  
        while(head != nullptr) {
            Node* temp = head;      
            head = head->next;    
            delete temp;            
        }
    }

    T GetFirst() const {  
        if(head == nullptr) {
            throw EmptyCollection();
        }
        return head->value;
    }

    T GetLast() const {  
        if(head == nullptr) {  
            throw EmptyCollection();
        }
        Node* current = head;
        while(current->next != nullptr) {  
            current = current->next;
        }
        return current->value;
    }

    T Get(size_t index) const {  
        size_t currentLength = GetLength();
        if(index>=currentLength) {
            throw IndexOutOfRange(index, currentLength); 
        }
        Node* current = head;
        for(size_t i=0; i<index; i++) {  
            current = current->next;
        }
        return current->value;
    }

    size_t GetLength() const { 
        size_t count = 0;
        Node* current = head;
        while(current != nullptr) {
            count++;
            current = current->next;
        }
        return count;
    }

    LinkedList<T>* GetSubList(size_t startIndex, size_t endIndex) const {  
        size_t currentLength = GetLength();
        if(endIndex>=currentLength) {
            throw IndexOutOfRange(endIndex, currentLength);
        }
        
        if(startIndex > endIndex) {
            throw InvalidArgument("GetSubList: startIndex (" + std::to_string(startIndex) + 
                                  ") > endIndex (" + std::to_string(endIndex) + ")");
        }
        LinkedList<T>* result = new LinkedList<T>();
        Node* current = head;
        
        for(size_t i=0; i<startIndex; i++) {
            current = current->next;
        }
   
        for(size_t i=startIndex; i<=endIndex; i++) {
            result->Append(current->value);
            current=current->next;
        }
        return result;
    }

    
    void Append(T item) {
        Node* newNode = new Node(item);  
        if(head==nullptr) {
            head = newNode;
        } 
        else {
            Node* current = head;
            while(current->next != nullptr) { 
                current = current->next;
            }
            current->next = newNode; 
        }
    }

    void Prepend(T item) {
        Node* newNode = new Node(item);
        newNode->next = head;  
        head = newNode;    
    }

    void InsertAt(T item, size_t index) {
        size_t currentLength = GetLength();
        if(index>currentLength) { 
            throw IndexOutOfRange(index, currentLength); 
        }
        if(index==0) {
            Prepend(item);
            return;
        }
        if(index==currentLength) {
            Append(item);
            return;
        }
        Node* newNode = new Node(item);
        Node* current = head;
        for(size_t i=0; i<index-1; i++) {  
            current = current->next;
        }
        newNode->next = current->next;  
        current->next = newNode;        
    }

    LinkedList<T>* Concat(LinkedList<T>* otherLinkedList) const { 
        if(otherLinkedList == nullptr) {
            throw NullPointer();
        }
        LinkedList<T>* result = new LinkedList<T>();
        
        Node* current = head;
        while(current != nullptr) {
            result->Append(current->value);
            current = current->next;
        }
        
        current = otherLinkedList->head;
        while(current != nullptr) {
            result->Append(current->value);
            current = current->next;
        }
        return result;
    }

};