//Since the template given is literally a piece of SHIT, I have to create a new one
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
#define maxn 5000  //max number of vertices in a graph

using namespace std;

struct Graph{
	bool edge[maxn][maxn];
	bool Exi[maxn];
	bool vis[maxn];
	Graph(){
		memset(edge, false, sizeof(edge));
		memset(Exi, false, sizeof(Exi));
	}
	bool Dfs(int st, int u, int len){	//search the whole graph for "odd-rings"
		vis[u] = true;
		bool f = true;
		for(int v = 0; v < maxn; v++){
			if(!edge[u][v])  continue;  
			if(v == st && !(len & 1))  return false;
			if(vis[v])  continue;
			f = f && Dfs(st, v, len+1);
		}
		return f;
	}
	bool Check(int u){  //check for "odd-rings", if it exists, the bipartite property is violated
		memset(vis, false, sizeof(vis));
		return Dfs(u, u, 0);
	}
	void InsertVertex(int index){
		if(Exi[index]){
			puts("Vertex Already Existed!\n");
			return;
		}
		Exi[index] = true;
	}
	void AddEdge(int u, int v){
		if(!Exi[u] || !Exi[v]){
			puts("Invalid Edge!\n");
			return;
		}
		if(u == v){
			puts("Invalid Edge!\n");
			return;
		}
		if(edge[u][v]){
			puts("Edge Already Existed!\n");
			return;
		}
		edge[u][v] = edge[v][u] = true;
		if(!Check(u)){  
			puts("Invalid Edge!\n");
			edge[u][v] = edge[v][u] = false;
		}
	}
	void DeleteVertex(int u){
		if(!Exi[u]){
			puts("Vertex Not Existed!\n");
			return;
		}
		Exi[u] = false;
		for(int v = 0; v < maxn; v++)
			edge[u][v] = edge[v][u] = false;
	}
	void DeleteEdge(int u, int v){
		if(!edge[u][v]){
			puts("Edge Not Existed!\n");
			return;
		}
		edge[u][v] = edge[v][u] = false;
	}
	void IncidentEdges(int u){
		for(int v = 0; v < maxn; v++){
			if(!edge[u][v])  continue;
			printf("From %d to %d\n", u, v);
		}
	}
	void Print(){
		for(int u = 0; u < maxn; u++){
			if(!Exi[u])  continue;
			printf("Origin %d:\n", u);
			IncidentEdges(u);  //determine and print all incident edges
			puts("------------");
		}
		puts("==After One Print==\n");
	}
};
int main(){
	
	//test case1:
	Graph *g1 = new Graph();
	g1->InsertVertex(1);
	g1->InsertVertex(2);
	g1->InsertVertex(7);
	g1->InsertVertex(5);
	g1->Print();
	
	g1->AddEdge(1, 5);
	g1->AddEdge(2, 7);
	g1->AddEdge(2, 5);
	g1->Print();
	
	g1->InsertVertex(3);
	g1->InsertVertex(6);
	g1->AddEdge(3, 6);
	g1->AddEdge(5, 3);
	g1->Print();
	
	g1->AddEdge(2, 3);  //Invalid edge for bipartite graph
	g1->DeleteVertex(5);
	g1->Print();
	
	g1->InsertVertex(9);
	g1->AddEdge(2, 9);
	g1->DeleteEdge(2, 7);
	g1->AddEdge(1, 6);
	g1->Print();
	
	//test case2:
	Graph *g2 = new Graph();
	g2->InsertVertex(2);
	g2->InsertVertex(3);
	g2->InsertVertex(4);
	g2->InsertVertex(1);
	g2->AddEdge(1, 2);
	g2->AddEdge(1, 4);
	g2->Print();
	
	g2->InsertVertex(5);
	g2->AddEdge(2, 4);	//Invalid edge for bipartite graph
	g2->AddEdge(2, 3);
	g2->AddEdge(2, 5);
	g2->Print();
	
	g2->DeleteVertex(4);
	g2->DeleteEdge(3, 2);
	g2->AddEdge(3, 5);
	g2->Print();

	return 0;
}
