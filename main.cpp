#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include"Graph.h"

using namespace std;

Graph* loader(string filename);

int main(){

  Graph* g = loader("graph.txt");

  vector<int> ds = g->DS_GreedyRandomized(0.3);
  for(int i = 0; i < ds.size(); i++){
    cout << ds[i] << endl;
  }

  return 0;
}

Graph* loader(string filename){
  FILE* f = fopen(filename.c_str(), "r");
  int n, a, b;
  double w;
  Graph* g = new Graph();
  fscanf(f, "%d", &n);
  for(int i = 0; i < n; i++){
    fscanf(f, "%d %lf", &a, &w);
    g->addVertex(a, w);
  }
  while(fscanf(f, "%d %d %lf", &a, &b, &w) != EOF){
    g->addEdge(a, b, w);
  }
  return g;
}
