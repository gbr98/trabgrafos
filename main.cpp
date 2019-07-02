#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<chrono>
#include<sys/time.h>
#include"Graph.h"

using namespace std;

Graph* loader(string filename);

void P1Test(Graph* g);

void fullTest(Graph* g, int alg, int seed, double alpha, int n_test);

double solutionCost(Graph* g, vector<int> solution);

void printSolution(Graph* g, vector<int> solution);

int main(int argc, char* argv[]){

  if(argc < 2){
    cout << "Formato inválido" << endl;
    return -1;
  }

  Graph* g = loader(argv[1]);

  int alg = 0, seed = 0, n_test = 0;
  double alpha = 0;
  if(argc >= 3){
    alg = atoi(argv[2]);
  }
  if(argc >= 4){
    seed = atoi(argv[3]);
    if(seed == 0){
      timespec ts;
      clock_gettime(CLOCK_REALTIME, &ts);
      seed = (int) ts.tv_nsec;//time(NULL);
    }
  }
  if(argc >= 5){
    alpha = atof(argv[4]);
  }
  if(argc >= 6){
    n_test = atoi(argv[5]);
  }

  fullTest(g, alg, seed, alpha, n_test);

  return 0;
}

void P1Test(Graph* g){
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
}

void fullTest(Graph* g, int alg, int seed, double alpha, int n_test){
  auto start = chrono::steady_clock::now();
  vector<int> ds;
  cout << seed << " ";
  srand(seed);

  if(alg == 0){
    // GREEDY TEST
    ds = g->DS_Greedy();
    cout << solutionCost(g, ds) << " ";
  } else if(alg == 1){
    // RANDOMIZED GREEDY TEST
    vector<int> best = g->DS_GreedyRandomized(alpha);
    double cost, minCost = solutionCost(g, best);
    for(int i = 1; i < n_test; i++){
      ds = g->DS_GreedyRandomized(alpha);
      cost = solutionCost(g, ds);
      if(cost < minCost){
        minCost = cost;
        best = ds;
      }
    }
    //printSolution(g, best);
    cout << minCost << " ";
  } else if(alg == 2){
    // REACTIVE RANDOMIZED GREEDY TEST
    int n_alpha = 10;
    int pos;
    double p_rand, p_sum, b_sum, a_best;
    double* best_alpha = new double[n_alpha];
    double* p_alpha = new double[n_alpha];
    vector<int> best;
    /*for(int i = 0; i < n_alpha; i++){
      p_alpha[i] = 1.0/n_alpha;
    }*/
    //first run for all values of alpha
    for(int i = 0; i < n_alpha; i++){
      alpha = (i+1)*(1.0/n_alpha);
      best = g->DS_GreedyRandomized(alpha);
      best_alpha[i] = solutionCost(g, best);
    }
    //---
    for(int i = 0; i < n_test; i++){
      //recalculate prob. vector
      b_sum = 0;
      for(pos = 0; pos < n_alpha; pos++){
        b_sum += best_alpha[pos];
      }
      for(pos = 0; pos < n_alpha; pos++){
        p_alpha[pos] = (b_sum-best_alpha[pos])/((n_alpha-1)*b_sum);
      }
      //---
      p_rand = (rand()%100000)/100000.0;
      p_sum = p_alpha[0];
      for(pos = 0; p_rand > p_sum; pos++){
        p_sum += p_alpha[pos+1];
      }
      alpha = (pos+1)*(1.0/n_alpha);
      a_best = solutionCost(g, g->DS_GreedyRandomized(alpha));
      best_alpha[pos] = (a_best < best_alpha[pos] ? a_best : best_alpha[pos]);
    }
    pos = 0;
    /*for(int i = 0; i < n_alpha; i++){
      if(best_alpha[i] < best_alpha[pos])
        pos = i;
      cout << best_alpha[i] << " ";
    }
    cout << endl;*/
    cout << best_alpha[pos] << " " << (pos+1)*(1.0/n_alpha) << " " << n_test << " ";
  } else {
    cout << "Erro: algoritmo inválido" << endl;
  }
  auto end = chrono::steady_clock::now();
  cout << chrono::duration_cast<chrono::microseconds>(end - start).count() << endl;
}

Graph* loader(string filename){
  //cout << "Iniciando leitura..." << endl;
  FILE* f = fopen(filename.c_str(), "r");
  int n, conn;
  double w;
  char r[100];
  Graph* g = new Graph();
  fscanf(f, "%s", r);
  fscanf(f, "%d", &n);
  fscanf(f, "%s", r);
  //cout << "Pulando posições..." << endl;
  for(int i = 0; i < n; i++){
    fscanf(f, "%s", r); //ignoring positions
    fscanf(f, "%s", r);
  }
  fscanf(f, "%s", r);
  //cout << "Lendo pesos dos vértices..." << endl;
  for(int i = 0; i < n; i++){
    fscanf(f, "%lf", &w);
    g->addVertex(i, w); //ID = position
  }
  fscanf(f, "%s", r);
  //cout << "Lendo arestas..." << endl;
  for(int i = 0; i < n; i++){
    for(int j = i; j < n; j++){ //simetrical matrix
      fscanf(f, "%d", &conn);
      if(conn == 1 && i != j){
        g->addEdge(i, j, 1);
      }
    }
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
