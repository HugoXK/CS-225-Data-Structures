//
//  graph.cpp
//  
//
//  Created by KD on 11.04.21.
//

#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;
#include "graph.h"
using std::cout;


/* The structure of a vertex is analogous to a node in a doubly linked list. In addition we keep an "edgelist" with each vertex, i.e. the list of neighbour vertices. */
template<class T> vertex<T>::vertex(T item, vertex<T> *pt_next, vertex<T> *pt_prev)
{
    vertexid = item;
    next = pt_next;
    previous = pt_prev;
    edgelist = 0;
}

template<class T> int vertex<T>::numoutedges(void)
{
    if (edgelist != 0)
        return (*edgelist).getlength();
    return 0;
}

template<class T> T vertex<T>::getitem(void)
{
    return vertexid;
}

template<class T> vertex<T> * vertex<T>::getnext(void)
{
    return next;
}

template<class T> vertex<T> * vertex<T>::getprevious(void)
{
    return previous;
}

template<class T> elist<T> * vertex<T>::getedges(void)
{
    return edgelist;
}

template<class T> void vertex<T>::setitem(T item)
{
    vertexid = item;
    return;
}

template<class T> void vertex<T>::setnext(vertex<T> *pt_next)
{
    next = pt_next;
    return;
}

template<class T> void vertex<T>::setprevious(vertex<T> *pt_prev)
{
    previous = pt_prev;
    return;
}

template<class T> void vertex<T>::setedges(elist<T> *pt_edges)
{
    edgelist = pt_edges;
    return;
}

/* A vertexlist is a doubly linked list with vertices and a dummy node. */
template<class T> vertexlist<T>::vertexlist(void)
{
    dummy = new vertex<T>;
    (*dummy).setnext(dummy);
    (*dummy).setprevious(dummy);
    numvertices = 0;
}

template<class T> int vertexlist<T>::getlength(void)
{
    return numvertices;
}

/* For counting edges we must add up the counts of outgoing edges for each vertex */
template<class T> int vertexlist<T>::countedges(void)
{
    int count = 0;
    vertex<T> * pt = (*dummy).getnext();
    for (int i = 0; i < numvertices; i++)
    {
        count = count + (*pt).numoutedges();
        pt = (*pt).getnext();
    }
    return count;
}

/* append is analogous to linked lists, i.e. we add a new vertex at the end and adjust pointers. */
template<class T> void vertexlist<T>::append(T item)
{
    vertex<T> * pt_last = (*dummy).getprevious();
    vertex<T> * pt_new = new vertex<T>;
    (*pt_new).setitem(item);
    (*pt_new).setnext(dummy);
    (*pt_new).setprevious(pt_last);
    (*dummy).setprevious(pt_new);
    (*pt_last).setnext(pt_new);
    ++numvertices;
    return;
}

/* A new vertex is only added, if it is not yet in the list. */
template<class T> void vertexlist<T>::addvertex(T item)
{
    if (contains(item) == false)
        append(item);
    return;
}

/* To check if a vertex is already in the list the list is scanned until the element is found. */
template<class T> bool vertexlist<T>::contains(T item)
{
    bool result = false;
    vertex<T> * pt = (*dummy).getnext();
    for (int i = 0; i < numvertices; i++)
    {
        if ((*pt).getitem() == item)
        {
            result = true;
            break;
        }
        else
            pt = (*pt).getnext();
    }
    return result;
}

/* In order to remove a vertex we have to find it in the list of vertices. If it has a non-empty list of neighbours, it cannot be deleted. It cannot be deleted, if it appwars in any edgelist. */
template<class T> bool vertexlist<T>::remove(T item)
{
    bool result = false;
    vertex<T> * location = 0;
    vertex<T> *pt = (*dummy).getnext();
    for (int i = 0; i < numvertices; i++)
    {
        if ((*pt).getitem() == item)
        {
            if ((*pt).getedges() != 0)
            {
                cout << "The vertex " << item << " cannot be deleted, as it appears in an edge.\n";
                return result;
            }
            /* We may be able to delete the vertex at location pt, but we still have to check other edgelists. Therefore pt is stored in location. */
            location = pt;
            continue;
        }
        elist<T> * edges = (*pt).getedges();
        if (edges != 0 && (*edges).contains(item) == true)
            /* it is appears in an edgelist, there is no delete. */
        {
            cout << "The vertex " << item << " cannot be deleted, as it appears in an edge.\n";
            return result;
        }
        pt = (*pt).getnext();
    }
    /* Finally, we get to the deletion from the veertexlist. */
    if (location != 0)
    {
        result = true;
        vertex<T> * pt_prev = (*location).getprevious();
        vertex<T> * pt_next = (*location).getnext();
        (*pt_prev).setnext(pt_next);
        (*pt_next).setprevious(pt_prev);
        --numvertices;
    }
    /* The returned Boolean result indicates, if the vertex was deleted or not. */
    return result;
}

