#include "Graph.h"
#include <iostream>

Graph::Graph(){
  this->n = 0;
  this->m = 0;
  this->rootVertex = NULL;
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

bool Graph::addEdge(int ID1, int ID2, double value){
  Vertex* a = this->getVertex(ID1);
  Vertex* b = this->getVertex(ID2);
  if(a != NULL && b != NULL){
    a->connectEdge(ID2, value);
    b->connectEdge(ID1, value);
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
    if(a->removeEdge(ID2) && b->removeEdge(ID1)){
      //if removed edge successfully
      this->m -= 1;
      return true;
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
      std::cout << e->getVertexID() << " ";
      e = e->getNext();
    }
    std::cout << std::endl;
    p = p->getNext();
  }
}
