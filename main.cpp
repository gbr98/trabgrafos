#include<iostream>
#include<vector>
#include"Graph.h"

using namespace std;

int main(){
  
  Graph* g = new Graph();
  
  g->addVertex(1, 2);
  g->addVertex(2, 4);
  g->addVertex(3, 3);
  g->addVertex(4, 5);
  g->addVertex(5, 7);
  g->addVertex(6, 8);
  g->addVertex(7, 1);
  
  g->addEdge(1, 2, 0);
  g->addEdge(2, 4, 0);
  g->addEdge(3, 4, 0);
  g->addEdge(4, 7, 0);
  g->addEdge(5, 6, 0);
  g->addEdge(6, 7, 0);
  
  vector<int> ds = g->DS_Greedy();
  for(int i = 0; i < ds.size(); i++){
    cout << ds[i] << endl;
  }
  
  return 0;
}
