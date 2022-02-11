#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// ------------CLASS NODE-------------------------
class Node{
    /*vector<int> keys;*/
    int min,median,max;
    vector<Node*> Ch;
    bool leaf; // Is true when node is leaf, else false

public:
    vector<int> keys;
    Node(int t, bool leaf);
    void traverse(int tabs);

    //INSERTION
    void Insert(int key, int* val, Node*& newEntry);
    void split(int* val, Node*& newEntry);
    Node* makeNewRoot(int val, Node* newEntry);

    //DELETION
	int Searchindex(int x);
	void Delete(int x);
	void DeleteFromLeaf(int index);
	void DeleteFromNonLeaf(int index);
	int FindPred(int index);
	int FindSucc(int index);
	void ManageUF(int index); //Manage UnderFlow
	void borrowFromPrev(int index);
	void borrowFromNext(int index);
	void merge(int index);

    bool Find_Key_Helper(int key);
	friend class BTree;
};

// ------------CLASS BTREE------------------------

class BTree{
    int t; //Order
public:
    Node* root;
    BTree(int t);
    void Insert(int key);
    void Print();
	void Delete(int x);
    // Ex1.(ii)
    int BTree_FindMin(Node* pointer);
    int BTree_FindMax(Node* pointer);
    // Ex1.(iv)
    void BTree_Find_Successor(int key);
    void BTree_Find_Predecessor(int key);
    //Helper functions
    void BTree_Find_Predecessor_Helper(int key, Node *node, int *pre);
    void BTree_Find_Successor_Helper(int key, Node *node, int *suc);
    bool Find_Key(int key);
};

// ------------------------CLASS DEF-------------------------------

Node::Node(int t, bool leaf){
    this->max = t-1;
    this->min = ceil((float)t/2)-1;
    this->median = ceil((float)max/2);
    this->leaf = leaf;
}


void Node::traverse(int tabs){
    int i;
    string s;

    for (int j = 0; j < tabs; j++)
        s += '\t';

    for (i = 0; i < keys.size(); i++){
  		if (leaf == false)
	        Ch[i]->traverse(tabs + 1);

      	cout << s << keys[i] << endl;
    }

    if (leaf == false) //Last child
        Ch[i]->traverse(tabs + 1);
}


void Node::split(int* val, Node*& newEntry){
    newEntry = new Node(max+1, false);
    *val = this->keys[median]; //median becomes parent

    // Last half entries will go to new node
    for (int i = median + 1; i < max+1; i++)
        newEntry->keys.push_back(this->keys[i]);

    this->keys.resize(median);

    // Last half entries will go to new node
    for (int i = median + 1; i <= max+1; i++)
        newEntry->Ch.push_back(this->Ch[i]);

    this->Ch.resize(median + 1);
}


void Node::Insert(int new_key, int* val, Node*& newEntry){
    if (leaf == false){ //NON-LEAF
        int i = 0;

        while (i < keys.size() && new_key > keys[i])
            i++;

        Ch[i]->Insert(new_key, val, newEntry); //Inserting in left child

        if (newEntry == NULL) //To end the recursion
            return;

        if (keys.size() < max){

            keys.insert(keys.begin() + i, *val);
            Ch.insert(Ch.begin() + i + 1, newEntry);
            newEntry = NULL;
        }

        else{ //node is full >> split
            keys.insert(keys.begin() + i, *val);
            Ch.insert(Ch.begin() + i + 1, newEntry);
            split(val, newEntry);
        }
    }

    else{ //LEAF
		//cout<<"\n"<<max<<"\t"<<min<<"\t"<<median;
        vector<int>::iterator it;

        it = lower_bound(keys.begin(), keys.end(), new_key);
        keys.insert(it, new_key);

        if (keys.size() == max+1){
            newEntry = new Node(max+1, true);
            *val = this->keys[median];

            for (int i = median + 1; i < max+1; i++)
                newEntry->keys.push_back(this->keys[i]);

            this->keys.resize(median);
        }
    }
}


Node* Node::makeNewRoot(int val, Node* newEntry){
    Node* root = new Node(max+1, false);

    root->keys.push_back(val);
    root->Ch.push_back(this);
    root->Ch.push_back(newEntry);

    return root;
}


