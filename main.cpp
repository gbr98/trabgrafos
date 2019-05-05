#include<iostream>

using namespace std;

int main(){
  
  Graph g = new Graph();
  for(int i = 0; i < 10; i++){
    g.addVertex(i*i);
  }
  
  return 0;
}
