/*

NL ----> NON LEAF
L  ----> LEAF

*/


#include <iostream>
#include <vector>
#include<cmath>

using namespace std;
int Lflag=0;

//-----------------------------------------------------------CLASS DECLARATION-----------------------------------------------------------------

class Node
{
   
    int Lmin,Lmedian,Lmax;
    int NLmin,NLmedian,NLmax;

    vector<Node*> Ch;
    bool leaf; // Is true when node is leaf, else false

public:
    vector<int> keys;
    Node(int ti, int tl, bool leaf);
    void traverse(int tabs);

    //INSERTION
    void insert(int key, int* val, Node*& newEntry);
    void split(int* val, Node*& newEntry);
    Node* makeNewRoot(int val, Node* newEntry);

    //DELETION
	int Searchindex(int x);
	int LSearchindex(int x); //First to search in leaf

	void Delete(int x);
	void DeleteFromLeaf(int index);
	void DeleteFromNonLeaf(int index);

	int FindPred(int index);
	int FindSucc(int index);

	void ManageLUF(int index); //Manage Leaf UnderFlow
	void ManageNLUF(int index); //Manage Non-Leaf UnderFlow

	void borrowFromPrev(int index);
	void borrowFromNext(int index);

	void merge(int index);
    
    //Helper functions
    /*void*/ int BpTree_Find_Predecessor_Helper(int key/*, Node *node, int *pre*/);
    /*void*/ int BpTree_Find_Successor_Helper(int key/*, Node *node, int *suc*/);
    bool Find_Key_Helper(int key);

	friend class BpTree;
};

//-------------------------------------

class BpTree
{
    int t; //Order
public:
    Node* root;
    BpTree(int ti, int tl);
    void insert(int key);
    void print();
	void Delete(int x);
    
    // Ex2.(ii)
    int BpTree_FindMin(Node* pointer);
    int BpTree_FindMax(Node* pointer);
    // Ex2.(iv)
    /*void*/ int BpTree_Find_Successor(int key);
    /*void*/ int BpTree_Find_Predecessor(int key);
    Node *getRoot();
    bool Find_Key(int key);
    /*   BpTree();*/
};

//-----------------------------------------------------------CLASS DECLARATION-----------------------------------------------------------------

//-----------------------------------------------------------CLASS DEF-------------------------------------------------------------------------

Node::Node(int ti, int tl, bool leaf)
{
    this->NLmax = ti-1;
    this->NLmin = ceil((float)ti/2)-1;
    this->NLmedian = ceil((float)(NLmax+1)/2) - 1;

    this->Lmax = tl-1;
    this->Lmin = ceil((float)tl/2)-1;
    this->Lmedian = ceil((float)Lmax/2);

    this->leaf = leaf;
}

//-------------------------------------

void Node::traverse(int tabs)
{
    int i;
    string s;

    for (int j = 0; j < tabs; j++)
        s += '\t';

    for (i = 0; i < keys.size(); i++)
    {
  		if (leaf == false)
	        Ch[i]->traverse(tabs + 1);

      	cout << s << keys[i] << endl;
    }

    if (leaf == false) //Last child
        Ch[i]->traverse(tabs + 1);
}

//-------------------------------------

void Node::split(int* val, Node*& newEntry)
{
    newEntry = new Node(NLmax+1, Lmax+1, false);
    *val = this->keys[NLmedian]; //median becomes parent

    // Last half entries will go to new node
    for (int i = NLmedian+1; i < NLmax+1; i++)
        newEntry->keys.push_back(this->keys[i]);

    this->keys.resize(NLmedian);

    // Last half entries will go to new node
    for (int i = NLmedian+1; i <= NLmax+1; i++)
        newEntry->Ch.push_back(this->Ch[i]);

    this->Ch.resize(NLmedian + 1);
}

//-------------------------------------

