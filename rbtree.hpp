#include <iostream>
#include <stack>
#include <vector>
#include "deque"


template <typename T>
struct RBTreeNode{
    RBTreeNode(): black(false), data(), left(), right(nullptr), parent(nullptr){

    }
    RBTreeNode(T element): black(false), data(element), left(nullptr), right(nullptr), parent(nullptr){

    }
    bool black;
    T data;
    RBTreeNode<T>* left;
    RBTreeNode<T>* right;
    RBTreeNode<T>* parent;
    bool isFictional(){
        if(left == nullptr && right == nullptr){
            return true;
        }
        else{
            return false;
        }
    }

};


template <typename T>
class RedBlackTree{
public:
    RedBlackTree(): root(nullptr){

    }

    RedBlackTree(RedBlackTree& tree) = delete;
    RedBlackTree& operator =(RedBlackTree& tree) = delete;

    ~RedBlackTree(){
        if (root == nullptr){
            return;
        }
        std::deque<RBTreeNode<T>*> removing_queue;
        removing_queue.push_back(root);

        while(!removing_queue.empty()){
            RBTreeNode<T>* front = removing_queue.front();
            removing_queue.pop_front();

            if(front -> left != nullptr){
                removing_queue.push_back(front -> left);
            }
            if (front -> right != nullptr){
                removing_queue.push_back(front -> right);
            }
            delete front;
        }
    }

    void insert(T element){
        if (root == nullptr){
            root = new RBTreeNode<T>(element);
            RBTreeNode<T>* fictional_left = new RBTreeNode<T>();
            RBTreeNode<T>* fictional_right = new RBTreeNode<T>();
            fictional_left -> black = fictional_right -> black = true;
            fictional_left -> parent = fictional_right -> parent = root;
            root -> left = fictional_left;
            root -> right = fictional_right;
            root -> black = true;
        }
        else {
            RBTreeNode<T>* temp = root;
            RBTreeNode<T>* temp_parent = root;
            while (!(temp -> isFictional()))
            {
                if (element < temp -> data){
                    temp_parent = temp;
                    temp = temp -> left;
                }
                else{
                    temp_parent = temp;
                    temp = temp -> right;
                }
            }

            delete temp;
            temp = new RBTreeNode<T>(element);
            temp -> parent = temp_parent;
            auto fictional_left = new RBTreeNode<T>();
            fictional_left -> black = true;
            auto fictional_right = new RBTreeNode<T>();
            fictional_right -> black = true;
            fictional_left -> parent = fictional_right -> parent = temp;
            temp -> left = fictional_left;
            temp -> right = fictional_right;

            if (temp -> parent -> black){
                return;
            }

            else if (!(temp -> parent -> parent -> right -> black) && !(temp -> parent -> parent -> left -> black)){
                RBTreeNode<T>* grandfather = temp -> parent -> parent;
                while(grandfather != root && grandfather != nullptr){
                    grandfather -> left -> black = true;
                    grandfather -> right -> black = true;
                    grandfather -> black = false;
                    temp = grandfather;
                    grandfather = (temp -> parent -> black)? nullptr : temp -> parent -> parent;
                }
                if (grandfather == root){
                    grandfather -> black = grandfather -> left -> black = grandfather -> right -> black = true;
                }
            }

            else if (temp -> parent -> parent -> right -> black){
                if (!node_is_left(temp)){
                    leftTurn(temp -> parent);
                    temp = temp -> left;
                }
                rightTurn(temp -> parent -> parent);
                temp -> parent -> black = true;
                temp -> parent -> right -> black = false;
            }

            else if (temp -> parent -> parent -> left -> black){
                if (node_is_left(temp)){
                    rightTurn(temp -> parent);
                    temp = temp -> right;
                }
                if(temp -> parent -> parent == root){
                    root = leftTurn(temp -> parent -> parent);
                }
                else{
                    leftTurn(temp -> parent -> parent);
                }
                temp -> parent -> black = true;
                temp -> parent -> left -> black = false;
            }
        }
    }

    void remove(RBTreeNode<T>* node){
        if (!(node -> left -> isFictional()) && !(node -> right -> isFictional())){
            RBTreeNode<T>* temp = node;
            temp = temp -> right;
            while(!(temp -> left -> isFictional())){
                temp = temp -> left;
            }
            node -> data = temp -> data;
            remove(temp);
        }
        else if(!(node -> black)){
            remove_red(node);
        }
        else if(node -> left -> isFictional() && node -> right -> isFictional()){
            remove_black_childless(node);
        }
        else{
            remove_black_one_child(node);
        }


    }

    RBTreeNode<T>* find(T key){
        RBTreeNode<T>* temp = root;
        while(!(temp -> isFictional()) && temp -> data != key){
            if(key < temp -> data){
                temp = temp -> left;
            }
            else{
                temp = temp -> right;
            }
        }
        if (temp -> data == key){
            return temp;
        }
        else{
            return nullptr;
        }
    }

