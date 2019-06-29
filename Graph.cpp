#include "Graph.h"
#include <iostream>
#include <queue>
#include <stack>
#include <algorithm>
#include <limits>
#include <math.h>

Graph::Graph(){
  this->n = 0;
  this->m = 0;
  this->rootVertex = NULL;
  this->directed = false;
}

Graph::Graph(bool directed){
  this->n = 0;
  this->m = 0;
  this->rootVertex = NULL;
  this->directed = directed;
}

int Graph::addVertex(int ID){
  return this->addVertex(ID, 0);
}

int Graph::addVertex(int ID, double value){
  Vertex* v = new Vertex(ID, value);
  v->setNext(this->rootVertex);
  this->rootVertex = v;
  this->n += 1;
}

bool Graph::removeVertex(int ID){
  if(this->rootVertex == NULL){
    //nothing to remove
    return false;
  } else {
    Vertex* a = NULL;
    Vertex* p = this->rootVertex;
    while(p != NULL){
      if(p->getID() == ID){
        //remove vertex
        if(a == NULL){
          this->rootVertex = p->getNext();
        } else {
          a->setNext(p->getNext());
        }
        p = NULL;
        this->n -= 1;
        return true;
      }
      a = p;
      p = p->getNext();
    }
    //vertex wasn't found
    return false;
  }
}

Vertex* Graph::getVertex(int ID){
  if(this->rootVertex == NULL){
    return NULL;
  } else {
    Vertex* p = this->rootVertex;
    while(p != NULL){
      if(p->getID() == ID){
        return p;
      }
      p = p->getNext();
    }
    return NULL;
  }
}

Vertex* Graph::getRootVertex(){
  return this->rootVertex;
}

double Graph::getEdgeValue(int ID1, int ID2){
  Vertex* a = this->getVertex(ID1);
  Edge* e = this->getVertex(ID1)->getRootEdge();
  while(e != NULL){
    if(e->getVertexID() == ID2){
      return e->getValue();
    }
    e = e->getNext();
  }
  return 0; //returns 0 if there's no edge between the
            //specified vertices (ID1, ID2);
}

bool Graph::addEdge(int ID1, int ID2, double value){
  Vertex* a = this->getVertex(ID1);
  Vertex* b = this->getVertex(ID2);
  if(a != NULL && b != NULL){
    if(this->directed){
      a->connectEdge(ID2, value);
      a->addOutDeg();
      b->addInDeg();
    } else {
      a->connectEdge(ID2, value);
      b->connectEdge(ID1, value);
    }
    this->m += 1;
    return true;
  } else {
    return false;
  }
}

bool Graph::removeEdge(int ID1, int ID2){
  Vertex* a = this->getVertex(ID1);
  Vertex* b = this->getVertex(ID2);
  if(a != NULL && b != NULL){
    if(a->removeEdge(ID2)){
      //if removed edge successfully
      if(this->directed){
        a->remOutDeg();
        b->remInDeg();
        this->m -= 1;
        return true;
      } else {
        if(b->removeEdge(ID1)){
          this->m -= 1;
          return true;
        } else {
          return false;
        }
      }
    } else {
      return false;
    }
  } else {
    return false;
  }
}

int Graph::getN(){
  return this->n;
}

int Graph::getM(){
  return this->m;
}

bool Graph::isNull(){
  return n == 0;
}

bool Graph::isEmpty(){
  return m == 0;
}

void Graph::printVertices(){
  Vertex* p = this->rootVertex;
  Edge* e;
  while(p != NULL){
    std::cout << "[" << p->getID() << "] : ";
    e = p->getRootEdge();
    while(e != NULL){
      std::cout << e->getVertexID() << "(" << e->getValue() << ") ";
      e = e->getNext();
    }
    std::cout << std::endl;
    p = p->getNext();
  }
}

vector<int> Graph::getVertexIDList(){
  vector<int> ids;
  Vertex* p = this->rootVertex;
  while(p != NULL){
    ids.push_back(p->getID());
    p = p->getNext();
  }
  return ids;
}

Graph* Graph::inverse(){
  Graph* g = new Graph();
  Vertex* p = this->rootVertex;
  Vertex* q;
  Edge* e;
  while(p != NULL){
    g->addVertex(p->getID(), p->getValue());
    p = p->getNext();
  }
  p = this->rootVertex;
  while(p != NULL){
    q = this->rootVertex;
    while(q != NULL){
      if(this->directed){
        if(p != q)
          g->addEdge(p->getID(), q->getID(), 0);
      } else {
        if(p == q) break;
        g->addEdge(p->getID(), q->getID(), 0);
      }
      q = q->getNext();
    }
    e = p->getRootEdge();
    while(e != NULL){
      g->removeEdge(p->getID(), e->getVertexID());
      e = e->getNext();
    }
    p = p->getNext();
  }
  return g;
}

