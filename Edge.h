#ifndef EDGE_H
#define EDGE_H
class Vertex;

class Edge {

  private:
    int vertexID;
    Edge* next;
    double value;

  public:
    Edge();
    Edge(int vertexID, Edge* next, double value);
    int getVertexID();
    Edge* getNext();
    double getValue();
    void setVertexID(int vertexID);
    void setNext(Edge* next);
    void setValue(double value);
};

#endif
