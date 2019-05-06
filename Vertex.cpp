#include "Vertex.h"
#include "Edge.h"

#include <stdlib.h>

Vertex::Vertex(int ID){
  this->value = 0;
  this->ID = ID;
  this->rootEdge = NULL;
}

Vertex::Vertex(int ID, double value){
  this->value = value;
  this->ID = ID;
  this->rootEdge = NULL;
}

void Vertex::connectEdge(Edge* e){
  e->setNext(this->rootEdge);
  this->rootEdge = e;
}

void Vertex::connectEdge(int connID, double value){
  Edge* p = new Edge(connID, NULL, value);
  this->connectEdge(p);
}

bool Vertex::removeEdge(int connID){
  if(this->rootEdge == NULL){
    //nothing to remove
    return false;
  } else {
    Edge* a = NULL;
    Edge* p = this->rootEdge;
    while(p != NULL){
      if(p->getVertexID() == connID){
        //remove edge
        if(a == NULL){
          this->rootEdge = p->getNext();
        } else {
          a->setNext(p->getNext());
        }
        p = NULL;
        return true;
      }
      a = p;
      p = p->getNext();
    }
    //edge wasn't found
    return false;
  }
}

int Vertex::getID(){
  return this->ID;
}

double Vertex::getValue(){
  return this->value;
}

void Vertex::setValue(double value){
  this->value = value;
}

Edge* Vertex::getRootEdge(){
  return this->rootEdge;
}

void Vertex::setRootEdge(Edge* rootEdge){
  this->rootEdge = rootEdge;
}

Vertex* Vertex::getNext(){
  return this->next;
}

void Vertex::setNext(Vertex* next){
  this->next = next;
}

void Vertex::setColor(int color){
  this->color = color;
}

int Vertex::getColor(){
  return this->color;
}