BTree::BTree(int t){
    root = new Node(t, true);
}


void BTree::Insert(int key){
    Node* newEntry = NULL;
    int val = 0;
    root->Insert(key, &val, newEntry);

    if (newEntry != NULL)
        root = root->makeNewRoot(val, newEntry);
}


void BTree::Print(){
    root->traverse(0);
}


int Node::Searchindex(int x){
	int index=0;

  //cout<<keys.size()<<"\n";
  // while (index<keys.size())
  //{cout<<"\n>>>"<<keys[index]; index++;}

	while (index<keys.size() && keys[index] < x){
		++index;
    }
	return index;
}


void Node::Delete(int x){
	int index = Searchindex(x);
	//cout<<"ff"<<index;

	if (index < keys.size() && keys[index] == x){
		//cout<<"eeeee";
		if (leaf)
			DeleteFromLeaf(index);
		else
			DeleteFromNonLeaf(index);
	}
	else{
		if (leaf){
			cout << "The key "<< x <<" is does not exist in the tree\n";
			return;
		}

		// Flag indicates if the key is present in the sub-tree rooted with last child of this node
		bool flag = ( (index==keys.size())? true : false );

	    //  cout<<"dd";
		if (Ch[index]->keys.size() < min)
			ManageUF(index);

		if (flag && index > keys.size())
			Ch[index-1]->Delete(x);
		else{
			Ch[index]->Delete(x);
			if (Ch[index]->keys.size() < min)
				ManageUF(index);
		}
	}
	return;
}


void Node::DeleteFromLeaf (int index){
	//cout<<"eeeee"<<keys.size();
	//for (int i=0; i<keys.size(); ++i)
	//cout<<"\n"<<keys[i]<<"\n";

	for (int i=index+1; i<keys.size(); ++i)
		keys[i-1] = keys[i];

	int n=keys.size();
	
	keys.resize(n-1);

	return;
}


void Node::DeleteFromNonLeaf(int index){

	int x = keys[index];

    if (Ch[index]->keys.size() >= min)
    {
        int predecessor = FindPred(index);
        keys[index] = predecessor;
        Ch[index]->Delete(predecessor);
        if (Ch[index]->keys.size() < min)
				ManageUF(index);
    }

    else if  (Ch[index+1]->keys.size() >= min){
        int successor = FindSucc(index);
        keys[index] = successor;
        Ch[index+1]->Delete(successor);
        if (Ch[index]->keys.size() < min)
				ManageUF(index);
    }else{
        merge(index);
        Ch[index]->Delete(x);
    }
    return;
}


int Node::FindPred(int index){
	//Reach Leaf
	Node *cursor=Ch[index];
	while (!cursor->leaf)
		cursor = cursor->Ch[cursor->keys.size()];

	return cursor->keys[cursor->keys.size()-1];
}


int Node::FindSucc(int index){
	//Reach Leaf
	Node *cursor = Ch[index+1];
	while (!cursor->leaf)
		cursor = cursor->Ch[0];

	return cursor->keys[0];
}


void Node::ManageUF(int index){

	if (index!=0 && Ch[index-1]->keys.size()>min)
		borrowFromPrev(index);

	else if (index!=keys.size() && Ch[index+1]->keys.size()>min)
		borrowFromNext(index);

	else{
		if (index != keys.size())
			merge(index);
		else
			merge(index-1);
	}
	return;
}


void Node::borrowFromPrev(int index){

	Node *child=Ch[index];
	Node *sibling=Ch[index-1];

  	int a=child->keys.size();
	int b=sibling->keys.size();
	child->keys.resize(a+1);


	for (int i=child->keys.size()-2; i>=0; --i)
		child->keys[i+1] = child->keys[i];

	if (!child->leaf){
		for(int i=child->keys.size(); i>=0; --i)
			child->Ch[i+1] = child->Ch[i];
	}

	child->keys[0] = keys[index-1];

	if(!child->leaf)
		child->Ch[0] = sibling->Ch[sibling->keys.size()];

	keys[index-1] = sibling->keys[sibling->keys.size()-1];

	sibling->keys.resize(b-1);
	return;
}


