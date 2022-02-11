//
//  solution.h
//

#ifndef TRIE_SOLUTION_H
#define TRIE_SOLUTION_H

#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include "trie.h"
using std::cout;
using std::cin;

// Helper function you may or may not use, convert AList<char> into string
string CharList2Str(AList<char> list) {
    string s;
    for (int i = 0; i < list.getlength(); ++i) {
        s += list[i+1];
    }
    return s;
}

/* ========== Function headers for Ex.2 (i) ========== */
template<class T> AList<string> trie<T>::return_extend(AList<T> list, AList<string> &result) {
    trienode<char> *pt = start;
    AList<char> word;
    string word1;
    for (int i = 1; i <= list.getlength();i++)
    {
        while ((*pt).data != list[i])
        {
            pt = (*pt).getnext(); 
        }
        word.append((*pt).data);
        pt = (*pt).getfollow();     
    }
    if (pt == NULL)
    {
        word1 = CharList2Str(word);
        result.insert(0,word1);
        return result;
    }
    else
    {   
        while ((*pt).getfollow() != NULL)
        {
            pt = (*pt).getnext();   
        }
        word.append((*pt).data);
        word1 = CharList2Str(word);
        result.insert(0,word1);
        return result;
    }
}

/* ========== Function headers for Ex.2 (ii) ========== */
template<class T> AList<string> trie<T>::return_prefix(AList<T> list, AList<string> &result) {
    trienode<char> *pt = start;
    AList<char> word;
    string word1;
    for (int i = 1; i <= list.getlength()-1;i++)
    {
        while ((*pt).data != list[i])
        {
            pt = (*pt).getnext();
            if (pt == NULL) break; 
        }
        if (pt == NULL) break; 
        word.append((*pt).data);
        pt = (*pt).getfollow();     
    }
    word1 = CharList2Str(word);
    result.insert(0,word1);
    return result;
}


/* ========== Function headers for Ex.2 (iii) ========== */
template<class T> AList<string> trie<T>::return_diff(AList<T> list, AList<string> &result) {
    AList<char> word;
    string word1;
    int diff;
    for (int i = 1; i <= list.getlength();i++)//i-th charactor is different
    {
        diff = list.getlength();
        trienode<char> *pt = start;
        trienode<char> *pt1;
        //cout<<1<<endl;
        for(int t = 0; t <= word.getlength();t++)
        {
            word.remove(t);
        }
        for (int j = 1; j <= list.getlength();j++)
        {
            
            if (j == i)
            {
                pt1 = (*pt).getfollow();
                while ((*(pt->follow)).data != list[j+1] && j+1 <= list.getlength())//(*(pt->follow)).data != list[j+1]
                {
                    pt = (*pt).getnext();
                    if (pt == NULL) break;

                }
                //cout<<3<<endl;
                if (pt == NULL) break;
                diff = diff-1;
                word.append((*pt).data);
                pt = (*pt).getfollow();
                continue;
            }
            if (pt == NULL) break;
            while ((*pt).data != list[j])
            {
                pt = (*pt).getnext();
                if (pt == NULL) break;  
            }
            if (pt == NULL) break;
            diff = diff-1; 
            word.append((*pt).data);
            pt = (*pt).getfollow();  
        }
        if (diff == 0) break;          
    }
    word1 = CharList2Str(word);
    result.insert(0,word1);
    return result;
}

// Helper function you may or may not use
template<class T> bool trie<T>::contains_extend(trienode<T> *pt, AList<T> list, AList<T> path, AList<string> &result) {
    return 0;
}

// Helper function you may or may not use
template<class T> bool trie<T>::contains_diff(trienode<T> *pt, AList<T> list, AList<T> path, AList<string> &result, int diff) {
    return 0;
}

#endif //TRIE_SOLUTION_H
