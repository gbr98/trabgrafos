#ifndef VERTEX_H
#define VERTEX_H
class Edge;

class Vertex {

  private:
    int ID;
    double value;
    Edge* rootEdge;
    
    Vertex* next;
    int color;
    
  public:
    Vertex(int ID);
    Vertex(int ID, double value);
    void connectEdge(int connID, double value);
    void connectEdge(Edge* e);
    bool removeEdge(int connID);
    int getID();
    double getValue();
    Edge* getRootEdge();
    void setValue(double value);
    void setRootEdge(Edge* rootEdge);

    Vertex* getNext();
    void setNext(Vertex* next);
    
    void setColor(int color);
    int getColor();
};

#endif