void Graph::DFS(Vertex* v){
  stack<int> stck;
  bool visited[10];

  for (int i=0; i<10; i++){
    visited[i] = false;
    cout<<visited[i];
  }
  while (true){
    if (!visited[v->getID()]){
      cout << "Visitando vertice "<<v->getID()<<"...\n";
      visited[v->getID()] = true;
      stck.push(v->getID());
    }
    bool found = false;
    Edge *e = v->getRootEdge();
    while (e != NULL){
      v = getVertex(e->getVertexID());
      if (visited[v->getID()]== false){
        found = true;
        break;
      }
      e = e->getNext();
    }
    if (found == true)
      v = getVertex(v->getID());
    else {
      stck.pop();
      if (stck.empty())
        break;
      v = getVertex(stck.top());
    }
  }
}

int* Graph::BFS(int ID){
  int* v = new int[this->n];
  int counter = 0;
  queue<int> queue;
  queue.push(ID);
  Vertex* head;
  Edge* e;
  //set color of each vertex to white (0)
  Vertex* p = this->rootVertex;
  while(p != NULL){
    p->setColor(0);
    p = p->getNext();
  }
  //---
  while(!queue.empty()){
    //get head of the queue
    head = this->getVertex(queue.front());
    queue.pop();
    head->setColor(1);
    v[counter] = head->getID(); counter++;
    //---
    //for each adj. vertex, push to the queue
    e = head->getRootEdge();
    while(e != NULL){
      head->setColor(1);
      queue.push(e->getVertexID());
      e = e->getNext();
    }
    //---
  }
  return v;
}

int* Graph::TSorting(){
  if(!this->directed) return NULL; //nothing to do
  //make a copy of the graph
  int* v = new int[this->n];
  int counter = 0;
  Graph* g = new Graph();
  Vertex* p = this->rootVertex;
  Vertex* aux;
  Edge* e;
  while(p != NULL){
    g->addVertex(p->getID(), p->getValue());
    p = p->getNext();
  }
  while(p != NULL){
    e = p->getRootEdge();
    while(e != NULL){
      g->addEdge(p->getID(), e->getVertexID(), e->getValue());
      e = e->getNext();
    }
    p = p->getNext();
  }
  //---
  while(g->getRootVertex() != NULL){
    p = g->getRootVertex();
    while(p != NULL){
      if(p->getInDeg() == 0){
        e = p->getRootEdge();
        while(e != NULL){
          g->removeEdge(p->getID(), e->getVertexID());
          e = e->getNext();
        }
        v[counter] = p->getID(); counter++;
        aux = p->getNext();
        g->removeVertex(p->getID());
        p = aux;
      } else {
        p = p->getNext();
      }
    }
  }
  return v;
}

typedef struct {
  int id = 0;
  int incomingID = -1;
  int incomingIndex = -1;
  bool calculated = false;
  double distance = 0;
} DjikstraVertex;

double   Graph::SP_Djikstra(int ID1, int ID2){
  int minIndex, ID2Index;
  double wEdge;
  vector<int> q = this->getVertexIDList();
  int rem = q.size(); //remaining vertices
  DjikstraVertex* all = new DjikstraVertex[q.size()];
  for(int i = 0; i < q.size(); i++){
    all[i].id = q[i];
    all[i].distance = numeric_limits<double>::infinity();
    all[i].incomingID = -1;
    all[i].incomingIndex = -1;
    if(q[i] == ID1) all[i].distance = 0;
    if(q[i] == ID2) ID2Index = i;
  }
  while(rem > 0){
    minIndex = -1;
    for(int i = 0; i < q.size(); i++){
      if(minIndex < 0){
        if(all[i].calculated == false){
          minIndex = i;
        }
        continue;
      }
      if(all[i].distance < all[minIndex].distance && all[i].calculated == false){
        minIndex = i;
      }
    }
    all[minIndex].calculated = true; //removing from Q
    rem--;
    for(int i = 0; i < q.size(); i++){
      wEdge = this->getEdgeValue(all[minIndex].id, all[i].id);
      if(wEdge > 0 && (all[i].distance > all[minIndex].distance+wEdge)){
        all[i].distance = all[minIndex].distance+wEdge;
        all[i].incomingID = all[minIndex].id;
        all[i].incomingIndex = minIndex;
      }
    }
  }
  double distance = all[ID2Index].distance;
  delete[] all;
  return distance;
}