void Node::insert(int new_key, int* val, Node*& newEntry)
{
    if (leaf == false) //NON-LEAF
    {
        int i = 0;

        while (i < keys.size() && new_key > keys[i])
            i++;

        Ch[i]->insert(new_key, val, newEntry); //Inserting in left child

        if (newEntry == NULL) //To end the recursion
            return;

        if (keys.size() < NLmax)
        {
        	//cout<<"\n"<<keys.size();
            keys.insert(keys.begin() + i, *val);
            Ch.insert(Ch.begin() + i + 1, newEntry);
            newEntry = NULL;
        }

        else //node is full >> split
        {
            keys.insert(keys.begin() + i, *val);
            Ch.insert(Ch.begin() + i + 1, newEntry);
            split(val, newEntry);
        }
    }

    else //LEAF
    {
		//cout<<"\n"<<Lmax<<"\t"<<Lmin<<"\t"<<Lmedian;
		//cout<<"\n"<<NLmax<<"\t"<<NLmin<<"\t"<<NLmedian;
        vector<int>::iterator it;

        it = lower_bound(keys.begin(), keys.end(), new_key);
        keys.insert(it, new_key);

        if (keys.size() == Lmax+1)
        {
            newEntry = new Node(NLmax+1, Lmax+1 , true);
            *val = this->keys[Lmedian];

            for (int i = Lmedian; i < Lmax+1; i++)
                newEntry->keys.push_back(this->keys[i]);

            this->keys.resize(Lmedian);
        }
    }
}

//-------------------------------------

Node* Node::makeNewRoot(int val, Node* newEntry)
{
    Node* root = new Node(NLmax+1,Lmax+1, false);

    root->keys.push_back(val);
    root->Ch.push_back(this);
    root->Ch.push_back(newEntry);

    return root;
}

//-------------------------------------

BpTree::BpTree(int ti, int tl)
{
    root = new Node(ti, tl, true);
}

//-------------------------------------

void BpTree::insert(int key)
{
    Node* newEntry = NULL;
    int val = 0;
    root->insert(key, &val, newEntry);

    if (newEntry != NULL)
        root = root->makeNewRoot(val, newEntry);
}

//-------------------------------------

void BpTree::print()
{
    root->traverse(0);
}

//-------------------------------------

int Node::LSearchindex(int x)
{
	if (Lflag==1)
	{
		return -1;
	}
	int index=0,temp=0;

	if(leaf)
	{
  		//cout<<keys.size()<<"\n";
  		// while (index<keys.size())
 		//{cout<<"\n>>>"<<keys[index]; index++;}

		while (index<keys.size() && keys[index] <= x)
    	{
			++index;
    	}
    	DeleteFromLeaf(index-1);
    	//cout<<Lflag;
	}

	else
	{
		while (temp<keys.size() && keys[temp] <= x)
    	{
			++temp;
	   	}
		index=Ch[temp]->LSearchindex(x);

		if (Ch[temp]->keys.size() < Lmin && Lflag==1 && index!=0)
		{
			ManageLUF(temp);
		}
	}

	return index;
}

//-------------------------------------

int Node::Searchindex(int x)
{
	int index=0;
  //cout<<keys.size()<<"\n";
  // while (index<keys.size())
  //{cout<<"\n>>>"<<keys[index]; index++;}

	while (index<keys.size() && keys[index] < x)
    {
		++index;
    }
	return index;
}

//-------------------------------------

void Node::Delete(int x)
{
	int index = Searchindex(x);
	//cout<<"ff"<<index;

	if (index < keys.size() && keys[index] == x)
	{
		//cout<<"eeeee";
		if (leaf)
			DeleteFromLeaf(index);
		else
		{
			//cout<<"eeee";
			DeleteFromNonLeaf(index);
		}
	}
	else
	{

		if (leaf && Lflag==1)
			return;

		if (leaf && Lflag==0)
		{
			cout << "The key "<< x <<" is does not exist in the tree\n";
			return;
		}

		// Flag indicates if the key is present in the sub-tree rooted with last child of this node
		bool flag = ( (index==keys.size())? true : false );

	    //cout<<"dd";
	    if(Ch[index]->leaf)
	    {
			if (Ch[index]->keys.size() < Lmin)
			{
				ManageLUF(index);
			}
		}
		else
		{
			if (Ch[index]->keys.size() < NLmin)
			{
				ManageNLUF(index);
			}
		}


		if (flag && index > keys.size())
			Ch[index-1]->Delete(x);
		else
		{
			Ch[index]->Delete(x);

			if(Ch[index]->leaf)
	    	{
				if (Ch[index]->keys.size() < Lmin)
					ManageLUF(index);
			}
			else
			{
				if (Ch[index]->keys.size() < NLmin)
					ManageNLUF(index);
			}
		}
	}
	return;
}

