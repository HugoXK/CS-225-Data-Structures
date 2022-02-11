// We implement a solution to maxflow problem using Dinic algorithm and thus determine the existence 
// of perfect matching for a given bipartite graph
// ps: Dinic is very fast for bipartite graph, it's O(n^0.5 * M)
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <cstring>
#include <ctime>
#include <algorithm>
#include <vector>
#include <queue>
#include <map>
#define maxn 100100  //max vertices
#define maxm 1000100  //max edges
#define INF 0x7FFFFFFF
using namespace std;
// n is the vertices on the left
// m is the vertices on the right
// e is the total number of edges
// the rest e lines gives u, v which means there is an edge between u and v (u from left v from right)
int n, m, e, s, t, cur = -1;
int level[maxn], q[maxn];
struct List{
    int obj, cap;
    List *next, *rev;
}Edg[maxm], *head[maxn], *iter[maxn];

void Addedge(int a, int b, int c){
    Edg[++cur].next = head[a];
    Edg[cur].obj = b;
    Edg[cur].cap = c;
    Edg[cur].rev = Edg+(cur^1);
    head[a] = Edg+cur;
}

bool bfs(){  //build the graph by dividing into diff levels
    int hh, tt;
    q[hh = tt = 0] = s;
    for(int i = s; i <= t; i++)  level[i] = -1;
    level[s] = 0;
    while(hh <= tt){
        int now = q[hh++];
        for(List *p = head[now]; p; p = p->next){
            int v = p->obj, c = p->cap;
            if(c && level[v] == -1){
                level[v] = level[now] + 1;
                q[++tt] = v;
            }
        }
    }
    return level[t] != -1;
}

int Dinic(int now, int f){  //Dinic algorithm
    if(now == t || !f)  return f;
    int ret = 0;
    for(List *&p = iter[now]; p; p = p->next){
        int v = p->obj, c = p->cap;
        if(c && level[v] > level[now]){
            int d = Dinic(v, min(f, c));
            f -= d;
            p->cap -= d;
            ret += d;
            p->rev->cap += d;
            if(!f)  break;
        }
    }
    return ret;
}

int MaxFlow(){	//maxflow to get the max matching
    int flow = 0;
    while(bfs()){
        for(int i = s; i <= t; i++)  iter[i] = head[i];
        flow += Dinic(s, INF);
    }
    return flow;
}

int main(){
	
	//test cases: (You should input data by yourself to the terminal)
	
	//test case #1
	//input:
	/*
		1 1 1
		1 1
	*/
	//output:
	/*
		Yes
	*/
	
	
	//test case #2
	//input:
	/*
		4 2 7
		3 1
		1 2
		3 2
		1 1
		4 2
		4 1
		1 1
	*/
	//output:
	/*
		No
	*/
	
	scanf("%d%d%d", &n, &m, &e);
	
	s = 1;  t = s + n + m + 1;
	for(int i = s; i <= t; i++)  head[i] = NULL;
	
	for(int i = 1; i <= n; i++){
	  	Addedge(s, s+i, 1);
	  	Addedge(s+i, s, 0);
	}
	for(int i = 1; i <= m; i++){
      	Addedge(s+n+i, t, 1);
      	Addedge(t, s+n+i, 0);
	}
	
	int u, v;
	for(int i = 1; i <= e; i++){
	  	scanf("%d%d", &u, &v);
	  	Addedge(s+u, s+n+v, 1);
	  	Addedge(s+n+v, s+u, 0);
	}		//build the graph

	if(MaxFlow() == n && n == m)
		printf("Yes");
	else
		printf("No");
    return 0;
} 