void Node::borrowFromNext(int index){

	Node *child=Ch[index];
	Node *sibling=Ch[index+1];

	int a=child->keys.size();
	int b=sibling->keys.size();
	child->keys.resize(a+1);

	child->keys[(child->keys.size())-1] = keys[index];

	if (!(child->leaf))
		child->Ch[(child->keys.size())] = sibling->Ch[0];

	keys[index] = sibling->keys[0];

	for (int i=1; i<sibling->keys.size(); ++i)
		sibling->keys[i-1] = sibling->keys[i];

	if (!sibling->leaf){
		for(int i=1; i<=sibling->keys.size(); ++i)
			sibling->Ch[i-1] = sibling->Ch[i];
	}

	sibling->keys.resize(b-1);
	return;
}


void Node::merge(int index){
	Node *child = Ch[index];
	Node *sibling = Ch[index+1];

 	int a=child->keys.size();
	int b=sibling->keys.size();
	child->keys.resize(a+b+1);

	child->keys[median-1] = keys[index];

	for (int i=0; i<sibling->keys.size(); ++i)
		child->keys[i+median] = sibling->keys[i];

	if (!child->leaf){
		for(int i=0; i<=sibling->keys.size(); ++i)
			child->Ch[i+median] = sibling->Ch[i];
	}

	for (int i=index+1; i<keys.size(); ++i)
		keys[i-1] = keys[i];

	for (int i=index+2; i<=keys.size(); ++i)
		Ch[i-1] = Ch[i];


	int n=keys.size();
	keys.resize(n-1);

	delete(sibling);
	return;
}


void BTree::Delete(int x){
	if (!root){
		cout << "The Tree is Empty\n";
		return;
	}

	root->Delete(x);

	if (root->keys.size()==0){
		Node *tmp = root;
		if (root->leaf)
			root = NULL;
		else
			root = root->Ch[0];

		delete tmp;
	}
	return;
}
// ---------------------------------CS 225 Assignment 8 Ex1------------------------------

// -----------------------Ex1.(ii)-------------------------

int BTree::BTree_FindMin(Node* pointer){
    if (pointer == NULL){          // T is empty
        return 0;
    }else if (pointer->leaf){       // x is leaf
        return pointer->keys[0];     // return the minimum key of x
    }else{
        return BTree_FindMin(pointer->Ch[0]);
    }
}

 int BTree::BTree_FindMax(Node* pointer){
     if (pointer == NULL){          // pointer is empty
         return 0;
     }else if (pointer->leaf){       // pointer is leaf
         return pointer->keys[pointer->keys.size()-1];     // return the maximum key of x
     }else{
         return BTree_FindMax(pointer->Ch[pointer->Ch.size()-1]);
     }
 }

// ------------------------Ex1.(iv)-------------------------

/*
    // Case 1: successor in Node (index <size-1)
    // Case 2: Nonleaf, successor in Child (index = size-1)
    // Case 3: Leaf, successor in Parents ( the first one larger than key)
 */

void BTree::BTree_Find_Predecessor(int key){
    int infinit = 9999999;
    int pre = infinit;

    BTree_Find_Predecessor_Helper(key, root, &pre);
  
    if (pre == infinit){
        cout << "No predecessor for the key you input is found. Please check your input.\n";
    }else{
        cout << "The predecessor of element " << key << " is "<< pre <<".\n"<<endl;
    }

}

void BTree::BTree_Find_Successor(int key){
    int infinit = 9999999;
    int suc = infinit;

    BTree_Find_Successor_Helper(key, root, &suc);

    if (suc == infinit){
        cout << "No successor for the key you input is found. Please check your input.\n";
    }else{
        cout << "The successor of element " << key << " is "<< suc <<".\n"<<endl;
    }
}

//------------------HELP FUNCTION DEF----------------------------

void BTree::BTree_Find_Predecessor_Helper(int key, Node *node, int *pre){
    if (node == nullptr){
        return;
    }

    int index = node->Searchindex(key);

    if (index > 0){
        *pre = node->keys[index - 1];
    }

    if (key == node->keys[index]){
        if (!(node->leaf)){
            *pre = BTree_FindMax(node->Ch[index]);
        }
        return;
    }
    BTree_Find_Predecessor_Helper(key, node->Ch[index], pre);
}


