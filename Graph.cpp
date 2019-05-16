#include "Graph.h"
#include <iostream>
#include <queue>
#include <stack>
#include <algorithm>

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