/* Adding an edge is done by first locating the origin vertex in the vertex list, then inserting the destination vertex into the associated edgelist. */
template<class T> void vertexlist<T>::addedge(T first, T second)
{
    vertex<T> * pt = (*dummy).getnext();
    for (int i = 0; i < numvertices; i++)
    {
        if ((*pt).getitem() == first)
        {
            elist<T> * pt_edges = (*pt).getedges();
            if (pt_edges != 0)
                (*pt_edges).addvertex(second);
            else
            {
                elist<T> * newlist = new elist<T>(second);
                (*pt).setedges(newlist);
            }
            return;
        }
        pt = (*pt).getnext();
    }
    /* If the origin vertex is not yet in the vertex list, it is first appended to the vertex list. The associated edgelist contains a single vertex. */
    append(first);
    pt = (*dummy).getprevious();
    elist<T> * newlist = new elist<T>(second);
    (*pt).setedges(newlist);
    return;
}

/* Removing an edge is done by first locating the origin vertex in the vertex list, then searching for the destination vertex into the associated edgelist. In this case the edge is always deleted, if it is found. */
template<class T> void vertexlist<T>::removeedge(T first, T second)
{
    vertex<T> * pt = (*dummy).getnext();
    for (int i = 0; i < numvertices; i++)
    {
        if ((*pt).getitem() == first)
        {
            elist<T> * pt_edges = (*pt).getedges();
            if (pt_edges != 0)
            {
                (*pt_edges).remove(second);
                if ((*pt_edges).getlength() == 0)
                    (*pt).setedges(0);
            }
            return;
        }
        pt = (*pt).getnext();
    }
    return;
}

/* To build list of outgoing edges for a given vertex, a new edge sequence is created for the output, then by scanning the associated edgelist edges (pairs of vertices) are added one by one. */
template<class T> edgelist<T> * vertexlist<T>::outgoingedges(T item)
{
    edgelist<T> * result = new edgelist<T>;
    vertex<T> * ptv = (*dummy).getnext();
    for (int i = 0; i < numvertices; i++)
    {
        if ((*ptv).getitem() == item)
        {
            elist<T> * edges = (*ptv).getedges();
            if (edges != 0)
                (*edges).addall(item, result);
            return result;
        }
        ptv = (*ptv).getnext();
    }
    return result;
}

/* only for testing */
template<class T> void vertexlist<T>::prettyprint(void)
{
    vertex<T> *ptv = (*dummy).getnext();
    for (int i = 0; i < numvertices; i++)
    {
        cout << "Edges with origin " << (*ptv).getitem() << " :\n";
        edgelist<T> * pte = outgoingedges((*ptv).getitem());
        (*pte).prettyprint();
        ptv = (*ptv).getnext();
    }
    return;
}

/* The edge list for each vertex is organised analogous to the vertexlist, but the are no assiciated edgelists. The elements of the list are organised by the neighbour class. */
template<class T> neighbour<T>::neighbour(T item, neighbour<T> *pt_next, neighbour<T> *pt_prev)
{
    secondvertex = item;
    next = pt_next;
    previous = pt_prev;
}

template<class T> T neighbour<T>::getitem(void)
{
    return secondvertex;
}

template<class T> neighbour<T> * neighbour<T>::getnext(void)
{
    return next;
}

template<class T> neighbour<T> * neighbour<T>::getprevious(void)
{
    return previous;
}

template<class T> void neighbour<T>::setitem(T item)
{
    secondvertex = item;
    return;
}

