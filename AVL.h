//
// Created by 85375 on 2025/12/6.
//

#ifndef AVLTREE_AVL_H
#define AVLTREE_AVL_H
#include <iostream>
using namespace std;
template <class KEY , class OTHER>

struct set {
    KEY key;
    OTHER other;
};
template <class KEY , class OTHER>
class AvlTree {
private:
    struct AvlNode {
        set<KEY , OTHER> data;
        AvlNode *l_child;
        AvlNode *r_child;
        int height;

        explicit AvlNode(const set<KEY , OTHER> &element , AvlNode *lc = nullptr , AvlNode *rc = nullptr , int h = 1):data(element) , l_child(lc) , r_child(rc) , height(h){};
    };

    AvlNode *root;
public:
    AvlTree(AvlNode* n = nullptr):root(n){};
    ~AvlTree(){Clear();}
    void Clear(){Clear(root) ; root = nullptr;}
    set<KEY , OTHER>* find(const KEY &x) const;
    void insert(const set<KEY , OTHER> &x){insert(x , root);};
    void remove(const KEY &x){remove(x , root);};
private:
    enum subtree {
        Left,
        Right,
    };

    void insert(const set<KEY , OTHER> &x , AvlNode *&node );
    bool remove(const KEY &x , AvlNode *&node);
    void Clear(AvlNode *node);
    void LL(AvlNode *&node);
    void RR(AvlNode *&node);
    void RL(AvlNode *&node);
    void LR(AvlNode *&node);
    bool adjust(AvlNode *&node , subtree sub);

    static int height(const AvlNode *node) {return (nullptr == node) ?  0 : node-> height;}//工具函数
    static int max(int a , int b){return (a > b) ? a : b;}
};

template <class KEY , class OTHER>
void AvlTree<KEY , OTHER>::Clear(AvlNode* node) {
    if (node) {
        Clear(node-> l_child);
        Clear(node-> r_child);
        delete node;
    }
}

template <class KEY , class OTHER>
set<KEY , OTHER>* AvlTree<KEY , OTHER>::find(const KEY &x) const {
    AvlNode* tmp = root;
    while (tmp) {
        if (x == tmp-> data.key)
            return &tmp-> data;
        if (x > tmp-> data.key)
            tmp = tmp-> r_child;
        else
            tmp = tmp-> l_child;
    }
    return nullptr;
}

template <class KEY , class OTHER>
void AvlTree<KEY , OTHER>::insert(const set<KEY, OTHER> &x , AvlNode *&node) {//这边引用是因为重新平衡时根节点的父节点要重新连接
    if (!node)
        node = new AvlNode (x);
    else if (x.key > node-> data.key){
        insert(x , node-> r_child);
        if (2 == height(node-> r_child) - height(node-> l_child)) {
            if (x.key > node-> r_child-> data.key)
                RR(node);
            else
                RL(node);
        }
    }
    else if (x.key < node-> data.key) {
        insert(x , node-> l_child);
        if (2 == height(node-> l_child) - height(node-> r_child)) {
            if (x.key < node-> l_child->data.key)
                LL(node);
            else
                LR(node);
        }
    }
    node-> height = max(height(node-> l_child) , height(node-> r_child)) + 1;
}

template <class KEY , class OTHER>
void AvlTree<KEY , OTHER>::LL(AvlNode *&node) {//注意高度更新！！
    AvlNode *tmp = node-> l_child;
    node-> l_child = tmp-> r_child;
    tmp-> r_child = node;
    node-> height = max(height(node-> l_child) , height(node-> r_child)) + 1;
    tmp-> height = max(height(tmp-> l_child) , height(tmp-> r_child)) + 1;
    node = tmp;
}
template <class KEY , class OTHER>
void AvlTree<KEY , OTHER>::RR(AvlNode *&node) {//高度更新
    AvlNode *tmp = node-> r_child;
    node-> r_child = tmp-> l_child;
    tmp-> l_child = node;
    node->height = max(height(node-> l_child) , height(node-> r_child)) + 1;
    tmp-> height = max(height(tmp-> l_child) , height(tmp-> r_child)) + 1;
    node = tmp;
}
template <class KEY , class OTHER>
void AvlTree<KEY , OTHER>::LR(AvlNode *&node) {
    RR(node-> l_child);
    LL(node);
}
template <class KEY , class OTHER>
void AvlTree<KEY , OTHER>::RL(AvlNode *&node) {
    LL(node-> r_child);
    RR(node);
}

template <class KEY , class OTHER>
bool AvlTree<KEY , OTHER>::remove(const KEY &x , AvlNode *&node) {
    if (!node) {
        return true;
    }
    if (x == node-> data.key) {
       if (!node-> l_child || !node-> r_child) {
           AvlNode *tmp = node;
           node = (node-> l_child) ? node-> l_child : node->r_child;
           delete tmp;
           return false;
       }
        else {//好看起见，（照着教材上的）还是写上吧
             AvlNode *tmp = node->r_child;
            while (tmp-> l_child) {
                tmp = tmp-> l_child;
            }
            node-> data = tmp-> data;
            if (remove(tmp-> data.key , node->r_child))
                return true;
            return adjust(node , Right);
        }
    }
    if (x < node-> data.key) {
        if (remove(x , node-> l_child))
            return true;
        return adjust(node , Left);
    }
    if (x > node-> data.key) {
        if (remove(x , node-> r_child))
            return true;
        return adjust(node , Right);
    }
    return true;
}
template <class KEY ,class OTHER>
bool AvlTree<KEY , OTHER>::adjust(AvlNode *&node , subtree sub) {
    if (Left == sub) {
        if (height(node-> l_child) == height(node-> r_child)) {
            node-> height++;//更新高度
            return false;
        }
        if (height(node-> l_child) == height(node-> r_child) - 1)
            return true;
        if (height(node-> l_child) == height(node-> r_child) - 2) {
            if (height(node-> r_child->l_child) == height(node-> r_child ->r_child)) {
                RR(node);
                return true;
            }
            if (height(node-> r_child-> l_child) == height(node-> r_child-> r_child) - 1) {
                RR(node);
                return false;
            }
            if (height(node-> r_child-> l_child) == height(node-> r_child-> r_child) + 1) {
                RL(node);
                return false;
            }
            cout<<"莉莉是笨蛋，程序弄错啦……"<<endl;
            throw;
        }
    }
    if(Right == sub) {
        if (height(node-> r_child) == height(node-> l_child)) {
            --node-> height;
            return false;
        }
        if (height(node-> r_child) == height(node-> l_child) - 1)
            return true;
        if (height(node-> r_child) == height(node-> l_child) - 2) {
            if (height(node-> l_child-> r_child) == height(node-> l_child-> l_child)) {
                LL(node);
                return true;
            }
            if (height(node-> l_child-> r_child) == height(node-> l_child-> l_child) - 1) {
                LL(node);
                return false;
            }
            if (height(node-> l_child-> r_child) == height(node-> l_child-> l_child) + 1) {
                LR(node);
                return false;
            }
            cout<<"莉莉是笨蛋，程序弄错啦……"<<endl;
            throw;
        }
    }
    throw;
}








#endif //AVLTREE_AVL_H
//questions
//标准库会用递归调用吗；是不是所有能递归的问题全部能转化
//内联函数的性能问题（
//前缀运算符和后缀？--node-> height ; node-> height --   line 170
//static成员工具函数存在的问题

//关于Avl树和RB树插入时新节点与父节点的链接区别（已解决