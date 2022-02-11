#include <iostream>
#include <ctime>                /* For performance comparasion */
#include <vector>
#include "hash_extend_f.h"
#include "hashset_f.h"
using namespace std;

int main(){
    /******************* Put your own test here *******************/
    /* Your are expected to give adequate test to show the correctness of your implementation */
    /* Otherwise you may be deducted points */

    /**************************************************************/
    /* Check Point 1: Correctness */
    int input1[] =  {1, 1, 1, 5, 2};
    int input2[] = {0, 2, 12, 3, 3, 3, 3, 4, 5, 5, 42};

    HashSet_LCFS<int> test1, test2;
    HashSet_RH<int> test3, test4;
    /* Add test to your container */
    for(auto i: input1){
        test1.add(i);
        test3.add(i);
    }


    for (auto i:input2){
        test2.add(i);
        test4.add(i);
    }


    /* Test on both inputs, missing operation would be deducted points */
    /* Test for LCFS Hash*/
    
    test1.add(3);
    test1.add(23);
    test1.add(7);

    test3.add(3);
    test3.add(23);
    test3.add(7);
    /* Test for RH Hash */
    
    test2.add(25);
    test2.add(6);
    test2.add(43);

    test4.add(25);
    test4.add(6);
    test4.add(43);
    printf("CHECK POINT1:\n");
    printf("(1)Test for LCFS Hash: input1——add 3, 23, 7 input2——add 25, 6, 43\n");
    printf("input1_LCFS\n");
    test1.display();
    printf("input2_LCFS\n");
    test2.display();
    printf("(2)Test for RH Hash: input1——add 3, 23, 7 input2——add 25, 6, 43\n");
     printf("input1_RH\n");
    test3.display();
     printf("input2_RH\n");
    test4.display();


    /**************************************************************/
    /* Check Point 2: Compare the performance with initial hashset as baseline */
    /* ctime is included for you to help */
    printf("CHECK POINT2:\n");
    printf("(3)Compare the performance:\n");
    vector<int> input3;
    for(int i=0;i<=800000;i++){
        input3.push_back(rand()%20000000+1);
    }
    hashset<int> init;
    HashSet_LCFS<int> init1;
    HashSet_RH<int> init2;
    
    clock_t time_stt = clock();
    for (auto i:input3){
        init.add(i);
    }
    
    cout<<"The time cost of the initial hashset is "<<1000*(clock()-time_stt)/(double)CLOCKS_PER_SEC<<"ms"<<endl;
    
    clock_t time_1 = clock();
    for (auto i:input3){
        init1.add(i);
    }
    cout<<"The time cost of the LCFS hashset is "<<1000*(clock()-time_1)/(double)CLOCKS_PER_SEC<<"ms"<<endl;
    
    clock_t time_2 = clock();
    for (auto i:input3){
        init2.add(i);
    }
    cout<<"The time cost of the RH hashset is "<<1000*(clock()-time_2)/(double)CLOCKS_PER_SEC<<"ms"<<endl;

    return 0; 
}