template<class T> void neighbour<T>::setnext(neighbour<T> * pt)
{
    next = pt;
    return;
}

template<class T> void neighbour<T>::setprevious(neighbour<T> * pt)
{
    previous = pt;
    return;
}

/* elist is the class used for the representation of doubly linked lists of vertices (no dummy node) associated with the vertices in the vertexlist. All functions are analogous to those defined for vertexlist. */
template<class T> elist<T>::elist(T item)
{
    first = new neighbour<T>;
    (*first).setitem(item);
    length = 1;
    (*first).setnext(first);
    (*first).setprevious(first);
}

template<class T> int elist<T>::getlength(void)
{
    return length;
}

template<class T> void elist<T>::append(T item)
{
    neighbour<T> * pt_last = (*first).getprevious();
    neighbour<T> * pt_new = new neighbour<T>;
    (*pt_new).setitem(item);
    (*pt_new).setnext(first);
    (*pt_new).setprevious(pt_last);
    (*first).setprevious(pt_new);
    (*pt_last).setnext(pt_new);
    ++length;
    return;
}

template<class T> void elist<T>::addvertex(T item)
{
    if (contains(item) == false)
        append(item);
    return;
}

template<class T> bool elist<T>::contains(T item)
{
    bool result = false;
    neighbour<T> * pt = first;
    for (int i = 0; i < length; i++)
    {
        if ((*pt).getitem() == item)
        {
            result = true;
            break;
        }
        else
            pt = (*pt).getnext();
    }
    return result;
}

template<class T> void elist<T>::remove(T item)
{
    neighbour<T> * pt = first;
    for (int i = 0; i < length; i++)
    {
        if ((*pt).getitem() == item)
        {
            neighbour<T> * pt_prev = (*pt).getprevious();
            neighbour<T> * pt_next = (*pt).getnext();
            if (i == 0){
                first = pt_next;
            } 
            (*pt_prev).setnext(pt_next);
            (*pt_next).setprevious(pt_prev);
            --length;
            return;
        }
        pt = (*pt).getnext();
    }
    return;
}
/* The function addall adds all pairs with the given item and the list members, i.e. all outgoing edges to the list given by the pointer pt. It exploit the function add on edgelists. */
template<class T> void elist<T>::addall(T item, edgelist<T> *pt)
{
    neighbour<T> * pte = first;
    for (int i = 0; i < length; i++)
    {
        (*pt).add(item, (*pte).getitem());
        pte = (*pte).getnext();
    }
    return;
}

/* Edges are simply pairs of vertices with obvious get and set functions. */
template<class T> edge<T>::edge(T first, T second)
{
    firstitem = first;
    seconditem = second;
}

template<class T> T edge<T>::origin(void)
{
    return firstitem;
}

template<class T> T edge<T>::destination(void)
{
    return seconditem;
}

template<class T> void edge<T>::setorigin(T item)
{
    firstitem = item;
    return;
}

template<class T> void edge<T>::setdestination(T item)
{
    seconditem = item;
    return;
}

/* only for testing */
template<class T> void edge<T>::prettyprint(void)
{
    cout << "(origin: " << origin() << ", destination: " << destination() << ") + ";
    return;
}

/* edgelists, i.e. sequences of pairs of vertices are only used for output purposes. */
template<class T> edgelist<T>::edgelist(void)
{
    reprarray = new edge<T>*[100];
    numedges = 0;
}

template<class T> int edgelist<T>::getnumedges(void)
{
    return numedges;
}

template<class T> void edgelist<T>::add(T item1, T item2)
{
    edge<T> * newedge = new edge<T>(item1,item2);
    reprarray[numedges] = newedge;
    ++numedges;
    return;
}

template<class T> void edgelist<T>::prettyprint(void)
{
    for (int i = 0; i < numedges; i++)
    {
        edge<T> * pte = reprarray[i];
        (*pte).prettyprint();
    }
    cout << "\n      --------\n";
    return;
}

/* A graph object just contains a pointer to a vertexlist. */
template<class T> graph<T>::graph(void)
{
    vertices = new vertexlist<T>;
}

template<class T> int graph<T>::numvertices(void)
{
    return (*vertices).getlength();
}

