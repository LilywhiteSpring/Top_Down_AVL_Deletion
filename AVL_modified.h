// Created by LilywhiteHaru on 2026/5/1.
#ifndef AVLTREE_AVL2_H
#define AVLTREE_AVL2_H
#include <stack>
#include "AVL.h"
#include <iostream>
#include <stack>

template <class KEY , class OTHER>
class AvlTree2 {
private:
    struct AVLNode {
        AVLNode  *l_child;
        AVLNode *r_child;
        set<KEY , OTHER> data;
        int height;

        explicit AVLNode(const set<KEY , OTHER> &elem , AVLNode *left = nullptr , AVLNode *right = nullptr , int h = 1):data(elem) , l_child(left) , r_child(right) , height(h) {}
    };

    struct BackTrace
    {

    };
    AVLNode *root;

    void Clear(AVLNode *node);
    void insert(const set<KEY , OTHER> &data , AVLNode *&node);
    void LL(AVLNode *&node);
    void LR(AVLNode *&node);
    void RR(AVLNode *&node);
    void RL(AVLNode *&node);
    void RL_spec(AVLNode *&node);
    void LR_spec(AVLNode *&node);
    void shuffle_height(); // abandoned
    void shuffle_height_mod(stack<AVLNode *> path); // contained the operation of deletion

    static int height(const AVLNode *node){ return (node) ? node-> height : 0 ;}

public:
    AvlTree2():root(nullptr){}
    ~AvlTree2() { Clear() ; }

    void Clear(){Clear(root) ; root = nullptr ;}
    void insert(const set<KEY , OTHER> &data){insert(data , root) ; std::cout<<"set("<<data.key<<" , "<<data.other<<") inserted successfully!\n";}
    void remove(const KEY &key);

};

template<class KEY , class OTHER>
void AvlTree2<KEY , OTHER>::Clear(AVLNode *node) {
    if (!node)// the special condition
        return;
   std::stack<AVLNode *> nodes;
    nodes.push(node);
    while (!nodes.empty()) {
        AVLNode *tmp = nodes.top();
        nodes.pop();
        if (tmp-> r_child)
            nodes.push(tmp-> r_child);
        if (tmp-> l_child)
            nodes.push(tmp-> l_child);
        delete tmp;
    }
}

template<class KEY , class OTHER>
void AvlTree2<KEY , OTHER>::RR(AVLNode *&node) {
    AVLNode *tmp = node-> r_child;
    node-> r_child = tmp-> l_child;
    tmp-> l_child = node;

    node-> height = 1 + (height(node-> l_child) > height(node-> r_child) ? height(node-> l_child) : height(node-> r_child));
    tmp-> height = 1 + (height(tmp-> l_child) > height(tmp-> r_child) ? height(tmp-> l_child) : height(tmp -> r_child));
    node = tmp;
}

template<class KEY , class OTHER>
void AvlTree2<KEY , OTHER>::LL(AVLNode *&node) {
    AVLNode *tmp = node-> l_child;
    node-> l_child = tmp-> r_child;
    tmp-> r_child = node;

    node-> height = 1 + (height(node-> l_child) > height(node-> r_child) ? height(node-> l_child) : height(node-> r_child));
    tmp->height = 1 + (height(tmp-> l_child) > height(tmp-> r_child) ? height(tmp-> l_child) : height(tmp-> r_child));
    node = tmp;
}

template<class KEY , class OTHER>
void AvlTree2<KEY , OTHER>::RL(AVLNode *&node) {
    LL(node-> r_child);
    RR(node);
}
template<class KEY , class OTHER>
void AvlTree2<KEY , OTHER>::LR(AVLNode *&node) {
    RR(node-> l_child);
    LL(node);
}
template<class KEY , class OTHER>
void AvlTree2<KEY , OTHER>::RL_spec(AVLNode *&node) {
    AVLNode *target = node-> r_child-> l_child;
    if (height(target-> l_child) < height(target-> r_child)) {
        AVLNode *tmp = target-> l_child;
        target-> l_child = target-> r_child;
        target-> r_child = tmp;
    }
    RL(node);
}

template<class KEY , class OTHER>
void AvlTree2<KEY , OTHER>::LR_spec(AVLNode *&node) {
    AVLNode *target = node-> l_child-> r_child;
    if (height(target-> r_child) < height(target-> l_child)) {
        AVLNode *tmp = target-> r_child;
        target-> r_child = target->  l_child;
        target-> l_child = tmp;
    }
    LL(node);
}

