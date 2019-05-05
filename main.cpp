#include<iostream>
#include"Graph.h"

using namespace std;

int main(){
  
  Graph* g = new Graph();
  for(int i = 0; i < 10; i++){
    g->addVertex(i*i);
  }
  for(int i = 0; i < 9; i++){
    g->addEdge(i*i, (i+1)*(i+1), i/2.0);
  }
  g->printVertices();
  
  return 0;
}