template<class T> int graph<T>::numedges(void)
{
    return (*vertices).countedges();
}

template<class T> void graph<T>::addedge(T origin, T destination)
{
    (*vertices).addedge(origin, destination);
    (*vertices).addedge(destination, origin);
    return;
}

template<class T> void graph<T>::addvertex(T item)
{
    (*vertices).addvertex(item);
    return;
}

template<class T> void graph<T>::deleteedge(T origin, T destination)
{
    (*vertices).removeedge(origin, destination);
    (*vertices).removeedge(destination, origin);
    return;
}

template<class T> void graph<T>::deletevertex(T item)
{
    (*vertices).remove(item);
    return;
}

template<class T> edgelist<T> * graph<T>::outgoingedges(T item)
{
    return (*vertices).outgoingedges(item);
}

template<class T> void graph<T>::prettyprint(void)
{
    cout << "\n\n====== Graph ======\n\n";
    (*vertices).prettyprint();
    cout << "=================\n\n";
    return;
}

//folowing function is added for hw8-ex3
template<class T> bool graph<T>::is_perfect_matching(){
    vector<int> boys;  
    vector<int> girls;
    for(int i = 1; i<=vertices->getlength();i++){
        vertex<T> temp_vertex = *(vertices->getvertex(i)); //get data from the vertex list;
        if(temp_vertex.getitem()/2*2 == temp_vertex.getitem()){
            boys.push_back(temp_vertex.getitem()); //if id is even, then push into boys set
        }else{
            girls.push_back(temp_vertex.getitem());  //if id is odd, then push into girls set
        }
    }
    edgelist<T> E;  //push all edges into the edgelist E(unordered)
    for(int i = 1; i<=vertices->getlength();i++){
        edgelist<T>* temp_lists = new edgelist<T>;
        temp_lists = vertices->outgoingedges(vertices->getvertex(i)->getitem()); //get every vertex' outgoing edges
        for(int i = 1; i<=temp_lists->getnumedges();i++){
            if(E.contain_pairs(temp_lists->getitem(i)->origin(),temp_lists->getitem(i)->destination())==false){ //if not stored
                E.add(temp_lists->getitem(i)->origin(),temp_lists->getitem(i)->destination());//store in E
            }
        }
    }
    edgelist<T> partial_match;
    edgelist<T> di_graph;
    edgelist<T> path;
    bool Halt = false;
    bool Output = false;
    int mode = 1;
    while (Halt == false){  //1.init 2.examine 3.build-digraph 4.build-path 5.modify
        switch(mode){
            case 1://init
            {
                mode = 2;
                break;
            }
            case 2://examine
            {
                bool judge = false;
                for(int i = 0; i<boys.size(); i++){
                    int count = 0;
                    int b = boys[i];   //for every boy
                    for(int m = 0; m<girls.size();m++){
                        int g = girls[m];
                        if (partial_match.contain_pairs(b,g)==true){
                            count++;    //if exist a girl where they match each other,then increase count by one
                        }
                    }
                    if(count == 0){
                        judge = true;//if exist a boy can't find pair
                        break;
                    }
                }
                if(judge){
                    mode = 3;
                }else{
                    Output = true;
                    Halt = true;
                    mode = 0;
                }
                break;
            }
            case 3://build-digraph
            {
                if(di_graph.getnumedges()!=0){
                    int length = di_graph.getnumedges();
                    for(int i = 1; i<=length;i++){
                        di_graph.remove_item(*(di_graph.getitem(1)));
                    }
                }
                edgelist<T> el_1 ;
                for(int i = 1; i<=partial_match.getnumedges();i++){
                    el_1.add(partial_match.getitem(i)->origin(),partial_match.getitem(i)->destination());
                }
                el_1.girls_to_boys();//calculate girls to boys(partial match)
                edgelist<T> el_2 ;
                for(int i = 1; i<=partial_match.getnumedges();i++){
                    el_2.add(partial_match.getitem(i)->origin(),partial_match.getitem(i)->destination());
                }
                edgelist<T> E_copy ;
                for(int i = 1; i<=E.getnumedges();i++){
                    E_copy.add(E.getitem(i)->origin(),E.getitem(i)->destination());
                }
                //compute E-Partial_match
                for(int i = 1; i<=el_2.getnumedges();i++){
                    E_copy.remove_item(*el_2.getitem(i));
                }
                E_copy.boys_to_girls(); //calculate boys to girls(E − partial match)
                for(int i = 1; i<=el_1.getnumedges();i++){
                    di_graph.add(el_1.getitem(i)->origin(),el_1.getitem(i)->destination());
                }
                for(int i = 1; i<=E_copy.getnumedges();i++){
                    if(di_graph.contain_pairs(E_copy.getitem(i)->origin(),E_copy.getitem(i)->destination())==false){
                    di_graph.add(E_copy.getitem(i)->origin(),E_copy.getitem(i)->destination());
                    }
                }
                mode = 4;
                break;
            }
            case 4://build-path
            {
                vector<int> x;
                for(int i = 0; i<boys.size(); i++){
                    int count = 0;
                    int b = boys[i];
                    for(int h = 0; h<girls.size();h++){
                        int g = girls[h];
                        if (partial_match.contain_pairs(b,g)==true){
                            count++;
                        }
                    }
                    if(count == 0){
                        x.push_back(b);
                    }
                }
                if(x.size()>0){
                    int chosen_boy = x[0];//find a boy who doesn't have mate
                    vector<int> y;
                    for(int n = 0; n<girls.size(); n++){
                        int count = 0;
                        int gg = girls[n];
                        for(int m = 0; m<boys.size();m++){
                            int bb = boys[m];
                            if (partial_match.contain_pairs(gg,bb)==true){
                                count++;
                            }
                        }
						
						if((count == 0) && (PATH(chosen_boy,di_graph,gg).getnumedges()!=0)){
                            y.push_back(gg); //find a girl who doesn't have mate and have connection with the boy
                        }
                    }
                    if(y.size()>0){
                        path = PATH(chosen_boy,di_graph,y[0]);    //find the path between boy and girl
                        mode = 5;
                    }else{
                        Output = false;
                        Halt = true;
                        mode = 0;
                    }
                    
                }
                break;
            }
            case 5://modify
            {
                edgelist<T> el_3;
                for(int i = 1; i<=partial_match.getnumedges();i++){
                    el_3.add(partial_match.getitem(i)->origin(),partial_match.getitem(i)->destination());
                }
                for(int i = 1; i <= path.getnumedges();i++){
                    if(partial_match.contain_pairs(path.getitem(i)->origin(),path.getitem(i)->destination())){
                        el_3.remove_item(*path.getitem(i));
                    }
                }
                for(int i = 1; i <= partial_match.getnumedges();i++){
                    if(path.contain_pairs(partial_match.getitem(i)->origin(),partial_match.getitem(i)->destination())){
                        path.remove_item(*partial_match.getitem(i));
                    }
                }
                edgelist<T> el_4;
                // (partial match − unordered(path))∪(unordered(path) − partial match)
                for(int i = 1; i<=el_3.getnumedges();i++){
                    el_4.add(el_3.getitem(i)->origin(),el_3.getitem(i)->destination());
                }
                for(int i = 1; i<=path.getnumedges();i++){
                    if(el_4.contain_pairs(path.getitem(i)->origin(),path.getitem(i)->destination())==false){
                        el_4.add(path.getitem(i)->origin(),path.getitem(i)->destination());
                    }
                }
                partial_match = el_4;
                mode = 2;
                break;
            }
        }
    }
    return Output;
}

