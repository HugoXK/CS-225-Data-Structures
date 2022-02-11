#include <iostream>
#include <math.h>
#include <cstdlib>
#include <map>
#include <vector>
#include "Ex2.h"
using namespace std;

template<class T> int NumSet<T>::Check_Relation(vector<pair<int,int> > R){
    /*
    Input:
        R: The relation to be checked. Each element in R is a pair(int,int)
    Output:
        0: The binary relation R is not symmetric
        1: The binary relation R is symmetric
    */
    // Declare the variables used
    int first_element;
    int second_element;
    int row_index;
    int row_index_two;
    // Initialize the return value
    int check_return_value = 1;
    // Initialize hashing table
    DList <pair<int,int> > hashing_table[sizeof(R)];
    
    int initial = 0 ;
    while (  initial < sizeof(R) ){
        first_element = R[initial].first;
        row_index = first_element % sizeof(R);
        hashing_table[row_index].append(R[initial]);
        initial++;
    }
   
    // Judge whether symmetric
    int judge = 0;
    while (  judge < sizeof(R) ){
        second_element = R[judge].second;
        row_index_two = second_element % sizeof(R);
        int depth = 0;
        while (depth < hashing_table[row_index_two].getlength()){
            if (hashing_table[row_index_two].getitem(depth+1).second == R[judge].first){
                break;
            }else{
                
                if ( depth+1 < hashing_table[row_index_two].getlength() ){
                    depth++;
                }else{
                    check_return_value = check_return_value & 0;
                    return check_return_value;
                }
                
            }
        }
        
        
        
        judge ++;
    }
    
    
    return check_return_value;
};
int main(){
    int Set_A[10]={1,2,3,4,5,6,7,8,9,10};
    NumSet<int> S(Set_A,10);
    //Here is for Test 1
    vector<pair<int,int> > Relation_R1;
    for (int i=0; i<10; i++){
        Relation_R1.push_back(make_pair(Set_A[i],Set_A[i]));
        for (int j=0; j<10; j++){
            if (i != j)
                Relation_R1.push_back(make_pair(Set_A[i],Set_A[j]));
        }
    }
    cout << "For Test 1:"<<endl;
    cout << "The correct answer should be Yes"<<endl;
    cout << "Your answer is:" << ((S.Check_Relation(Relation_R1) == 1)?"Yes":"No")<< endl;
    //Here is for Test 2
    vector<pair<int,int> > Relation_R2;
    for (int i=0; i<10; i++){
        Relation_R2.push_back(make_pair(Set_A[i],Set_A[i]));
        for (int j=0; j<10; j++){
            if ((i != j) && (j%2 != 1))
                Relation_R2.push_back(make_pair(Set_A[i],Set_A[j]));
        }
    }
    cout << "For Test 2:"<<endl;
    cout << "The correct answer should be No"<<endl;
    cout << "Your answer is:" << ((S.Check_Relation(Relation_R2)== 1)?"Yes":"No")<< endl;
    
    return 0;
}
