#include <cstddef>
#include <iterator>
#include <memory_resource>
#include <cstdlib>
#include <list>
#include <new>
#include <iostream>
#include <stdexcept>

class DynamicListMemoryResource: public std::pmr::memory_resource{
    private: 
        // Указатель на конкретное место в памяти, где блок будет выделен после NEW, size_ - размер блока, 
        // is free - проверка на переиспользование
        struct Block{
            void* ptr;
            size_t size_;
            bool is_free;
        };
        std::list <Block> blocks;
    public:
        void* do_allocate(size_t bytes, size_t alignment) override{
            for (auto& block : blocks){
                if (block.is_free && block.size_ >= bytes){
                    block.is_free = false;
                    return block.ptr;
                }
            }
            void* new_memory = ::operator new(bytes);
            blocks.push_back({new_memory, bytes, false});
            return new_memory;
        }

        void do_deallocate(void* p, size_t bytes, size_t alignment) override{
            for(auto& block : blocks){
                if( p == block.ptr){
                    block.is_free = true;
                    return;
                }
            }
        }
        bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override{
            return this == &other;
        }
        ~DynamicListMemoryResource() override {
            for (auto& block : blocks) {
                if (!block.is_free) { 
                    ::operator delete(block.ptr);
                }
            }
        }
};

template<typename T, typename Allocator = std::pmr::polymorphic_allocator<T>>
class Queue{
private:
    struct Node{
        T data;
        Node* next;
    };
    Node* head;
    Node* tail;
    size_t size_;
    using NodeAllocator = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
    NodeAllocator alloc;

public:
    class iterator {
    private:
        Node* current;
    public:
        iterator(Node* node) : current(node) {}
        
        T& operator*() {
            return current->data;
        }
        
        T* operator->() {
            return &(current->data);
        }
        
        // Префиксный ++
        iterator& operator++() {
            if (current) {
                current = current->next;
            }
            return *this;
        }
        // Постфиксный ++
        iterator operator++(int) {
            iterator temp = *this;
            ++(*this);
            return temp;
        }
        
        bool operator==(const iterator& other) const {
            return current == other.current;
        }
        
        bool operator!=(const iterator& other) const {
            return current != other.current;
        }
    };

    class const_iterator {
    private:
        const Node* current;
    public:
        const_iterator(const Node* node) : current(node) {}
        
        const T& operator*() const {
            return current->data;
        }
        
        const T* operator->() const {
            return &(current->data);
        }
        
        const_iterator& operator++() {
            if (current) {
                current = current->next;
            }
            return *this;
        }
        
        const_iterator operator++(int) {
            const_iterator temp = *this;
            ++(*this);
            return temp;
        }
        
        bool operator==(const const_iterator& other) const {
            return current == other.current;
        }
        
        bool operator!=(const const_iterator& other) const {
            return current != other.current;
        }
    };
    Queue() : head(nullptr), tail(nullptr), size_(0) {}
    Queue(const Allocator& allocator) : alloc(allocator), head(nullptr), tail(nullptr), size_(0) {}
    
    iterator begin() {
        return iterator(head);
    }
    
    iterator end() {
        return iterator(nullptr);
    }
    
    const_iterator begin() const {
        return const_iterator(head);
    }
    
    const_iterator end() const {
        return const_iterator(nullptr);
    }

    void push(const T& value){
        Node* new_node = alloc.allocate(1);
        new_node->data = value;
        new_node->next = nullptr;
        if (empty()){
            head = tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
        size_++;
    }
    
    void pop(){
        if(!empty()){
            Node* old_head = head;
            head = head->next;
            if (head == nullptr){
                tail = nullptr;
            }
            alloc.destroy(old_head);
            alloc.deallocate(old_head, 1);
            size_--;
        }
        else{
            std::cout << "Ошибка удаления из пустого." << std::endl;
        }
    }
    
    T& front(){
        if(empty()){
            throw std::runtime_error("Queue is empty");
        }
        return head->data;
    }
    
    const T& front() const {
        if(empty()){
            throw std::runtime_error("Queue is empty");
        }
        return head->data;
    }
    
    T& back(){
        if(empty()){
            throw std::runtime_error("Queue is empty");
        }
        return tail->data;
    }
    
    const T& back() const {
        if(empty()){
            throw std::runtime_error("Queue is empty");
        }
        return tail->data;
    } 
    bool empty() const {
        return size_ == 0;
    }

    size_t size() const {
        return size_;
    }

    ~Queue() {
        while (!empty()) pop();
    }  
};