//helpful functions added for ex3
template<class T> vertex<T>* vertexlist<T> ::getvertex(int index){
    vertex<T>* tempvertex = new vertex<T>;  // get the vertex 
    tempvertex = dummy;
    if(numvertices!=0){
        tempvertex = tempvertex->getnext();
    }
    for(int i = 1; i <= numvertices; i++){
        if (i!=index){
            tempvertex = tempvertex->getnext();
        }else{
            return tempvertex;
        }
    }
    return tempvertex;
}

template<class T> bool edgelist<T>::contain_pairs(T item1, T item2)
{
    if (numedges == 0){ //check whether the pair is in the set
        return false;
    }
    for(int i = 1; i <= numedges; i++){
        if (((getitem(i)->origin() == item1) && (getitem(i)->destination()== item2)) || ((getitem(i)->origin() == item2) && (getitem(i)->destination()==item1))){
            return true;
        }
    }
    return false;
}

template<class T> edge<T> * edgelist<T>::getitem(int index)
{
    return reprarray[index-1];  //get the item with index i
}

template<class T> void edgelist<T>:: boys_to_girls(void)
{
    for(int i = 1; i<=numedges;i++){    //turn undirected set to directed set
        if(getitem(i)->origin()/2*2==getitem(i)->origin()){
            continue;
        }else{
            T new_origin = getitem(i)->destination();
            T new_dest = getitem(i)->origin();
            getitem(i)->setorigin(new_origin);
            getitem(i)->setdestination(new_dest);
        }
    }
}