void BTree::BTree_Find_Successor_Helper(int key, Node *node, int *suc){
    if (node == nullptr){
        return;
    }

    int index = node->Searchindex(key);
    
    if (key == node->keys[index]){
        if (!(node->leaf)){
            *suc = BTree_FindMin(node->Ch[index + 1]);
        }else{
            if (index + 1 < node->keys.size()){
                *suc = node->keys[index + 1];
            }
        }
        return;
    }
    else{
        if (index < node->keys.size()){
            *suc = node->keys[index];
        }
    }
    BTree_Find_Successor_Helper(key, node->Ch[index], suc);
}

bool BTree::Find_Key(int key){
    return this->root->Find_Key_Helper(key);
}

bool Node::Find_Key_Helper(int key){
    if(leaf==true)
    {
        for(int i=0;i<keys.size();i++)
        {
            if(key==keys[i])
            {
                return true;
            }
        }
        return false;
    }
    
    int i;
    
    for ( i = 0; i < keys.size(); i++)
    {
        if(keys[i]==key)
        {
            return true;
        }
        if(keys[i]>key)
        {
            return Ch[i]->Find_Key_Helper(key);
        }
    }
    return Ch[i]->Find_Key_Helper(key);
}

// -----------------------------MAIN PROGRAM---------------------------

int main(){

    
    int choice,Order,e,n,x,min,max;
    cout<<"\nWelcome to the B-tree of CS225 Group 18.\n";
    cout<<"\nPlease enter the Tree Order(>2) you desire : ";
   	cin>>Order;

    BTree* T = new BTree(Order);

   s: cout<<"\n1: INSERT(small tree)\t  2:INSERT first n Numbers(big tree)\t 3:DELETE\t 4:PRINT\t 5:MIN\t 6:MAX\t 7:PREDECESSOR\t 8:SUCCESSOR\t 9:DONE \n";
    cin>>choice;

    switch(choice){
    	case 1:
    		cout<<"Please enter the element you want to insert: ";
    		cin>>e;
    		T->Insert(e);
    		goto s;

    	case 2:
    		cout<<"Please enter the number of elements you want to insert n: ";
    		cin>>n;

    		for(int i=1;i<=n;i++)
    			T->Insert(i);
    		goto s;

    	case 3:
    		cout<<"Please enter the element you want to delete: ";
    		cin>>x;
    		T->Delete(x);
    		goto s;

    	case 4:
            cout<<"A sample view of your current B-tree is shown as following: \n";
    		cout<<"\n\n";
    		T->Print();
    		cout<<"\n";
    		goto s;

        case 5:
            /*cout<<"Notice: Please make sure that the B-tree is not empty now! Otherwise you will see segmentation fault error message due to invalid operation ordered by you!\n\n";*/
            if (T->root->keys.size() == 0){
                cout << "Invalid operation since tree is empty\n";
            }else{
                min = T->BTree_FindMin(T->root);
                cout << "The minimun key in current B-tree is " << min << ".\n"<<endl;
            }
            goto s;

        case 6:
            /*cout<<"Notice: Please make sure that the B-tree is not empty now! Otherwise you will see segmentation fault error message due to invalid operation ordered by you!\n\n";*/
            if (T->root->keys.size() == 0){
                cout << "Invalid operation since tree is empty\n";
            }else{
                max = T->BTree_FindMax(T->root);
                cout << "The maximun key in current B-tree is " << max << ".\n"<<endl;
            }
            goto s;

        case 7:
            cout<<"Please enter the element which you want to find its predecessor: ";
            cin>>e;
            if (T->Find_Key(e)==false){
                cout<<"Invalid input since your input key doesn't exist in B-tree! \n";
            }else{
                /*pre = */T->BTree_Find_Predecessor(e);
                /*cout<<"The predecessor of element "<< e <<" is "<< pre <<".\n"<<endl;*/
            }
            goto s;

        case 8:
            cout<<"Please enter the element which you want to find its successor: ";
            cin>>e;
            if (T->Find_Key(e)==false){
                cout<<"Invalid input since your input key doesn't exist in B-tree! \n";
            }else{
                /*suc = */T->BTree_Find_Successor(e);
                /*cout<<"The successor of element "<< e <<" is "<< suc <<".\n"<<endl;*/
            }
            goto s;

    	case 9:
            cout<<"\nThank you for using the B-tree of CS225 Group 18. You log out successfully! \n";
    		break;

    	default:
    		cout<<"\nNo such case! Please check your input and re-execute! Ending program.\n ";
    		break;
    }

    return 0;
}

