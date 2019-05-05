#include<iostream>
#include"Graph.h"

using namespace std;

int main(){
  
  Graph* g = new Graph();
  for(int i = 0; i < 10; i++){
    g->addVertex(i*i);
  }
  //g->printVertices();
  cout << g->getVertex(1)->getID() << endl;
  
  return 0;
}