template<class T> void edgelist<T>:: girls_to_boys(void)
{
    for(int i = 1; i<=numedges;i++){    //turn the undirected set to directed set
        if(getitem(i)->origin()/2*2==getitem(i)->origin()){
            T new_origin = getitem(i)->destination();
            T new_dest = getitem(i)->origin();
            getitem(i)->setorigin(new_origin);
            getitem(i)->setdestination(new_dest);
        }else{
            continue;
        }
    }

}

template<class T> void edgelist<T>::remove_item(edge<T> item){
    for(int i = 1; i<=numedges;i++){    //remove the item
        if ((getitem(i)->origin()==item.origin() && getitem(i)->destination()==item.destination()) || (getitem(i)->origin()==item.destination() && getitem(i)->destination()==item.origin())){
            for (int m = i; m <= numedges-1; m++){
                reprarray[m-1] = reprarray[m];
            }
            numedges--;
            break;
        }
    }
}

template<class T> bool graph<T>::reachable(T boy, edgelist<T>di_graph, T girl)
{
    if(PATH(boy,di_graph,girl).getnumedges()==0){
        return false;
    }
    return true;

}

template<class T> edgelist<T> graph<T>::PATH(T boy, edgelist<T>di_graph, T girl)
{
   edgelist<T> connect_path;
   for(int i = 1; i<=di_graph.getnumedges();i++){
       if(di_graph.getitem(i)->origin()==boy && di_graph.getitem(i)->destination()==girl){
           connect_path.add(boy,girl);
           return connect_path;
       }
    }
    //main procedure follow what we do in ECE120
    vector<T> queue;    //the element in queue can never overlap
    vector<T> previous; //store the parent element
    vector<T> back_track_list;  //store the back track
    int explored = 0;   //mark the explored element
    queue.push_back(boy);
    previous.push_back(-1); //set the start's previous to be -1
    bool judge = false;
    while(true){
        T temp_explore = queue[explored];
        for(int i = 1; i<=di_graph.getnumedges();i++){
            if(di_graph.getitem(i)->origin()==temp_explore){
                T desti = di_graph.getitem(i)->destination();
                bool judge_1 = true;
                for(int m = 0; m<queue.size();m++){ //check whether the destination is already in the queue
                    if (queue[m]==desti){
                        judge_1 = false;
                        break;
                    }
                }
                if(judge_1){
                    queue.push_back(di_graph.getitem(i)->destination());
                    previous.push_back(temp_explore);
                }else{
                    continue;
                }
                if (di_graph.getitem(i)->destination()==girl){
                    judge = true;
                    break;
                }
            }
        }
        if(judge){
            break;
        }
        explored++;
        if(explored >= queue.size()){
            break;
        }
    }

    if (judge==false){  //if can't find such path
        return connect_path;
    }

    back_track_list.push_back(queue.back());
    T element = queue.back();
    T pre_ele = previous.back();
    // int track = queue.size()-1;
    while(true){
        if(pre_ele != -1){
            back_track_list.push_back(pre_ele);
        }
        if(pre_ele == boy){
            break;
        }
        for(int i = queue.size()-1; i >=0; i--){
            if (queue[i]==pre_ele){
                pre_ele = previous[i];
                break;
            }
        }
    }
    for(int i = back_track_list.size()-1; i>=1;i--){
        connect_path.add(back_track_list[i],back_track_list[i-1]);
    }

   return connect_path;
}