//-------------------------------------

void Node::DeleteFromLeaf (int index)
{
	//cout<<"eeeee"<<keys.size();
	//for (int i=0; i<keys.size(); ++i)
	//cout<<"\n"<<keys[i]<<"\n";
	if(index==-1)
		return;
	Lflag=1;
	for (int i=index+1; i<keys.size(); ++i)
		keys[i-1] = keys[i];

	int n=keys.size();
	keys.resize(n-1);	
	return;
}

//-------------------------------------

void Node::DeleteFromNonLeaf(int index)
{

	int x = keys[index];

	if(Ch[index+1]->leaf)
	{
	    if  (Ch[index+1]->keys.size() >= Lmin)
	    {
	        int successor = FindSucc(index);
	        keys[index] = successor;
	        //Ch[index+1]->Delete(successor);
    	}
    }

    else
    {
		if  (Ch[index+1]->keys.size() >= NLmin)
    	{
    	    int successor = FindSucc(index);
    	    keys[index] = successor;
    	    //Ch[index+1]->Delete(successor);
    	}
    }
    return;
}

//-------------------------------------

int Node::FindPred(int index)
{
	//Reach Leaf
	Node *cursor=Ch[index];
	while (!cursor->leaf)
		cursor = cursor->Ch[cursor->keys.size()];

	return cursor->keys[cursor->keys.size()-1];
}

//-------------------------------------

int Node::FindSucc(int index)
{
	//Reach Leaf
	Node *cursor = Ch[index+1];
	while (!cursor->leaf)
		cursor = cursor->Ch[0];

	return cursor->keys[0];
}

//-------------------------------------

void Node::ManageLUF(int index)
{

	if (index!=0 && Ch[index-1]->keys.size()>Lmin)
		borrowFromPrev(index);

	else if (index!=keys.size() && Ch[index+1]->keys.size()>Lmin)
		borrowFromNext(index);

	else
	{
		if (index != keys.size())
			merge(index);
		else
			merge(index-1);
	}
	return;
}

//-------------------------------------

void Node::ManageNLUF(int index)
{

	if (index!=0 && Ch[index-1]->keys.size()>NLmin)
		borrowFromPrev(index);

	else if (index!=keys.size() && Ch[index+1]->keys.size()>NLmin)
		borrowFromNext(index);

	else
	{
		if (index != keys.size())
			merge(index);
		else
			merge(index-1);
	}
	return;
}

//-------------------------------------

