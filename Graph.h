#include <vector>
#include "Vertex.h"
#include "Edge.h"

#ifndef GRAPH_H
#define GRAPH_H

using namespace std;

class Graph {

  private:
  Vertex* rootVertex;
  int n, m;
  bool directed;

  public:
  Graph();
  Graph(bool directed);
  int addVertex(int ID); //return ID of added vertex
  int addVertex(int ID, double value);
  bool removeVertex(int ID);
  Vertex* getVertex(int ID);
  Vertex* getRootVertex();
  bool addEdge(int ID1, int ID2, double value); //return true if added; false, ow
  bool removeEdge(int ID1, int ID2);
  bool isNull();
  bool isEmpty();
  void printVertices();

  Graph* inverse();
  void DFS(Vertex* v);
  int* BFS(int ID);
  int* TSorting();

  vector<int> DS_Greedy();
  vector<int> DS_GreedyRandomized(float alpha);
};

#endif
