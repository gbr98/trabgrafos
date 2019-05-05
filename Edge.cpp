#include "Edge.h"
#include "Vertex.h"

Edge::Edge(){

}

Edge::Edge(int vertexID, Edge* next, double value){
  this->vertexID = vertexID;
  this->next = next;
  this->value = value;
}

int Edge::getVertexID(){
  return this->vertexID;
}

void Edge::setVertexID(int vertexID){
  this->vertexID = vertexID;
}

Edge* Edge::getNext(){
  return this->next;
}

void Edge::setNext(Edge* next){
  this->next = next;
}

double Edge::getValue(){
  return this->value;
}

void Edge::setValue(double value){
  this->value = value;
}
