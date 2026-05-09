#include "AVL.h"
#include "AVL_modified.h"
int main() {
    set<int , char> sets[30]; // self_defined type 'set'


    AvlTree2<int , char> t2;
    for (int i = 0 ; i < 26 ; ++i) {
        sets[i] = {i , static_cast<char>('z' - i)};
        t2.insert(sets[i]);
    }
    for (int i = 0 ; i < 30 ; ++i)
        t2.remove(i);

    // AvlTree<int , char> t;
    // for (int i = 0 ; i < 5 ; ++i) {
    //     sets[i] = {i , static_cast<char>(i + 'a')};
    //     t.insert(sets[i]);
    // }
    // for (int i = 0 ; i < 5 ; ++i) {
    //     t.remove(i);
    // }
    return 0;
}