void Node::borrowFromPrev(int index)
{

	Node *child=Ch[index];
	Node *sibling=Ch[index-1];

  	int a=child->keys.size();
	int b=sibling->keys.size();
	child->keys.resize(a+1);


	for (int i=child->keys.size()-2; i>=0; --i)
		child->keys[i+1] = child->keys[i];

	if (!child->leaf)
	{
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

//-------------------------------------

void Node::borrowFromNext(int index)
{

	Node *child=Ch[index];
	Node *sibling=Ch[index+1];

	int a=child->keys.size();
	int b=sibling->keys.size();
	child->keys.resize(a+1);

	child->keys[(child->keys.size())-1] = keys[index];

	if (!(child->leaf))
		child->Ch[(child->keys.size())] = sibling->Ch[0];

	for (int i=1; i<sibling->keys.size(); ++i)
		sibling->keys[i-1] = sibling->keys[i];

	keys[index] = sibling->keys[0];

	if (!sibling->leaf)
	{
		for(int i=1; i<=sibling->keys.size(); ++i)
			sibling->Ch[i-1] = sibling->Ch[i];
	}

	sibling->keys.resize(b-1);
	return;
}

//-------------------------------------

void Node::merge(int index)
{
	Node *child = Ch[index];
	Node *sibling = Ch[index+1];

 	int a=child->keys.size();
	int b=sibling->keys.size();
	


	if (!child->leaf)
	{
		child->keys.resize(a+b+1);
		child->keys[a] = keys[index];

		for (int i=0; i<sibling->keys.size(); ++i)
		child->keys[i+a+1] = sibling->keys[i];

		for(int i=0; i<=sibling->keys.size(); ++i)
			child->Ch[i+a+1] = sibling->Ch[i];
	}

	else
	{
		child->keys.resize(a+b);

		for (int i=0; i<sibling->keys.size(); ++i)
		child->keys[i+a] = sibling->keys[i];
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

//-------------------------------------

void BpTree::Delete(int x)
{
	if (!root)
	{
		cout << "The Tree is Empty\n";
		return;
	}

	root->LSearchindex(x);
	//root->traverse(0);
	root->Delete(x);

	if (root->keys.size()==0)
	{
		Node *tmp = root;
		if (root->leaf)
			root = NULL;
		else
			root = root->Ch[0];

		delete tmp;
	}
	return;
}

//-----------------------------------------------------CS 225 Assignment 8 Ex2----------------------------------------------------------------

// -----------------------Ex2.(ii)-------------------------


int BpTree::BpTree_FindMin(Node* pointer){
    if (pointer == NULL){          // T is empty
        return 0;
    }else if (pointer->leaf){       // x is leaf
        return pointer->keys[0];     // return the minimum key of x
    }else{
        return BpTree_FindMin(pointer->Ch[0]);
    }
}

int BpTree::BpTree_FindMax(Node* pointer){
     if (pointer == NULL){          // pointer is empty
         return 0;
     }else if (pointer->leaf){       // pointer is leaf
         return pointer->keys[pointer->keys.size()-1];     // return the maximum key of x
     }else{
         return BpTree_FindMax(pointer->Ch[pointer->Ch.size()-1]);
     }
 }

Node *BpTree::getRoot() {
  return root;
}

/*
BpTree::BpTree() {
  root = NULL;
}
*/


// -----------------------Ex2.(iv)-------------------------


/*
 // Case 1: successor in Node (index <size-1)
 // Case 2: successor in next Node (index = size-1)

int BpTree::BpTree_Find_Successor(int key){
    Node* pointer = root;
    while(!pointer->leaf){
        Ch.size();
        for (int index = 0; index){
 
        }
        pointer = pointer->Ch[0];
    }
    int i,size;
    size = pointer->keys.size();
    for (i = 0; i < size; i++){
        if (pointer->keys[i]==key){
            return pointer->keys[i+1];
        }
    }
    cout<<"Error: The element entered is not found!";
    return 0;
}
 

int BpTree::BpTree_Find_Predecessor(int key){
    
}
 
 */


/*void BpTree::BpTree_Find_Predecessor(int key){
    int infinit = 9999999;
    int pre = infinit;

    BpTree_Find_Predecessor_Helper(key, root, &pre);
  
    if (pre != infinit){
        cout << "The predecessor of element " << key << " is "<< pre <<".\n"<<endl;
    }else{
        cout << "No predecessor for the key you input is found. Please check your input.\n";
    }

}

void BpTree::BpTree_Find_Successor(int key){
    int infinit = 9999999;
    int suc = infinit;

    BpTree_Find_Successor_Helper(key, root, &suc);

    if (suc != infinit){
        cout << "The successor of element " << key << " is "<< suc <<".\n"<<endl;
    }else{
        cout << "No successor for the key you input is found. Please check your input.\n";
    }
}
*/
//------------------HELP FUNCTION DEF----------------------------

/*void BpTree::BpTree_Find_Predecessor_Helper(int key, Node *node, int *pre){
    if (node == nullptr){
        return;
    }

    int index = node->Searchindex(key);

    if (index > 0){
        *pre = node->keys[index - 1];
    }

    if (key == node->keys[index]){
        if (!(node->leaf)){
            *pre = BpTree_FindMax(node->Ch[index]);
        }
        return;
    }
    BpTree_Find_Predecessor_Helper(key, node->Ch[index], pre);
}


void BpTree::BpTree_Find_Successor_Helper(int key, Node *node, int *suc){
    if (node == nullptr){
        return;
    }

    int index = node->Searchindex(key);
    
    if (key == node->keys[index]){
        if (!(node->leaf)){
            *suc = BpTree_FindMin(node->Ch[index + 1]);
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
    BpTree_Find_Successor_Helper(key, node->Ch[index], suc);
}
*/

int BpTree::BpTree_Find_Predecessor(int key){
    return this->root->BpTree_Find_Predecessor_Helper(key);
}

int Node::BpTree_Find_Predecessor_Helper(int key){
    if(leaf==true)
    {
        for(int i=keys.size()-1;i>=0;i--)
        {
            if(keys[i]<key)
            {
                return keys[i];
            }
        }
        return -1;
    }
    
    int i;
    
    for ( i = 0; i < keys.size(); i++)
    {
        if(keys[i]==key)
        {
            return Ch[i]->BpTree_Find_Predecessor_Helper(key);
        }
        if(keys[i]>key)
        {
            return Ch[i]->BpTree_Find_Predecessor_Helper(key);
        }
    }
    return Ch[i]->BpTree_Find_Predecessor_Helper(key);
}

int BpTree::BpTree_Find_Successor(int key)
{
    return this->root->BpTree_Find_Successor_Helper(key);
}

int Node::BpTree_Find_Successor_Helper(int key){
    if(leaf==true)
    {
        for(int i=0;i<keys.size();i++)
        {
            if(key<keys[i])
            {
                return keys[i];
            }
            if(key==keys[i])
            {
                return keys[i+1];
            }
        }
        return -1;
    }
    
    int i;
    
    for ( i = 0; i <keys.size(); i++)
    {
        if(keys[i]==key)
        {
            return Ch[i+1]->BpTree_Find_Successor_Helper(key);
        }
        
        if(keys[i]>key)
        {
            return Ch[i]->BpTree_Find_Successor_Helper(key);
        }
    }
    return Ch[i]->BpTree_Find_Successor_Helper(key);
}


/*
 
 int BpTree::BpTree_Find_Judge(int key) {
   if (root == NULL) {
     cout << "Tree is empty\n";
   } else {
     Node *cursor = root;
     while (cursor->IS_LEAF == false) {
       for (int i = 0; i < cursor->size; i++) {
         if (x < cursor->key[i]) {
           cursor = cursor->ptr[i];
           break;
         }
         if (i == cursor->size - 1) {
           cursor = cursor->ptr[i + 1];
           break;
         }
       }
     }
     for (int i = 0; i < cursor->size; i++) {
       if (cursor->key[i] == x) {
         //cout << "Found\n";
         return 1;
       }
     }
     //cout << "Not found\n";
     return 0;
   }
 }
 */

bool BpTree::Find_Key(int key){
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
        if(keys[i]>key)
        {
            return Ch[i]->Find_Key_Helper(key);
        }
    }
    return Ch[i]->Find_Key_Helper(key);
}

//-----------------------------------------------------------CLASS DEF-------------------------------------------------------------------------

int main()
{
    int choice,NLOrder,LOrder,e,n,x,min,max,suc,pre;
    cout<<"\nWelcome to the B-tree of CS225 Group 18.\n";
    cout<<"\nEnter the Interior Tree Order(>2): ";
   	cin>>NLOrder;

    cout<<"\nEnter the Leaf Tree Order(>2): ";
   	cin>>LOrder;

    BpTree* T = new BpTree(NLOrder,LOrder);

   s: cout<<"\n1: INSERT(small tree)\t  2:INSERT first n Numbers(big tree)\t 3:DELETE\t 4:PRINT\t 5:Min\t 6:MAX\t 7:PREDECESSOR\t 8:SUCCESSOR\t 9:DONE \n";
    cin>>choice;

    switch(choice)
    {
    	case 1:
    		cout<<"Please enter the element you want to insert: ";
    		cin>>e;
    		T->insert(e);
    		goto s;

    	case 2:
    		cout<<"Please enter the number of elements you want to insert n: ";
    		cin>>n;

    		for(int i=1;i<=n;i++)
    			T->insert(i);
    		goto s;

    	case 3:
    		cout<<"Please enter the element you want to delete: ";
    		cin>>x;
    		Lflag=0;
    		T->Delete(x);
    		goto s;

    	case 4:
            cout<<"A sample view of your current B+tree is shown as following: \n";
    		cout<<"\n\n";
    		T->print();
    		cout<<"\n";
    		goto s;

        case 5:
            /*cout<<"Notice: Please make sure that the B-tree is not empty now! Otherwise you will see segmentation fault error message due to invalid operation ordered by you!\n\n";*/
            if (T->root->keys.size() == 0){
                cout << "Invalid operation since tree is empty\n";
            }else{
                min = T->BpTree_FindMin(T->root);
                cout << "The minimun key in current B+tree is " << min << ".\n"<<endl;
            }
            goto s;

        case 6:
            /*cout<<"Notice: Please make sure that the B-tree is not empty now! Otherwise you will see segmentation fault error message due to invalid operation ordered by you!\n\n";*/
            if (T->root->keys.size() == 0){
                cout << "Invalid operation since tree is empty\n";
            }else{
                max = T->BpTree_FindMax(T->root);
                cout << "The maximun key in current B+tree is " << max << ".\n"<<endl;
            }
            goto s;
            
        case 7:
            cout<<"Please enter the element which you want to find its predecessor: ";
            cin>>e;
            if (T->Find_Key(e)==false){
                cout<<"Invalid input since your input key doesn't exist in B+tree! \n";
            }else{
                pre = T->BpTree_Find_Predecessor(e);
                cout<<"The predecessor of element "<< e <<" is "<< pre <<".\n"<<endl;
            }
            goto s;

        case 8:
            cout<<"Please enter the element which you want to find its successor: ";
            cin>>e;
            if (T->Find_Key(e)==false){
                cout<<"Invalid input since your input key doesn't exist in B+tree! \n";
            }else{
                suc = T->BpTree_Find_Successor(e);
                cout<<"The successor of element "<< e <<" is "<< suc <<".\n"<<endl;
            }
            goto s;

    	case 9:
            cout<<"\nThank you for using the B+tree of CS225 Group 18. You log out successfully! \n";
    		break;

    	default:
            cout<<"\nNo such case! Please check your input and re-execute! Ending program.\n ";
    		break;
    }

    return 0;
}


/* Sample Test Case for Min and MAX:
 
 Case 1:
 
 xuke@xukedeMacBook-Pro Ex2 % ./B+tree

 Welcome to the B-tree of CS225 Group 18.

 Enter the Interior Tree Order(>2): 4

 Enter the Leaf Tree Order(>2): 4

 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 2
 Please enter the number of elements you want to insert n: 10

 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 5
 The minimun key in current B+tree is 1.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 6
 The maximun key in current B+tree is 10.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 9

 Thank you for using the B+tree of CS225 Group 18. You log out successfully!
 
 Case 2:
 xuke@xukedeMacBook-Pro Ex2 % ./B+tree

 Welcome to the B-tree of CS225 Group 18.

 Enter the Interior Tree Order(>2): 6

 Enter the Leaf Tree Order(>2): 6

 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 2
 Please enter the number of elements you want to insert n: 100

 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 5
 The minimun key in current B+tree is 1.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 6
 The maximun key in current B+tree is 100.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 9

 Thank you for using the B+tree of CS225 Group 18. You log out successfully!
 
 Case 3:
 xuke@xukedeMacBook-Pro Ex2 % ./B+tree

 Welcome to the B-tree of CS225 Group 18.

 Enter the Interior Tree Order(>2): 8

 Enter the Leaf Tree Order(>2): 8

 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 2
 Please enter the number of elements you want to insert n: 1000

 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 5
 The minimun key in current B+tree is 1.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 6
 The maximun key in current B+tree is 1000.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min     6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 9

 Thank you for using the B+tree of CS225 Group 18. You log out successfully! 
 
 */

/* Sample Test Case for PREDECESSOR and SUCCESSOR:
 xuke@xukedeMacBook-Pro Ex2 % ./B+tree

 Welcome to the B-tree of CS225 Group 18.

 Enter the Interior Tree Order(>2): 4

 Enter the Leaf Tree Order(>2): 4

 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 2
 Please enter the number of elements you want to insert n: 50

 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 4
 A sample view of your current B+tree is shown as following:


                 1
                 2
             3
                 3
                 4
         5
                 5
                 6
             7
                 7
                 8
     9
                 9
                 10
             11
                 11
                 12
         13
                 13
                 14
             15
                 15
                 16
 17
                 17
                 18
             19
                 19
                 20
         21
                 21
                 22
             23
                 23
                 24
     25
                 25
                 26
             27
                 27
                 28
         29
                 29
                 30
             31
                 31
                 32
     33
                 33
                 34
             35
                 35
                 36
         37
                 37
                 38
             39
                 39
                 40
         41
                 41
                 42
             43
                 43
                 44
         45
                 45
                 46
             47
                 47
                 48
             49
                 49
                 50


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 8
 Please enter the element which you want to find its successor: 33
 The successor of element 33 is 34.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 8
 Please enter the element which you want to find its successor: 34
 The successor of element 34 is 35.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 8
 Please enter the element which you want to find its successor: 35
 The successor of element 35 is 36.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 8
 Please enter the element which you want to find its successor: 36
 The successor of element 36 is 37.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 8
 Please enter the element which you want to find its successor: 37
 The successor of element 37 is 38.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 8
 Please enter the element which you want to find its successor: 38
 The successor of element 38 is 39.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 8
 Please enter the element which you want to find its successor: 39
 The successor of element 39 is 40.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 8
 Please enter the element which you want to find its successor: 40
 The successor of element 40 is 41.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 8
 Please enter the element which you want to find its successor: 41
 The successor of element 41 is 42.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 8
 Please enter the element which you want to find its successor: 42
 The successor of element 42 is 43.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 8
 Please enter the element which you want to find its successor: 43
 The successor of element 43 is 44.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 8
 Please enter the element which you want to find its successor: 44
 The successor of element 44 is 45.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 8
 Please enter the element which you want to find its successor: 45
 The successor of element 45 is 46.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 8
 Please enter the element which you want to find its successor: 46
 The successor of element 46 is 47.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 3
 The predecessor of element 3 is 2.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 4
 The predecessor of element 4 is 3.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 5
 The predecessor of element 5 is 4.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 6
 The predecessor of element 6 is 5.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 7
 The predecessor of element 7 is 6.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 8
 The predecessor of element 8 is 7.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 9
 The predecessor of element 9 is 8.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 10
 The predecessor of element 10 is 9.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 11
 The predecessor of element 11 is 10.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 12
 The predecessor of element 12 is 11.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 13
 The predecessor of element 13 is 12.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 14
 The predecessor of element 14 is 13.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 15
 The predecessor of element 15 is 14.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 16
 The predecessor of element 16 is 15.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 17
 The predecessor of element 17 is 16.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 18
 The predecessor of element 18 is 17.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 19
 The predecessor of element 19 is 18.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 20
 The predecessor of element 20 is 19.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 21
 The predecessor of element 21 is 20.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 22
 The predecessor of element 22 is 21.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 23
 The predecessor of element 23 is 22.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 24
 The predecessor of element 24 is 23.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 25
 The predecessor of element 25 is 24.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 26
 The predecessor of element 26 is 25.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 27
 The predecessor of element 27 is 26.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 7
 Please enter the element which you want to find its predecessor: 28
 The predecessor of element 28 is 27.


 1: INSERT(small tree)      2:INSERT first n Numbers(big tree)     3:DELETE     4:PRINT     5:Min 6:MAX     7:PREDECESSOR     8:SUCCESSOR     9:DONE
 9

 Thank you for using the B+tree of CS225 Group 18. You log out successfully!

 */
