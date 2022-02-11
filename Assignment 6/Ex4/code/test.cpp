#include "median_AVL.cpp"
using std::cout;
int main()
{
    BST<int> tree=BST<int>();
    int i;
    cout << "Here is the test.\n We first build a large AVL tree with median property, and then delete several nodes to see what happens.\n";
    tree.insert(7);
    tree.insert(5);
    tree.insert(17);
    tree.insert(4);
    tree.insert(100);
    tree.insert(28);
    tree.insert(6);
    tree.insert(10);
    tree.insert(1);
    for (i=19;i<=25;i++){
        tree.insert(i);
    }
    tree.remove(22);
    tree.remove(28);
    tree.remove(6);
    tree.display();
    getchar();
    cout << "Feel free to use your own data to test my code. But be careful about the segmentation fault";
}