/*
 Sample Test Case for Min Max PREDECESSOR and SUCCESSOR.
 xuke@xukedeMacBook-Pro Ex1 % ./Btree

 Welcome to the B-tree of CS225 Group 18.

 Please enter the Tree Order(>2) you desire : 4

 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 2
 Please enter the number of elements you want to insert n: 30

 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 5
 The minimun key in current B-tree is 1.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 6
 The maximun key in current B-tree is 30.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 4
 A sample view of your current B-tree is shown as following:


         1
         2
     3
         4
         5
     6
         7
         8
 9
         10
         11
     12
         13
         14
     15
         16
         17
 18
         19
         20
     21
         22
         23
     24
         25
         26
     27
         28
         29
         30


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 23
 The predecessor of element 23 is 22.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 22
 The predecessor of element 22 is 21.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 21
 The predecessor of element 21 is 20.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 20
 The predecessor of element 20 is 19.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 19
 The predecessor of element 19 is 18.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 18
 The predecessor of element 18 is 17.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 17
 The predecessor of element 17 is 16.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 16
 The predecessor of element 16 is 15.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 8
 Please enter the element which you want to find its successor: 11
 The successor of element 11 is 12.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 8
 Please enter the element which you want to find its successor: 12
 The successor of element 12 is 13.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 8
 Please enter the element which you want to find its successor: 13
 The successor of element 13 is 14.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 8
 Please enter the element which you want to find its successor: 14
 The successor of element 14 is 15.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 8
 Please enter the element which you want to find its successor: 15
 The successor of element 15 is 16.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 8
 Please enter the element which you want to find its successor: 16
 The successor of element 16 is 17.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 8
 Please enter the element which you want to find its successor: 17
 The successor of element 17 is 18.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 8
 Please enter the element which you want to find its successor: 18
 The successor of element 18 is 19.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 8
 Please enter the element which you want to find its successor: 19
 The successor of element 19 is 20.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 9

 Thank you for using the B-tree of CS225 Group 18. You log out successfully!
 
 
 xuke@xukedeMacBook-Pro Ex1 % ./Btree

 Welcome to the B-tree of CS225 Group 18.

 Please enter the Tree Order(>2) you desire : 6

 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 2
 Please enter the number of elements you want to insert n: 100

 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 5
 The minimun key in current B-tree is 1.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 6
 The maximun key in current B-tree is 100.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 2
 The predecessor of element 2 is 1.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 3
 The predecessor of element 3 is 2.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 4
 The predecessor of element 4 is 3.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 5
 The predecessor of element 5 is 4.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 6
 The predecessor of element 6 is 5.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 7
 The predecessor of element 7 is 6.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 8
 The predecessor of element 8 is 7.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 9
 The predecessor of element 9 is 8.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 34
 The predecessor of element 34 is 33.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 45
 The predecessor of element 45 is 44.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 56
 The predecessor of element 56 is 55.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 8
 Please enter the element which you want to find its successor: 76
 The successor of element 76 is 77.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 8
 Please enter the element which you want to find its successor: 66
 The successor of element 66 is 67.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 8
 Please enter the element which you want to find its successor: 45
 The successor of element 45 is 46.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 8
 Please enter the element which you want to find its successor: 34
 The successor of element 34 is 35.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 8
 Please enter the element which you want to find its successor: 12
 The successor of element 12 is 13.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 8
 Please enter the element which you want to find its successor: 1
 The successor of element 1 is 2.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 8
 Please enter the element which you want to find its successor: 880
 Invalid input since your input key doesn't exist in B-tree!

 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 0
 Invalid input since your input key doesn't exist in B-tree!

 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:MIN     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 9

 Thank you for using the B-tree of CS225 Group 18. You log out successfully!
 
 */
