#include<iostream>
#include<vector>
using namespace std;

template<class T> 
class MultiSet{
public:
    MultiSet() 
    {
        for (int i = 0; i < 13; ++i)
            hashtable.push_back(INT_MIN);
    }
    void insertion(int val)
    {
        int index = hash(val);
        if (hashtable[index] / 16 == val) hashtable[index]++;
        else 
        {
            while (hashtable[index] != INT_MIN && hashtable[index] / 16 != val) index = (index + 1) % 13;
            hashtable[index] = val * 16 + 1;
        }
    }
    void deletion(int val)
    {
        int index = hash(val);
        if (hashtable[index] / 16 == val) hashtable[index] = INT_MIN;
        else
        {
            if (hashtable[index] == INT_MIN) return;
            index = (index + 1) % 13;
            while (hashtable[index] != INT_MIN && hashtable[index] / 16 != val)
            {
                if (index == hash(val)) return;
                index = (index + 1) % 13;
            }
            hashtable[index] = INT_MIN;
        }
    }
    vector<int> all()
    {
        vector<int> ret;
        for (int i = 0; i < hashtable.size(); ++i)
        {
            if (hashtable[i] == INT_MIN) continue;
            else
            {
                for (int j = 0; j < hashtable[i] - (hashtable[i] / 16) * 16; ++j)
                    ret.push_back(hashtable[i] / 16);
            }
        }
        if (ret.size() == 0) return ret;
        for (int i = 0; i < ret.size() - 1; i++)
            for (int j = 0; j < ret.size() - 1 - i; j++)
            { 
                if (ret[j] > ret[j + 1])
                { 
                    int temp = ret[j];
                    ret[j] = ret[j + 1];
                    ret[j + 1] = temp;
                }
            }
        return ret;
    }
    int retrieval(int val)
    {
        int index = hash(val);
        if (hashtable[index] / 16 == val)return val;
        if (hashtable[index] == INT_MIN) return INT_MIN;
        index = (index + 1) % 13;
        while (index != hash(val) && hashtable[index] != INT_MIN && hashtable[index] / 16 != val)
            index = (index + 1) % 13;
        if (hashtable[index]/16 == val)  return val;
        else return INT_MIN;
    }
    MultiSet setunion(MultiSet A, MultiSet B)
    {
        MultiSet Union;
        auto ret1 = A.all();
        auto ret2 = B.all();
        int len1 = ret1.size();
        int len2 = ret2.size();
        int i = 0;
        int j = 0;
        while (i < len1 && j < len2)
        {
            Union.insertion(ret1[i]);
            Union.insertion(ret2[i]);
            i++;
            j++;
        }
        while (i < len1)
        {
            Union.insertion(ret1[i]);
            i++;
        }
        while (j < len2)
        {
            Union.insertion(ret2[j]);
            j++;
        }
        return Union;
    }
    MultiSet intersection(MultiSet A, MultiSet B)
    {
        MultiSet Union;
        auto ret1 = A.all();
        auto ret2 = B.all();
        int len1 = ret1.size();
        int len2 = ret2.size();
        int i = 0;
        int j = 0;
        while (i < len1 && j < len2)
        {
            int val1 = ret1[i];
            int val2 = ret2[j];
            if (val1 < val2)
            {
                i++;
            }
            else if (val1 == val2)
            {
                Union.insertion(val1);
                i++;
                j++;
            }
            else
            {
                j++;
            }
        }
        return Union;
    }
    MultiSet difference(MultiSet A, MultiSet B)
    {
        MultiSet Union;
        auto ret1 = A.all();
        auto ret2 = B.all();
        int len1 = ret1.size();
        int len2 = ret2.size();
        int i = 0;
        int j = 0;
        while (i < len1 && j < len2)
        {
            int val1 = ret1[i];
            int val2 = ret2[j];
            if (val1 < val2)
            {
                Union.insertion(val1);
                i++;
            }
            else if (val1 == val2)
            {
                i++;
                j++;
            }
            else j++;
        }
        while (i < len1)
        {
            int val1 = ret1[i];
            Union.insertion(val1);
            i++;
        }
        return Union;
    }
    void print()
    {
        vector<int> ret = all();
        if (ret.size() == 0)
        {
            cout << "It's empty!" << endl;
            return;
        }
        for (int i = 0; i < ret.size(); ++i)
            cout << ret[i] << " ";
        cout << endl;
    }
    int hash(int val) { return (3 * val + 100) % 13; }
private: 
    // use a binary search tree to act as a map, the value of each node contains element and count
    // value = element * 16 + count
    vector<int> hashtable;
};


