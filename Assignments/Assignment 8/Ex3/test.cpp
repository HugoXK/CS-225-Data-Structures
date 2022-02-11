//
//  test.cpp
//
//for testing the graph class
//
//  Created by KD on 11.04.21.
//

#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include "graph.cpp"
#include <vector>
using namespace std;
using std::cout;
using std::cin;

int main()
{
    vector<int> boys;
    for(int i = 2; i<=8; i=i+2){
        boys.push_back(i);  //boys have even id
    }
    vector<int> girls;
    for(int i = 1; i<=7; i=i+2){
        girls.push_back(i); //girls have odd id
    }
    graph<int> * mygraph = new graph<int>;  // create a new graph for partial match
    for(int i = 0; i < 4; i++){
        mygraph->addvertex(boys[i]);
        mygraph->addvertex(girls[i]);
    }
    //add edges
    cout << "test 1\n";
    mygraph->addedge(1,2);
    mygraph->addedge(3,6);
    mygraph->addedge(5,4);
    mygraph->addedge(5,8);
    mygraph->addedge(7,8);
    mygraph->prettyprint();
    cout << "Is perfect match? 1 true 0 false\n";
    cout << mygraph->is_perfect_matching() <<"\n";

    //clear mygraph
    mygraph->deleteedge(1,2);
    mygraph->deleteedge(3,6);
    mygraph->deleteedge(5,4);
    mygraph->deleteedge(5,8);
    mygraph->deleteedge(7,8);
   
    cout << "test 2 \n";
    mygraph->addedge(1,2);
    mygraph->addedge(3,2);
    mygraph->addedge(5,4);
    mygraph->addedge(7,6);
    mygraph->addedge(7,8);
    mygraph->prettyprint();
    cout << "Is perfect match? 1 true 0 false\n";
    cout << mygraph->is_perfect_matching() <<"\n";
    return 0;
}
