#include <iostream>
#include "rbtree.hpp"
#include "vector"

template<typename T>
struct listNode {
    listNode(): next(nullptr), prev(nullptr), data(){

    }
    listNode(T value): next(nullptr), prev(nullptr), data(value){

    }



    listNode<T>* next;
    listNode<T>* prev;
    T data;
};
template <typename T>
class double_linked_list{
private:

    listNode<T>* _begin;
    listNode<T>* _end;
    std::size_t _size;

public:
    double_linked_list(): _begin(nullptr), _end(nullptr), _size(0){

    }

    ~double_linked_list(){
        listNode<T>* temp = _begin;
        listNode<T>* next = nullptr;
        while(temp!= nullptr){
            next = temp -> next;
            delete temp;
            temp = next;
        }
    }

    void push_back(T value){
        listNode<T> *temp = new listNode(value);
        if(_size == 0){
            _begin = _end = temp;
            ++_size;
        }
        else if(_begin == _end){
            _begin -> next = temp;
            temp -> prev = _begin;
            _end = temp;
            ++_size;
        }
        else{
            temp->prev = _end;
            _end->next = temp;
            _end = temp;
            ++_size;
        }
    }

    void push_forward(T value){
        listNode<T>* temp = new listNode(value);
        temp -> next = _begin;
        _begin -> prev = temp;
        _begin = temp;
        ++_size;
    }

    listNode<T>* begin() const{
        return _begin;
    }

    listNode<T>* end() const{
        return _end;
    }

    void remove_back(){
        if (_size > 1){
            listNode<T>* temp = _end;
            _end = _end->prev;
            delete temp;
            --_size;
        }
        else if(_size == 1){
            delete _end;
            _begin = _end = nullptr;
            _size = 0;
        }
    }

    std::size_t size()const{
        return _size;
    }


};

template <typename T>
class set{
private:
    double_linked_list<RBTreeNode<T>*> element_view;
    RedBlackTree<T> tree;
    std::size_t _size;



public:
    set():element_view(), tree(), _size(0){

    }

    set(std::iterator<std::input_iterator_tag,T> begin, std::iterator<std::input_iterator_tag, T> end):element_view(), tree(), _size(){
        while(begin != end){
            tree.insert(*begin);
            ++_size;
            ++begin;
        }
    }



    set(std::initializer_list<T> list): element_view(), tree(), _size(0){
        for(auto i = list.begin(); i != list.end(); ++i){
            tree.insert(*i);
            ++_size;
        }
    }

    set(set& second):element_view(), tree(), _size(second._size){
        std::vector<RBTreeNode<T>*> view = second.tree.get_elements_increasing_order();
        for (std::size_t i = 0; i < second._size; ++i) {
            tree.insert(view.at(i) -> data);
        }
    }



    ~set() = default;

    void refresh_view(){
        std::vector<RBTreeNode<T>*> temp = tree.get_elements_increasing_order();
        listNode<RBTreeNode<T>*>* temp_node = element_view.begin();
        for (auto i = temp.begin(); i != temp.end(); ++i) {
            if(temp_node != nullptr){
                temp_node -> data = *i;
                temp_node = temp_node -> next;
            }
            else{
                element_view.push_back(*i);
            }
        }
        while(element_view.size() > temp.size()){
            element_view.remove_back();
        }
    }
    void refresh_view(T key){
        listNode<RBTreeNode<T>*> required = begin();
        std::vector<RBTreeNode<T>*> temp = tree.get_elements_increasing_order();
        listNode<RBTreeNode<T>*>* temp_node = element_view.begin();
        for (auto i = temp.begin(); i != temp.end(); ++i) {
            if(temp_node != nullptr){
                temp_node -> data = *i;
                required = (*i == key)?temp_node: required;
                temp_node = temp_node -> next;
            }
            else{
                element_view.push_back(*i);
                required = (*i == key)? element_view.end(): required;
            }
        }
        while(element_view.size() > temp.size()){
            element_view.remove_back();
        }
    }



    void insert(T key){
        tree.insert(key);
        _size++;
    }

    void erase(T key){
        RBTreeNode<T>* temp_node = tree.find(key);
        if (temp_node == nullptr){
            return;
        }
        tree.remove(temp_node);
        _size--;
    }

    std::size_t size()const{
        return _size;
    }

    class iterator;

    iterator begin()  {
        refresh_view();
        return iterator(element_view.begin());
    }

    iterator end()  {
        refresh_view();
        return iterator(element_view.end());
    }

    iterator find(T key) const {
        return iterator(refresh_view(key));
    }

    bool empty() const {
        return _size == 0;
    }

};



template <typename T> class set<T>::iterator{
private:
    friend class set;
    listNode<RBTreeNode<T>*>* current;

public:
    iterator(listNode<RBTreeNode<T>*>* node_ptr):current(node_ptr){

    }
    iterator& operator ++(){
        if(current != nullptr){
            current = current -> next;
        }
        return *this;
    }

    bool operator ==(const iterator& second){
        return (current == second.current);
    }

    iterator operator ++(int){
        if(current != nullptr){
            current = current -> next;
        }
        return *this;
    }

    iterator& operator --(){
        if (current != nullptr){
            current = current -> prev;
        }
        return *this;
    }

    iterator operator --(int){
        if (current != nullptr){
            current = current -> prev;
        }
        return *this;
    }

    bool operator !=(iterator second){
        return current != second.current;
    }

    T operator *(){
        return current -> data -> data;
    }

};