struct ChainNode
{
    ChainNode* next = nullptr;
    int value;
    int count = 1;
};

template<class T>
class MultiSet_Chaining{
public:
    MultiSet_Chaining() {}
    void insertion(int val) 
    {
        auto root = head;
        if (!root)
        {
            head = new ChainNode;
            head->value = val;
            return;
        }
        if (root->value > val)
        {
            ChainNode* tmp = new ChainNode;
            tmp->next = head;
            tmp->value = val;
            head = tmp;
            return;
        }
        while (root)
        {
            if (root->value == val)
            {
                root->count++;
                return;
            }
            else if (!root->next)
            {
                ChainNode* tmp = new ChainNode;
                root->next = tmp;
                tmp->value = val;
                return;
            }
            else if (root->next->value > val)
            {
                ChainNode* tmp = new ChainNode;
                tmp->next = root->next;
                root->next = tmp;
                tmp->value = val;
                return;
            }
            else root = root->next;
        }
    }
    void deletion(int val)
    {
        auto root = head;
        if (!root) return;
        while (root)
        {
            if (root->value < val)
            {
                if (!root->next) return;
                else if (root->next->value < val) root = root->next;
                else if (root->next->value == val)
                {
                    auto tmp = root->next;
                    root->next = root->next->next;
                    delete tmp;
                    return;
                }
                else return;
            }
            else if (root->value == val)
            {
                auto tmp = root;
                root = root->next;
                delete tmp;
                return;
            }
            else return;
        }
    }
    vector<int> all()
    {
        vector<int> ret;
        auto root = head;
        while (root)
        {
            for (int i = 0; i < root->count; ++i) 
                ret.push_back(root->value);
            root = root->next;
        }
        return ret;
    }
    int retrieval(int val)
    {
        ChainNode* root = head;
        while (root && root->value < val) root = root->next;
        if (!root || root->value > val) return INT_MIN;
        else return val;
    }
    MultiSet_Chaining setunion(MultiSet_Chaining A, MultiSet_Chaining B)
    {
        MultiSet_Chaining Union;
        auto ret1 = A.all();
        auto ret2 = B.all();
        int len1 = ret1.size();
        int len2 = ret2.size();
        int i = 0;
        int j = 0;
        while (i < len1 && j < len2)
        {
            Union.insertion(ret1[i]);
            Union.insertion(ret2[i]);
            i++;
            j++;
        }
        while (i < len1)
        {
            Union.insertion(ret1[i]);
            i++;
        }
        while (j < len2)
        {
            Union.insertion(ret2[j]);
            j++;
        }
        return Union;
    }
    MultiSet_Chaining intersection(MultiSet_Chaining A, MultiSet_Chaining B)
    {
        MultiSet_Chaining Union;
        auto ret1 = A.all();
        auto ret2 = B.all();
        int len1 = ret1.size();
        int len2 = ret2.size();
        int i = 0;
        int j = 0;
        while (i < len1 && j < len2)
        {
            int val1 = ret1[i];
            int val2 = ret2[j];
            if (val1 < val2)
            {
                i++;
            }
            else if (val1 == val2)
            {
                Union.insertion(val1);
                i++;
                j++;
            }
            else
            {
                j++;
            }
        }
        return Union;
    }
    MultiSet_Chaining difference(MultiSet_Chaining A, MultiSet_Chaining B)
    {
        MultiSet_Chaining Union;
        auto ret1 = A.all();
        auto ret2 = B.all();
        int len1 = ret1.size();
        int len2 = ret2.size();
        int i = 0;
        int j = 0;
        while (i < len1 && j < len2)
        {
            int val1 = ret1[i];
            int val2 = ret2[j];
            if (val1 < val2)
            {
                Union.insertion(val1);
                i++;
            }
            else if (val1 == val2)
            {
                i++;
                j++;
            }
            else j++;
        }
        while (i < len1)
        {
            int val1 = ret1[i];
            Union.insertion(val1);
            i++;
        }
        return Union;
    }
    void print()
    {
        vector<int> ret = all();
        if (ret.size() == 0)
        {
            cout << "It's empty!" << endl;
            return;
        }
        for (int i = 0; i < ret.size(); ++i)
            cout << ret[i] << " ";
        cout << endl;
    }
private:
    ChainNode* head;
};


