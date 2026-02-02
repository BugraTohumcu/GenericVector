#ifndef CUSTOM_VECTOR
#define CUSTOM_VECTOR

#include <cstddef>
#include <type_traits>
#include <initializer_list> 
#include <concepts>
#include <memory>
#include <stdexcept>


namespace veclib{
    
    template<typename T, typename Allocator = std::allocator<T>>
    class vector{
        private:
        size_t _capacity;
        size_t _size;
        T* data = nullptr;
        Allocator alloc;
        public:
        
        //Constructors
        vector(std::initializer_list<T> list):vector(){
            append(std::move(list));
        }
        
        vector():_capacity(1), _size(0){
            this->data = alloc.allocate(this->_capacity);
        }
        
        //Constructor for callables
        template<std::convertible_to<T>... Args>
        vector(T &&t, Args&&... args):vector(){
            append(std::move(t));
            (append(std::forward<Args>(args)), ...);
        }
        
        ~vector(){
            for(size_t i = 0; i < _size; ++i)
            this->data[i].~T();        
            alloc.deallocate(data, _capacity);
        }
        
        //Functions
        template<std::convertible_to<T> U>
        void append(U&& value){
            if(this->_capacity == this->_size) resize();
            new (this->data + _size) T(std::forward<U>(value));
            ++_size;
        }
        
        void append(std::initializer_list<T> list){
            for(auto const &e: list){
                append(e);
            }
        }
        
        size_t size() const{
            return this->_size;
        }
        
        size_t capacity() const {
            return this->_capacity;
        }
        
        void reserve(size_t new_capacity){
            if(new_capacity <= _capacity) return;
            T* new_data = this->alloc.allocate(new_capacity);
            for(int i = 0; i < this->_size ; i++){
                new (new_data + i) T(std::move(data[i]));
            }
            
            for(int i= 0; i< this->_size; i++){
                this->data[i].~T();
            }
            alloc.deallocate(this->data, this->_capacity);
            this->_capacity = new_capacity;
            this->data = new_data;
        }
        
        T* begin() { return data; }
        const T* begin() const { return data; }
        
        T* end() { return data + _size; }
        const T* end() const { return data + _size; }
        
        //Operator Overloadings
        decltype(auto) operator[](size_t index){
            return at(index);
        }
        
        decltype(auto) operator[](size_t index) const{
            return at(index);
        }
        
        template<typename G>
        friend std::ostream& operator<<(std::ostream& os,veclib::vector<G>& v){
            os << "[";
            for(int i = 0; i< v.size(); i++){
                if(i == v.size()-1){
                    os << v[i];
                    continue;
                }
                os << v[i] << ", ";
            }
            os << "]";
            return os;
        }
 
        private:
        void resize(){
            size_t newCapacity = _capacity*2;
            reserve(newCapacity);
        }
        
        T& at(size_t index){
            if(index >= this->_size) throw std::out_of_range("Index out of bound!");
            return this->data[index];
        }
        
        const T& at(size_t index) const{
            if(index >= this->_size) throw std::out_of_range("Index out of bound!");
            return this->data[index];
        }
        
    };
    
    template<typename First, typename... Rest>
    vector(First, Rest...) -> vector<First>;

};// namespace veclib

#endif