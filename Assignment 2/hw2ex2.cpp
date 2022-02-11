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
#include "hw2ex2.h"
#include <vector>
#include <iostream>
#include <stdio.h>
#include <cstdlib>

using namespace std;
int main(){
    vector<booking> b1({booking(1,2), booking(2,3), booking(2,4), booking(3,4)});

    // =========== Test 1 ===========
    Bookings test1 = Bookings(b1, 8);
    cout << "Test 1: " << endl;
    test1.test();
    cout << "Gold Result: Success" << endl;

    // =========== Test 2 ===========
    Bookings test2 = Bookings(b1, 1);
    cout << "\nTest 2: " << endl;
    test2.test();
    cout << "Gold Result: Fail" << endl;

    // =========== Hidden Test =========
    // You could addd your own test example
    vector<booking> b2({booking(1,5), booking(2,5), booking(1,2), booking(6,7)});
    vector<booking> b3({booking(1,2), booking(3,4), booking(1,100), booking(99,100)});
    // =========== Test 3 ===========
    Bookings test3 = Bookings(b2, 2);
    cout << "\nTest 3: " << endl;
    test3.test();
    cout << "Gold Result: Success" << endl;

    // =========== Test 4 ===========
    Bookings test4 = Bookings(b3, 1);
    cout << "\nTest 4: " << endl;
    test4.test();
    cout << "Gold Result: Fail" << endl;
    return 0;
}
/*
Suppose you have to process n advance bookings of rooms for a hotel with k
identical rooms. Bookings contain an arrival date and a departure date. You have to find out
whether there are enough rooms in the hotel to satisfy the demands.

Here we suppose that the arrival time is at night and departure time is at noon, so that on the same day the departure
is earlier than another arrival.
*/