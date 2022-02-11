#include "avl.cpp"
#include "fifo.cpp"

// CS 225 Assignment 7  # Group 18: Xu Ke, Wang Dingkun, Qian Shuaicun, Hua Bingshen

// your implementation

/*
*   Ex.1 (i)
*   return value:   a pointer to a new list containing the selected elements
*
*   notes:          typically, there are three variants of DFS in trees:
*                                       inorder, postorder and preorder.
*                   choose whichever you like the most.
*/
template<class T> AList<T>* AVL<T>::DFS(void){
    AList<T>* storage = new AList<T>(100);
    DFS_Helper(root,storage);
    return storage;
};

template<class T> void AVL<T>::DFS_Helper(avlnode<T>* avl_node, AList<T>* storage){
    if ( avl_node != nullptr){
        DFS_Helper(avl_node->getleft(),storage);
        storage->append(avl_node->getdata());
        DFS_Helper(avl_node->getright(),storage);
    }else{
        return;
    }
}

/*
*   Ex.1 (ii)
*   return value:   a pointer to a new list containing the selected elements
*
*   notes:          BFS is usually implemented with a FIFO queue, you may
*                   want to make use of that. Typically, when pushing the
*                   children of a node, there are two orders:
*                                   left child first or right child first.
*                   choose whichever you like the most.
*/
template<class T> AList<T>* AVL<T>::BFS(void){
    AList<T>* storage = new AList<T>(100);
    int current_height = 1;
    int max_height = BFS_getheight(root);
    while(current_height <= max_height){
        BFS_find(root, storage, current_height);
        current_height ++;
    }
    return storage;

};

template<class T> void AVL<T>::BFS_find(avlnode<T>* avl_node, AList<T>* storage, int relative_height){
    if ( avl_node!= nullptr){
        if ( relative_height > 1 ){
            BFS_find( avl_node->getleft(), storage, relative_height - 1);
            BFS_find( avl_node->getright() , storage, relative_height - 1);
        }else if ( relative_height == 1 ){
            storage->append(avl_node->getdata());
        }else if ( relative_height < 1 ){
            cout << "Error Warning: Invalid input for BFS_find.";
        }
    }else{
        return;
    }
}

template<class T> int AVL<T>::BFS_getheight(avlnode<T>* avl_node){
    if (avl_node != nullptr){
        int left = BFS_getheight(avl_node->getleft());
        int right = BFS_getheight(avl_node->getright());
            return max( left+1, right+1 );
    }else{
        return 0;
    }
}

template<class T> int AVL<T>::max(int a, int b){
    if ( a>b ){
        return a;
    }else{
        return b;
    }
}

/*
*   Ex.1 (iii)
*   return value:   a pointer to a new list containing the selected elements
*/
template<class T> AList<T>* AVL<T>::range(T x, T y){
    AList<T>* storage = new AList<T>(100);
    T element;
    AList<T>* temp = BFS();
    int length = temp->getlength();
    while( length >=1 ){
        element = temp->getitem(length);
        if ( judge_range( x, element, y)){
            storage->append(element);
        }
        length -=1;
    }
    delete temp;
    return storage;
};

template<class T> int AVL<T>::judge_range(T min, T element, T max){
    if ( ( min <= element) && ( max >= element)){
        return 1;
    }else{
        return 0;
    }
}