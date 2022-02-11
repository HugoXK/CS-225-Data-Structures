# Readme.md

[TOC]

## Quick Sort with AList

The code is in `rsort_and_qsort` folder.

The principle is pretty similar to the merge sort, because they both use **divide & conquer** algorithm. It uses an embedded `bsort()` to perform.

### Inner data structure

![innerDataStructures](/Users/mac/Desktop/CS225(Process 2.18)/Labs/Lab3/rsort_and_qsort/Graphs/innerDataStructures.png)

### Bubble Sort

This is too easy so no graphs are provided.

### Quicksort

![qsort](/Users/mac/Desktop/CS225(Process 2.18)/Labs/Lab3/rsort_and_qsort/Graphs/qsort.png)

## Radix Sort with AList

The code is in `rsort_and_qsort` folder.

Radix Sort is all about playing cards. It uses an embedded `ksort()` to perform.

### Inner Data Structures

![innerDataStructures](/Users/mac/Desktop/CS225(Process 2.18)/Labs/Lab3/rsort_and_qsort/Graphs/innerDataStructures.png)

### Radix Sort

Compared to selection sort, heap sort and merge sort, radix sort is **not** a sorting algorithm based on comparison. Note that any sorting algorithm based on comparison requires a complexity of at least $O(n\log n)$. Thus, radix sort & bucket sort is one way to break this through - they don’t necessarily need a lower bound. 

However, there seems to be some glitches (either by me or the writer), so I’d suggest for a more authoritative explanation for this part (incl. Bucket sort & Radix sort).

![rsort](/Users/mac/Desktop/CS225(Process 2.18)/Labs/Lab3/rsort_and_qsort/Graphs/rsort.png)

### Bucket Sort (K-sort)

![ksort](/Users/mac/Desktop/CS225(Process 2.18)/Labs/Lab3/rsort_and_qsort/Graphs/ksort.png)

## Merge Sort with DList

The code is in `mergeSort` folder.

Merge Sort is one of the most familiar sorting algorithms to us. It uses an embedded `ssort()` to perform.

### Inner Data Structures

![innerDataStructures](/Users/mac/Desktop/CS225(Process 2.18)/Labs/Lab3/mergeSort/Graphs/innerDataStructures.png)



### Selection Sort

![selectionSort](/Users/mac/Desktop/CS225(Process 2.18)/Labs/Lab3/mergeSort/Graphs/selectionSort.png)

### Merge Sort

![mergeSort](/Users/mac/Desktop/CS225(Process 2.18)/Labs/Lab3/mergeSort/Graphs/mergeSort.png)