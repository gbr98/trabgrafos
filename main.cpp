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
  cout << g->SP_Djikstra(1, 6) << endl;

  double** matFloyd = g->SP_Floyd();
  vector<int> id = g->getVertexIDList();
  for(int i = -1; i < g->getN(); i++){
    for(int j = -1; j < g->getN(); j++){
      if(i < 0 && j < 0) cout << "\t";
      else if(i < 0) cout << "\033[1;37;41m " << id[j] << " \t\033[0m";
      else if(j < 0) cout << "\033[1;37;41m " << id[i] << " \t\033[0m";
      else cout << matFloyd[i][j] << "\t";
    }
    cout << endl;
  }

  return 0;
}

void fullTest(){
  Graph* g = loader("graph.txt");
  vector<int> ds;

  // GREEDY TEST
  ds = g->DS_Greedy();
  printSolution(g, ds);

  // RANDOMIZED GREEDY TEST
  int n = 100;
  double alpha = 0.5;
  vector<int> best = g->DS_GreedyRandomized(alpha);
  double cost, minCost = solutionCost(g, best);
  for(int i = 1; i < n; i++){
    ds = g->DS_GreedyRandomized(alpha);
    cost = solutionCost(g, ds);
    if(cost < minCost){
      minCost = cost;
      best = ds;
    }
  }
  printSolution(g, best);

  // REACTIVE RANDOMIZED GREEDY TEST

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
