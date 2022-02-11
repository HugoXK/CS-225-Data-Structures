//
//  bst.h
//  
//
//  Created by KD on 14.03.21.
//

#ifndef median_AVL_h
#define mdian_AVL_h

template<class T> class BST;

template<class T> class node
{
    friend class BST<T>;
public:
    node(T item = 0, node<T> *left = 0, node<T> *right = 0);
//    virtual ~node(void);
    T getdata(void);
    node<T> *getleft(void);
    node<T> *getright(void);
    void setdata(T item);
    void setleft(node<T> *pt);
    void setright(node<T> *pt);
private:                                                                //Always remember that the node has three data: the content, the left and right pointer.
    T data;
    node<T> *pt_left;
    node<T> *pt_right;
    int balance;
};

template<class T> class BST
{
public:
    BST(void);
//    virtual ~BST(void);
    void insert(T item);
    void remove(T item);
    bool find(T item);
    void display(void);
private:
    node<T> *root;        //Note that the root is not the node itself, but the pointer to the node. What's the most interesting is it only contains a root.
    /* _insert, _delete and _find are auxiliary functions to navigate recursively through the BST for insert, delete and find */
    node<T> *_insert(node<T> *pt, T val);
    node<T> *_delete(node<T> *pt, T val);
    bool _find(node<T> *pt, T val);
    /* merge is an auxiliary function used, when a non-leaf node is deleted */
    node<T> *merge(node<T> *pt1, node<T> *pt2);
    /* merge iterates through the BST in depth-first way to display the values in all nodes; it can be extended to also display all pointers */
    void _display(node<T> * pt);
    node<T>* findswapleft(node<T> *pt_swap, node<T> *here);
    node<T>* findswapright(node<T> *pt_swap, node<T> *here);
    void median_op(node<T>* shit);
};

#endif /* bst_h */
