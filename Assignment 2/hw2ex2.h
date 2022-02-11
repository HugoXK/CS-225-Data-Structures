/*
 * @ Author     : 
 * 
 * @ File       : 
 * 
 * @ Purpose    : 
 * 
 * @ Usage      : 
 * 
 * @ Description: 
 * 
 * @ Input      : 
 * 
 * @ Output     : 
 * 
 */

#include <vector>
#include <iostream>
using namespace std;

class booking{
    public:
        booking(int a, int d){
            arrival = a;
            departure = d;
        };
        int arrival; 
        int departure; 
};


class Bookings{
    public:
        Bookings(vector<booking> bookings, int K){
            bks = bookings;
            k = K;
        };
        vector <booking> bks;
        int k;
        void bookings_print();
        void test();
        
        // Modify the following to solve the exercise
        // You may also add your own help function
        bool checkdemands();

    private:

};

void MergeSort(int *date, int *sym, int *tmp1, int *tmp2, int l, int r){
    if(l == r)  return;
    int mid = (l + r) >> 1;
    MergeSort(date, sym, tmp1, tmp2, l, mid);
    MergeSort(date, sym, tmp1, tmp2, mid+1, r); //sort the left and right part
    int pt1 = l, pt2 = mid + 1, c = 0;
    while(pt1 <= mid && pt2 <= r){
        c ++;
        if(date[pt1] > date[pt2] || (date[pt1] == date[pt2] && sym[pt1] > sym[pt2])){   //the right is smaller
            tmp1[c] = date[pt2];
            tmp2[c] = sym[pt2];
            pt2 ++;
        }
        else{
            tmp1[c] = date[pt1];
            tmp2[c] = sym[pt1];
            pt1 ++;
        }
    }
    while(pt1 <= mid){
        c ++;
        tmp1[c] = date[pt1];
        tmp2[c] = sym[pt1];
        pt1 ++;
    }
    while(pt2 <= r){
        c ++;
        tmp1[c] = date[pt2];
        tmp2[c] = sym[pt2];
        pt2 ++;
    }//remain a list not empty
    for(int i = 1; i <= c; i++){
        date[l+i-1] = tmp1[i];
        sym[l+i-1] = tmp2[i];
    }   //copy back to original list
}

// You should right the following to finish the exercise
// You may also add your own help function
bool Bookings::checkdemands(){
    int siz = bks.size();
    int cnt = siz * 2;
    int date[cnt+5], sym[cnt+5];    //two arrays to store the dates and the type (arr or depa)
    int tmp1[cnt+5], tmp2[cnt+5];   //two temporary arrays for mergesort
    for(int i = 1; i <= siz; i++){  //copy all data to one single array
        date[i] = bks[i-1].arrival;
        sym[i] = 1;
        date[i+siz] = bks[i-1].departure;
        sym[i+siz] = -1;
    }
    MergeSort(date, sym, tmp1, tmp2, 1, cnt);
    /*for(int i = 1; i <= cnt; i++)  //debug mergesort
        cout << date[i] << ' ';
    cout << endl;
    for(int i = 1; i <= cnt; i++)
        cout << sym[i] << ' ';
    cout << endl;*/
    int sum = 0;
    for(int i = 1; i <= cnt; i++){
        sum += sym[i];
        if(sum > k)  return false;  //if at some time the total rooms needed more than k, return false
    }
    return true;
};

// DO NOT modify the following 
void Bookings::bookings_print(){
    cout << "Bookings as follow:" << endl;
    for(booking i: this->bks){
        cout << "A: " << i.arrival << "  | D: " << i.departure << endl;
    }
    cout << "Max available room:" << this->k << endl;
    return ;
}

void Bookings::test(){
    bookings_print();
    cout << "Check whether satisfy the demands: " << (checkdemands()? "Success":"Fail") << endl;
}