int main() 
{
    int input1[] = { 1, 1, 1, 5, 2 };
    int input2[] = { 0, 2, 12, 3, 3, 3, 3, 4, 5, 5, 42 };

    MultiSet<int> test1, test2;
    MultiSet_Chaining<int> test6, test7;

    for (auto i : input1) {
        test1.insertion(i);
        test6.insertion(i);
    }
    for (auto i : input2) {
        test2.insertion(i);
        test7.insertion(i);
    }    

    /* Test for part(i) */
    cout << "Test of Part i:\n";

    cout << "Insertion Test for input1 , expected output: 1 1 1 2 5 \n";
    test1.print();
    cout << "Insertion Test for input2 , expected output: 0 2 3 3 3 3 4 5 5 12 42 \n";
    test2.print();

    cout << "Retrieval Test for input 1, input: 1, 3 expected output: 1 INT_MIN\n";
    cout << test1.retrieval(1) << " " << test1.retrieval(3) << endl;
    cout << "Retrieval Test for input 2, input: -1, 4 expected output: INT_MIN 4\n";
    cout << test2.retrieval(-1) << " " << test2.retrieval(4) << endl;

    cout << "Union Test for input1&2 , expected output: 0 1 1 1 2 2 3 3 3 3 4 5 5 5 12 42\n";
    MultiSet<int> test3 = test1.setunion(test1, test2);
    test3.print();

    cout << "Intersection Test for input1&2 , expected output: 2 5\n";
    MultiSet<int> test4 = test1.intersection(test1, test2);
    test4.print();

    cout << "Difference Test for input1&2 , expected output: 1 1 1\n";
    MultiSet<int> test5 = test1.difference(test1, test2);
    test5.print();

    cout << "Deletion Test for input1 , expected output: 1 1 1 5 \n";
    test1.deletion(2);
    test1.print();
    cout << "Deletion Test for input2 , expected output: 0 2 4 5 5 12 42\n";
    test2.deletion(3);
    test2.print();
    
    cout << " \n \n \n";


    /* Test for part(ii) */
    cout << "Test of Part ii:\n";

    cout << "Insertion Test for input1 , expected output: 1 1 1 2 5 \n";
    test6.print();
    cout << "Insertion Test for input2 , expected output: 0 2 3 3 3 3 4 5 5 12 42 \n";
    test7.print();

    cout << "Retrieval Test for input 3, input: 1, 3 expected output: 1 INT_MIN\n";
    cout << test6.retrieval(1) << " " << test6.retrieval(3) << endl;
    cout << "Retrieval Test for input 4, input: -1, 4 expected output: INT_MIN 4\n";
    cout << test7.retrieval(-1) << " " << test7.retrieval(4) << endl;

    cout << "Union Test for input1&2 , expected output: 0 1 1 1 2 2 3 3 3 3 4 5 5 5 12 42\n";
    MultiSet_Chaining<int> test8 = test6.setunion(test6, test7);
    test8.print();

    cout << "Intersection Test for input1&2 , expected output: 2 5\n";
    MultiSet_Chaining<int> test9 = test6.intersection(test6, test7);
    test4.print();

    cout << "Difference Test for input1&2 , expected output: 1 1 1\n";
    MultiSet_Chaining<int> test10 = test6.difference(test6, test7);
    test10.print();

    cout << "Deletion Test for input1 , expected output: 1 1 1 5 \n";
    test6.deletion(2);
    test6.print();
    cout << "Deletion Test for input2 , expected output: 0 2 4 5 5 12 42\n";
    test7.deletion(3);
    test7.print();

    return 0;
}