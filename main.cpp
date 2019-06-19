#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include"Graph.h"

using namespace std;

Graph* loader(string filename);

double solutionCost(Graph* g, vector<int> solution);

void printSolution(Graph* g, vector<int> solution);

int main(){

  Graph* g = loader("graph.txt");
  vector<int> ds;

  // GREEDY TEST
  ds = g->DS_Greedy();
  printSolution(g, ds);

  // RANDOMIZED GREEDY TEST
  ds = g->DS_GreedyRandomized(0.3);
  printSolution(g, ds);

  // REACTIVE RANDOMIZED GREEDY TEST

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

double solutionCost(Graph* g, vector<int> solution){
  double cost = 0;
  for(int i = 0; i < solution.size(); i++){
    cost += g->getVertex(solution[i])->getValue();
  }
  return cost;
}

void printSolution(Graph* g, vector<int> solution){
  for(int i = 0; i < solution.size(); i++){
    cout << solution[i] << " ";
  }
  cout << endl;
  cout << "Cost: " << solutionCost(g, solution) << endl;
}