double** Graph::SP_Floyd(){
  double wEdge;
  double** A = new double*[this->n];
  for(int i = 0; i < this->n; i++){
    A[i] = new double[this->n];
    for(int j = 0; j < this->n; j++){
      A[i][j] = numeric_limits<double>::infinity();
    }
  }

  vector<int> id = this->getVertexIDList();

  for(int i = 0; i < this->n; i++){
    for(int j = 0; j < this->n; j++){
      if(i != j){
        wEdge = this->getEdgeValue(id[i],id[j]);
        if(wEdge > 0){
          A[i][j] = wEdge;
        } else {
          A[i][j] = numeric_limits<double>::infinity();
        }
      } else {
        A[i][j] = 0;
      }
    }
  }

  for(int k = 0; k < this->n; k++){
    for(int i = 0; i < this->n; i++){
      for(int j = 0; j < this->n; j++){
        if(A[i][j] > A[i][k] + A[k][j]){
          A[i][j] = A[i][k] + A[k][j];
        }
      }
    }
  }

  return A;
}

typedef struct {
  int ID1;
  int ID2;
  bool inserted;
  double w;
} KruskalEdge;

bool compEdge(const KruskalEdge &e1, const KruskalEdge &e2){
  return e1.w < e2.w;
}

double   Graph::MST_Kruskal(){
  /*int edgeCounter;
  vector<int> id = this->getVertexIDList();
  vector<vector<int>> sets;
  KruskalEdge* edges = new KruskalEdge[this->m];
  Edge* e;
  Vertex* p = this->rootVertex;
  edgeCounter = 0;
  while(p != NULL){
    e = p->getRootEdge();
    while(e != NULL){
      edges[edgeCounter].ID1 = p->getID();
      edges[edgeCounter].ID2 = e->getVertexID();
      edges[edgeCounter].w = e->getValue();
      edges[edgeCounter].inserted = false;
      edgeCounter++;
      e = e->getNext();
    }
    p = p->getNext();
  }

  vector<KruskalEdge> edgesVector(edges);
  sort(edgesVector.begin(), edgesVector.end(), compEdge);

  for(int i = 0; i < id.size(); i++){
    sets[i].push_back(id[i]);
  }


  */
  return 0;
}

double   Graph::MST_Prim(){
  return 0;
}

vector<int> Graph::DS_Greedy(){
  vector<int> c;
  vector<Vertex> v;

  //sort vertices by value (weight)
  Vertex* p = this->rootVertex;
  Edge* e;
  while(p != NULL){
    v.push_back(Vertex(p->getID(), p->getValue()));
    p = p->getNext();
  }
  std::sort(v.begin(), v.end());
  /*for(int i = 0; i < this->n; i++){
    std::cout << v[i].getID() << std::endl;
  }*/
  //---

  //execute the greedy algorithm while the vector is not empty
  while(v.size() > 0){
    c.push_back(v[0].getID());
    p = this->getVertex(v[0].getID());
    v.erase(v.begin());
    e = p->getRootEdge();
    while(e != NULL){
      int vID = e->getVertexID();
      for(int i = 0; i < v.size(); i++){
        if(v[i].getID() == vID){
          v.erase(v.begin()+i);
          break;
        }
      }
      e = e->getNext();
    }
  }
  //---
  return c;
}


vector<int> Graph::DS_GreedyRandomized(float alpha){
  vector<int> c;
  vector<Vertex> v;

  //sort vertices by value (weight)
  Vertex* p = this->rootVertex;
  Edge* e;
  while(p != NULL){
    v.push_back(Vertex(p->getID(), p->getValue()));
    p = p->getNext();
  }
  std::sort(v.begin(), v.end());
  //---

  //execute the randomized greedy algorithm
  while(v.size() > 0){
    //picking vertex
    int evl = (int)floor(alpha*v.size());
    int pickIndex = (evl == 0 ? 0 : rand()%(evl));
    c.push_back(v[pickIndex].getID());
    p = this->getVertex(v[pickIndex].getID());
    v.erase(v.begin()+pickIndex);
    //---
    e = p->getRootEdge();
    while(e != NULL){
      int vID = e->getVertexID();
      for(int i = 0; i < v.size(); i++){
        if(v[i].getID() == vID){
          v.erase(v.begin()+i);
          break;
        }
      }
      e = e->getNext();
    }
  }
  //---
  return c;
}
