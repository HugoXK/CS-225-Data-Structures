//
//  bst.cpp
//  
//
//  Created by KD on 14.03.21.
//

#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include "median_AVL.h"
using std::cout;
using std::cin;

/* the constructor creates a node with the given value and left and right pointers */

template<class T> node<T>::node(T item, node<T> *left, node<T> *right)
{
    data = item;
    pt_left = left;
    pt_right = right;
    balance = 0;
}

/* the get and set operations are obvious */
template<class T> T node<T>::getdata(void)
{
    return data;
}

template<class T> node<T> *node<T>::getleft(void)
{
    return pt_left;
}

template<class T> node<T> *node<T>::getright(void)
{
    return pt_right;
}

template<class T> void node<T>::setdata(T item)
{
    data = item;
    return;
}

template<class T> void node<T>::setleft(node<T> *pt)
{
    pt_left = pt;
    return;
}

template<class T> void node<T>::setright(node<T> *pt)
{
    pt_right = pt;
    return;
}

/* the constructor contains a trivial BST containing just a null pointer at the root */
template<class T> BST<T>::BST(void)
{
    root = 0;
}

/* the insert, delete and find functions are realised recursively starting from the root using the corresponding auxiliary functions */
template<class T> void BST<T>::insert(T item)
{
    root = _insert(root, item);
    median_op(root);
    return;
}

template<class T> void BST<T>::remove(T item)
{
    root = _delete(root, item);
    median_op(root);
    return;
}

template<class T> bool BST<T>::find(T item)
{
    return _find(root, item);
}

template<class T> node<T> *BST<T>::_insert(node<T> *pt, T val)                   //This function can get the subtree rooted at the pt done.
{
    if (pt == 0)
    /*  First check, if the root of the tree (given by the pointer pt) into which the value val is to be inserted is empty.
        In this case create and return a new node with the given value val and nullpointers for left and right successors. */
    {
        node<T> *newnode = new node<T>;                 //Since the initial value set for left or right pointer are just null pointers.
        (*newnode).setdata(val);
        return newnode;
    }
    /* Otherwise, we either have to insert into the left or right successor tree. In both cases the returned pointer pt_new will become the new value of the left or right successor pointer, resp. */
    if (val < (*pt).getdata())
    {
        node<T> *pt_new;
        pt_new = _insert((*pt).getleft(), val);
        pt->balance--;
        (*pt).setleft(pt_new);
    }
    else if (val > (*pt).getdata())
    {
        node<T> *pt_new;
        pt_new = _insert((*pt).getright(), val);
        pt->balance++;
        (*pt).setright(pt_new);
    }
    return pt;
}

template<class T> node<T> *BST<T>::_delete(node<T> *pt, T val)                   //This function delete the item if the item is rooted at pt.
{
    /* _delete searches recursively for the node with the given value val. If a nullpointer is reached, val does not appear in the BST, so no change is made.  */
    if (pt==0){
        return pt;
    }
    if (pt->getdata() == val){
        if ((*pt).getleft() == 0)
        {
            return (*pt).getright();
        }
        /* in case there is no right successor tree, the result of the delete is the left successor tree */
        if ((*pt).getright() == 0)
        {
            return (*pt).getleft();
        }
        node<T>* newnode;
        newnode = findswapleft(pt, (*pt).getleft());
        pt->balance++;
        (*pt).setleft(newnode);
    }
    else if (pt->getdata() < val){
        node<T>* pt_new;
        pt_new=_delete(pt->getright(),val);
        pt->balance--;
        pt->setright(pt_new);
    }
    else{
        node<T>* pt_new;
        pt_new=_delete(pt->getleft(),val);
        pt->balance++;
        pt->setleft(pt_new);
    }
    return pt;
}

/* _find works in the same way as _delete, but only a truth value is returned. */
template<class T> bool BST<T>::_find(node<T> *pt, T val)              //This function can return whether the subtree has item.
{
    if (pt == 0)
        return false;
    if ((*pt).getdata() == val)
        return true;
    if (val < (*pt).getdata())
        return _find((*pt).getleft(), val);
    return _find((*pt).getright(), val);
}

template<class T> node<T> *BST<T>::merge(node<T> *pt1, node<T> *pt2)
{
    /* if there is no right successor tree, the left successor tree is automatically the result of the merge */
    if (pt2 == 0)
        return pt1;
    /* Otherwise, if the right successor tree has no left successor, make the given tree (pointed to by pt1) the new left successor instead of null. */
    if ((*pt2).getleft() == 0)
    {
        (*pt2).setleft(pt1);
        return pt2;
    }
    /* If the right successor (given by pointer pt2) has a non-empty left successor tree, insert the given tree (pointed to by pt1) into this left successor. */
    (*pt2).setleft(merge(pt1, (*pt2).getleft()));
    return pt2;
}

template<class T> void BST<T>::display(void)
{
    cout << "\nAVL Tree\n\n";
    _display(root);
    cout << "\n\n";
}

/* Iterate through the tree in order of the order < on T, i.e. first the left subtree, then the root, then the right subtree. */
template<class T> void BST<T>::_display(node<T> * pt)
{
    if (pt != 0)
    {
        _display((*pt).getleft());
        cout << "Node:\n";
        cout << "Value: " << (*pt).getdata() << "\n";
        cout << "Balance: " << (*pt).balance << "\n";
        node<T> *pt_l;
        pt_l = (*pt).getleft();
        if (pt_l != 0)
            cout << "Left: " << pt_l << " with value " << (*pt_l).getdata() << "\n";
        else cout << "Left: Null\n";
        node<T> *pt_r;
        pt_r = (*pt).getright();
        if (pt_r != 0)
            cout << "Right: " << pt_r << " with value " << (*pt_r).getdata() << "\n";
        else cout << "Right: Null\n";
        cout << "----\n";
        _display((*pt).getright());
    }
    return;
}

template<class T> node<T>* BST<T>::findswapleft(node<T> *pt_swap, node<T> *here)
{
    if ((*here).getright() == 0)
    {
        (*pt_swap).setdata((*here).getdata());
        return (*here).getleft();
    }
    node<T> *pt_new;
    pt_new = findswapleft(pt_swap, (*here).getright());
    here->balance--;
    (*here).setright(pt_new);
    return here;
}

template<class T> node<T>* BST<T>::findswapright(node<T> *pt_swap, node<T> *here)
{
    if ((*here).getleft() == 0)
    {
        (*pt_swap).setdata((*here).getdata());
        return (*here).getright();
    }
    node<T> *pt_new;
    pt_new = findswapleft(pt_swap, (*here).getleft());
    here->balance++;
    (*here).setleft(pt_new);
    return here;
}

template<class T> void BST<T>::median_op(node<T>* shit)
{
    if (shit!=0){
        if (shit->balance==-2 || shit->balance==-3){
            node<T>* pt_new;
            T item = shit->getdata();
            pt_new = findswapleft(shit,shit->pt_left);
            shit->balance++;
            shit->setleft(pt_new);
            _insert(shit,item);
        }
        if (shit->balance==1 || shit->balance==2){
            node<T>* pt_new;
            T item = shit->getdata();
            pt_new = findswapright(shit,shit->pt_right);
            shit->balance--;
            shit->setright(pt_new);
            _insert(shit,item);
        }
        median_op(shit->pt_left);
        median_op(shit->pt_right);
    }
}