template<class KEY , class OTHER>
void AvlTree2<KEY , OTHER>::shuffle_height() {
    if (!root) // check empty
        return;
    std::stack<std::pair<AVLNode* , int>> nodes; //Post traverse to proofread all heights :)
    nodes.emplace(root , 0);

    while (!nodes.empty()) {
        if (auto *tmp = &nodes.top() ; 1 == tmp-> second) {
            tmp-> first-> height = 1 + (height(tmp-> first-> l_child) > height(tmp-> first-> r_child) ? height(tmp-> first-> l_child) : height(tmp-> first-> r_child));
            nodes.pop();
        }
        else {
            ++tmp-> second;
            if (tmp-> first-> r_child)
                nodes.emplace(tmp-> first-> r_child , 0);
            if (tmp-> first-> l_child)
                nodes.emplace(tmp-> first-> l_child , 0);
        }
    }
}



template<class KEY , class OTHER>
void AvlTree2<KEY , OTHER>::insert(const set<KEY , OTHER> &data , AVLNode *& node) {
    if (!node)
        node = new AVLNode (data);
    else if (data.key > node-> data.key){
        insert(data , node-> r_child);
        if (2 == height(node-> r_child) - height(node-> l_child)) {
            if (data.key > node-> r_child-> data.key)
                RR(node);
            else
                RL(node);
        }
    }
    else if (data.key < node-> data.key) {
        insert(data , node-> l_child);
        if (2 == height(node-> l_child) - height(node-> r_child)) {
            if (data.key < node-> l_child->data.key)
                LL(node);
            else
                LR(node);
        }
    }
    node-> height = (height(node-> l_child) > height(node-> r_child) ? height(node-> l_child) : height(node-> r_child)) + 1;
}


template<class KEY , class OTHER>
void AvlTree2<KEY , OTHER>::remove(const KEY &key) {
    AVLNode **cur = &root;
    AVLNode *tmp;
    KEY now  = key;
    stack<AVLNode *> path;

    while (*cur) {
        if (now == (*cur)-> data.key)
        {
            if (!(*cur)-> l_child || !(*cur)-> r_child) {
                break;
            }
            tmp = (*cur)-> r_child;
            while (tmp-> l_child)
                tmp = tmp-> l_child;
            (*cur)-> data = tmp-> data;
            now = tmp-> data.key;
        }

        if (now < (*cur)-> data.key) {
            if (height((*cur)-> l_child) == height((*cur)-> r_child)) // if heights equal, the backtrace link breaks.
                while (!path.empty())
                    path.pop();
            else if (height((*cur)-> l_child) < height((*cur)-> r_child) && (*cur)-> l_child) {
                tmp = (*cur)-> r_child;
                if (height(tmp-> r_child) >= height(tmp-> l_child))
                    RR(*cur);
                else
                    RL_spec(*cur);
            }
            path.push(*cur);// append links of back-propagation
            cur = &(*cur)-> l_child;
        }
        else {
            if (height((*cur)-> r_child) == height((*cur)-> l_child))
                while (!path.empty())
                    path.pop();
            else if (height((*cur)-> r_child) < height((*cur)-> l_child) && (*cur)-> r_child) {
                tmp = (*cur)-> l_child;
                if (height(tmp-> l_child) >= height(tmp-> r_child))
                    LL(*cur);
                else
                    LR_spec(*cur);
            }
            path.push(*cur);// append links of back-propagation
            cur = &(*cur)-> r_child;
        }
    }

    if (!*cur) {
        std::cout<<key<<" 404 NOT FOUND!\n";
        return;
    }
    tmp =  *cur;
    *cur = (*cur)-> l_child ? (*cur)-> l_child : (*cur)-> r_child;
    if (*cur)
        path.push(*cur); // the last node
    delete tmp;

    shuffle_height_mod(path); // adjust the change of the height due to delete operation
    std::cout<<key<<" removed successfully!\n";
}

template<class KEY , class OTHER>
void AvlTree2<KEY , OTHER>::shuffle_height_mod(stack<AVLNode *>path){ //contained operations of deletion
    AVLNode *tmp;
    while (!path.empty())
    {
        tmp = path.top();
        path.pop();
        tmp-> height = 1 + (height(tmp-> l_child) > height(tmp-> r_child) ? height(tmp-> l_child) : height(tmp-> r_child));
    }
}

#endif //AVLTREE_AVL2_H
