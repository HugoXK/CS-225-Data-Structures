#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstdio>

using namespace std;

void InsertionSort(double *data, int l, int r)
{
	double tmp;
	int i;
	int j;
	for (i = l;i <= r;i++) {
		tmp = data[i];
		for (j = i - 1;j >= l && data[j] > tmp;j--) {
			data[j + 1] = data[j];
		}
		data[j + 1] = tmp;
	}
}

void MergeSort1(double *data, double *tmp, int *hh, int *tt, int l, int r, int k) {
	if (l == r) return;
	if(k > r-l+1)  k = r-l+1;
	int len = (r-l+1) / k;
	for (int i = 1; i <= k; i++) {
		hh[i] = l + len * (i-1);
		tt[i] = hh[i] + len - 1;
		if(i == k)  tt[i] = r;
		MergeSort1(data, tmp, hh, tt, hh[i], tt[i], k); // divide the task
	}
	for (int i = 1; i <= k; i++) {
		hh[i] = l + len * (i-1);
		tt[i] = hh[i] + len - 1;
		if(i == k)  tt[i] = r;
	}
	int cc = 0;
	while (true) { //find the min
		double Min = 1000000000.0;
		int pos = 1;
		int em = 0;
		for (int i = 1; i <= k; i++) {
	
			if (hh[i] > tt[i]) { 
				em++;
				continue; 
			}
			if (data[hh[i]] < Min) {
				Min = data[hh[i]];
				pos = i;
			}
		}
		if (em == k)  break;
		tmp[++cc] = data[hh[pos]];
		hh[pos] ++;
	}
	for (int i = 1; i <= cc; i++) // copy back
		data[l + i - 1] = tmp[i];
}


void MergeSort2(double *data, double *tmp, int l, int r, int t) {
	if (r - l + 1<= t) {
		InsertionSort(data, l, r);  //use insertion-sort
		return;
	}
	int mid = (l + r) >> 1;
	MergeSort2(data, tmp, l, mid, t);
	MergeSort2(data, tmp, mid + 1, r, t);
	int pt1 = l, pt2 = mid + 1, c = 0;
	while (pt1 <= mid && pt2 <= r) {
		c ++;
		if (data[pt1] > data[pt2]) {
			tmp[c] = data[pt2];
			pt2 ++;
		}
		else { 
			tmp[c] = data[pt1];
			pt1 ++;
		}
	}
	while (pt1 <= mid) {
		c ++;
		tmp[c] = data[pt1];
		pt1 ++;
	}
	while (pt2 <= r) {
		c ++;
		tmp[c] = data[pt2];
		pt2 ++;
	}
	for (int i = 1;i <= c; i++) 
		data[l + i - 1] = tmp[i];  //copy back
}

int main(){
	int n, k, t;
	cout << "Enter n, k, t\n";
	cin >> n >> k >> t;
	double data[n + 5], tmp[n + 5];
	int hh[n + 5], tt[n + 5];
	double o[n + 5];
	for (int i = 1; i <= n; i++){
		cin >> data[i]; o[i]=data[i];}
	InsertionSort(data, 1, n); ///selfsort
	for (int i = 1; i <= n; i++)  cout << data[i] << ' ';  cout << endl;
	for (int i = 1; i <= n; i++)
		data[i]=o[i];
	MergeSort1(data, tmp, hh, tt, 1, n, k); //ex1.1
	for (int i = 1; i <= n; i++)  cout << data[i] << ' ';  cout << endl;
	for (int i = 1; i <= n; i++)
		data[i]=o[i];
	MergeSort2(data, tmp, 1, n, t); //ex1.2
	for (int i = 1; i <= n; i++)  cout << data[i] << ' ';  cout << endl;
	/*
	test cases:
	#1
	1 3 2 8 0 10 4 100 9
	(9, 4, 2)
	0 1 2 3 4 8 9 10 100
	#2
	2.5 3.4 1.9 7.8 3.9 1 9 0 -1.7
	(9, 4, 4)
	-1.7 0 1 1.9 2.5 3.4 3.9 7.8 9
	*/

	return 0;
}
