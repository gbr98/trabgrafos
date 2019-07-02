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

void mainMenuFiller(int* alg, int* seed, int* n_test, double* alpha);

int main(int argc, char* argv[]){

  if(argc < 2){
    cout << "Formato inválido" << endl;
    return -1;
  }

  Graph* g = loader(argv[1]);

  int alg = 0, seed = 0, n_test = 0;
  double alpha = 0;

  if(argc == 2){
    mainMenuFiller(&alg, &seed, &n_test, &alpha);
  }
  if(argc >= 3){
    alg = atoi(argv[2]);
  }
  if(argc >= 4){
    seed = atoi(argv[3]);
    if(seed == 0){
      timespec ts;
      clock_gettime(CLOCK_REALTIME, &ts);
      seed = (int) ts.tv_nsec;//tempo em nanossegundos
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

/**
Teste de alguns algoritmos da parte 1 do trabalho
*/
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

/**
Permite executar qualquer um dos 3 algoritmos:
0 Guloso
1 Guloso Randomizado
2 Guloso Randomizado Reativo
Recebe um ponteiro para um Grafo (instância) e o número
do algoritmo a ser executado, além de possíveis parâmetros
adicionais, dependendo do algoritmo selecionado.
alg    -> número do algotitmo
seed   -> semente para geração de números pseudo-aleatórios
alpha  -> valor do alpha (apenas Guloso Randomizado)
n_test -> número de iterações (Randomizado e Reativo)
*/
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
    cout << minCost << " ";
  } else if(alg == 2){
    // REACTIVE RANDOMIZED GREEDY TEST
    int n_alpha = 10;
    int pos;
    double p_rand, p_sum, b_sum, a_best;
    double* best_alpha = new double[n_alpha]; //armazena o melhor custo para cada alpha
    double* p_alpha = new double[n_alpha]; //vetor de probabiliades
    vector<int> best;
    //Inicializa os valores das probabilidades dos alphas
    for(int i = 0; i < n_alpha; i++){
      alpha = (i+1)*(1.0/n_alpha);
      best = g->DS_GreedyRandomized(alpha);
      best_alpha[i] = solutionCost(g, best);
    }
    //---
    for(int i = 0; i < n_test; i++){
      //Recalcula o vetor de probabiliade de cada alpha
      pos = 0;
      for(int i = 0; i < n_alpha; i++){
        if(best_alpha[i] < best_alpha[pos])
          pos = i;
      }
      a_best = best_alpha[pos];
      b_sum = 0;
      for(pos = 0; pos < n_alpha; pos++){
        b_sum += best_alpha[pos];
      }
      for(pos = 0; pos < n_alpha; pos++){
        //função de atribuição de probabiliades para alphas
        p_alpha[pos] = (b_sum-n_alpha*a_best-best_alpha[pos]+a_best)/((n_alpha-1)*(b_sum-n_alpha*a_best));
      }
      //---
      p_rand = (rand()%100000)/100000.0;
      p_sum = p_alpha[0];
      for(pos = 0; p_rand > p_sum; pos++){
        p_sum += p_alpha[pos+1];
      }
      alpha = (pos+1)*(1.0/n_alpha);
      a_best = solutionCost(g, g->DS_GreedyRandomized(alpha));
      //substitui apenas se a solução obtida nesta iteração é melhor que a já
      //calculada para o alpha selecionado
      best_alpha[pos] = (a_best < best_alpha[pos] ? a_best : best_alpha[pos]);
    }
    pos = 0;
    for(int i = 0; i < n_alpha; i++){ //seleciona menor custo obtido & respectivo alpha
      if(best_alpha[i] < best_alpha[pos])
        pos = i;
    }
    cout << best_alpha[pos] << " " << (pos+1)*(1.0/n_alpha) << " " << n_test << " ";
  } else {
    cout << "Erro: algoritmo inválido" << endl;
  }
  auto end = chrono::steady_clock::now();
  cout << chrono::duration_cast<chrono::microseconds>(end - start).count() << endl;
}

/**
Cria um Grafo a partir de um arquivo representando uma
instância do problema (filename) e retorna um ponteiro
para este primeiro.
*/
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
    fscanf(f, "%s", r); //ignora informação sobre arestas
    fscanf(f, "%s", r);
  }
  fscanf(f, "%s", r);
  //cout << "Lendo pesos dos vértices..." << endl;
  for(int i = 0; i < n; i++){
    fscanf(f, "%lf", &w);
    g->addVertex(i, w); //ID <- posição no arquivo
  }
  fscanf(f, "%s", r);
  //cout << "Lendo arestas..." << endl;
  for(int i = 0; i < n; i++){
    for(int j = i; j < n; j++){ //como a matriz é simétrica, ignoramos
      fscanf(f, "%d", &conn);   //a diagonal inferior
      if(conn == 1 && i != j){
        g->addEdge(i, j, 1);
      }
    }
  }
  return g;
}

/**
Retorna o custo de uma solução do problema
*/
double solutionCost(Graph* g, vector<int> solution){
  double cost = 0;
  for(int i = 0; i < solution.size(); i++){
    cost += g->getVertex(solution[i])->getValue();
  }
  return cost;
}

/**
Imprime uma solução do problema
*/
void printSolution(Graph* g, vector<int> solution){
  for(int i = 0; i < solution.size(); i++){
    cout << solution[i] << " ";
  }
  cout << endl;
  cout << "Cost: " << solutionCost(g, solution) << endl;
}

/**
Apresenta um menu para que sejam passados
os parâmetros para o programa
*/
void mainMenuFiller(int* alg, int* seed, int* n_test, double* alpha){
  cout << "Qual algoritmo deseja executar?\n";
  cout << "0 - Guloso\n1 - Guloso Randomizado\n2 - Guloso Randomizado Reativo\n>> ";
  cin >> *alg;
  if(*alg > 0){
    cout << "Seed (0 -> random): ";
    cin >> *seed;
    if(*seed == 0){
      timespec ts;
      clock_gettime(CLOCK_REALTIME, &ts);
      *seed = (int) ts.tv_nsec;//tempo em nanossegundos
    }
    cout << "Número de iterações: ";
    cin >> *n_test;
    if(*alg == 1){
      cout << "alpha: ";
      cin >> *alpha;
    }
  }
}