    std::vector<RBTreeNode<T>*> get_elements_increasing_order(){
        if (root == nullptr){
            return std::vector<RBTreeNode<T>*>();
        }
        std::vector<RBTreeNode<T>*> to_return = std::vector<RBTreeNode<T>*>();
        std::stack<RBTreeNode<T>*> stack;

        RBTreeNode<T>* temp = root;
        stack.push(root);
        while(!stack.empty()){
            while(temp == stack.top() && !(temp -> left -> isFictional())){
                temp = temp -> left;
                stack.push(temp);
            }
            temp = stack.top();
            stack.pop();
            to_return.push_back(temp);

            if(!(temp -> right -> isFictional())){
                temp = temp -> right;
                stack.push(temp);
            }
        }
        return to_return;
    }


private:
    RBTreeNode<T>* root;
    RBTreeNode<T>* leftTurn(RBTreeNode<T>* node){
        RBTreeNode<T>* temp = node -> right;
        node -> right = temp -> left;
        temp -> parent = node -> parent;
        temp -> left = node;
        node -> parent = temp;
        return temp;
    }

    RBTreeNode<T>* rightTurn(RBTreeNode<T>* node){
        RBTreeNode<T>* temp = node -> left;
        node -> left = temp -> right;
        temp -> parent = node -> parent;
        temp -> right = node;
        node -> parent = temp;
        return temp;
    }

    bool node_is_left(RBTreeNode<T>* node){
        return (node->parent->left == node);
    }

    void remove_red(RBTreeNode<T>* removing_node){
        if (node_is_left(removing_node)){
            removing_node -> parent -> left = removing_node -> left;
            removing_node -> left -> parent = removing_node -> parent;
        }
        else {
            removing_node -> parent -> right = removing_node -> left;
            removing_node -> left -> parent = removing_node -> parent;
        }
        delete removing_node -> right;
        delete removing_node;
    }

    void remove_black_one_child(RBTreeNode<T>* removing_node){
        if(removing_node -> left -> isFictional()){
            removing_node -> data = removing_node -> right -> data;
            RBTreeNode<T>* temp = removing_node -> right;
            removing_node -> right = temp -> left;
            removing_node -> right -> parent = removing_node;
            delete temp -> right;
            delete temp;
        }
        else{
            removing_node -> data = removing_node -> left -> data;
            RBTreeNode<T>* temp = removing_node -> left;
            removing_node -> left = temp -> left;
            removing_node -> left -> parent = removing_node;
            delete temp -> right;
            delete temp;
        }
    }

    void remove_black_childless(RBTreeNode<T>* removing_node){

        delete removing_node -> right;
        RBTreeNode<T>* temp = removing_node;
        removing_node = removing_node -> left;
        removing_node -> parent = temp -> parent;
        delete temp;
        double_black_node_check(removing_node);

    }



    void double_black_node_check(RBTreeNode<T>* node){
        if (node == root){
            node -> black = true;
        }
            // Если узел левый
        else if(node_is_left(node) && !(node -> parent -> right -> black)){
            leftTurn(node -> parent);
            node -> parent -> parent -> black = true;
            node -> parent -> black = false;
            double_black_node_check(node);
        }
        else if(node_is_left(node) && node -> parent -> right -> black && node -> parent -> right -> left -> black && node -> parent -> right -> right -> black){
            node -> parent -> right -> black = false;
            if(node -> parent -> black){
                double_black_node_check(node -> parent);
            }
            else{
                node -> parent -> black = true;
            }
        }
        else if(node_is_left(node) && node -> parent -> right -> black && !(node -> parent -> right -> left -> black) && node -> parent -> right -> right -> black){
            rightTurn(node -> parent -> right);
            node -> parent -> right -> black = true;
            node -> parent -> right -> right -> black = false;
            double_black_node_check(node);
        }
        else if(node_is_left(node) && node -> parent -> right -> black && !(node -> parent -> right -> right -> black)){
            leftTurn(node -> parent);
            node -> parent -> parent -> black = node -> parent -> black;
            node -> parent -> black = true;
        }

            // Если узел правый
        else if(!node_is_left(node) && !(node -> parent -> left -> black)){
            rightTurn(node -> parent);
            node -> parent -> parent -> black = true;
            node -> parent -> black = false;
            double_black_node_check(node);
        }
        else if(!node_is_left(node) && node -> parent -> left -> black && node -> parent -> left -> left -> black && node -> parent -> left -> right -> black){
            node -> parent -> left -> black = false;
            if(node -> parent -> black){
                double_black_node_check(node -> parent);
            }
            else{
                node -> parent -> black = true;
            }
        }
        else if(!node_is_left(node) && node -> parent -> left -> black && !(node -> parent -> left -> right -> black) && node -> parent -> left -> left -> black){
            leftTurn(node -> parent -> left);
            node -> parent -> left -> black = true;
            node -> parent -> left -> left -> black = false;
            double_black_node_check(node);
        }
        else if(!node_is_left(node) && node -> parent -> left -> black && !(node -> parent -> left -> left -> black)){
            rightTurn(node -> parent);
            node -> parent -> parent -> black = node -> parent -> black;
            node -> parent -> black = true;
        }
    }


};




