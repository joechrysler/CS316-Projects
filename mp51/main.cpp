#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <queue>
#include <climits>
//#include "graph.h"

using namespace std;

const bool DEBUG = true;

struct Pair {
  int x;
  int y;
};


class Vertex {
  public:
    Vertex() {
      id            = -1;
      distance      = INT_MAX;
      predecessor   = NULL;
      visited       = false;
    };
    Vertex(int inId) {
      id            = inId;
      distance      = INT_MAX;
      predecessor   = NULL;
      visited       = false;
    };
    
    int           distance;
    int           id;
    Vertex        *predecessor;
    bool          visited;
};
class Edge {
  public:
    Edge() {
      v1 = NULL;
      v2 = NULL;
      distance = INT_MAX;
    };
    Edge(Vertex* inVertex1, Vertex* inVertex2, int inDistance) {
      v1 = inVertex1;
      v2 = inVertex2;
      distance = inDistance;
    };

    bool Connects(Vertex* inVertex1, Vertex* inVertex2) {
      return ((inVertex1 == this->v1 && inVertex2 == this->v2) || (inVertex2 == this->v1 && inVertex1 == this->v2));
    };


    Vertex    *v1;
    Vertex    *v2;
    int     distance;
};

class Graph {
  public:
    void addVertex(int newId) {
      Vertex* newVertex = new Vertex(newId);
      vertices.push_back(newVertex);
      static_vertices.push_back(newVertex);
    };
    void addEdge(Vertex* newV1, Vertex* newV2, int newDistance) {
      Edge* newEdge = new Edge(newV1, newV2, newDistance);
      edges.push_back(newEdge);
    };

    // Adjacency Matrix Functions
    void readAdjacencyMatrix(ifstream &infile) {
      vector<int> row;
      int max, i, j, temp;
      infile >> max;

      for (i = 0; i < max; i++) {
        row.erase(row.begin(), row.end());
        for (j = 0; j < max; j++) {
          infile >> temp;
          row.push_back(temp);
        }
        adjacencyMatrix.push_back(row);
      }
    };
    void ProcessAdjacencyMatrix() {
      int size = adjacencyMatrix.size();
      int i, j;
      
      for (i=0;i<size;i++)
        addVertex(i);
      for (i=0;i<size;i++)
        for (j=0;j<size;j++)
          if (adjacencyMatrix[i][j] != 0)
            addEdge(vertices[i], vertices[j], adjacencyMatrix[i][j]);
    };
    void printAdjacencyMatrix() {
      int i, j;

      cout << endl << "Adjacency Matrix:" << endl;
      for (i = 0; i < adjacencyMatrix.size(); i++) {
        for (j = 0; j < adjacencyMatrix[i].size(); j++) {
          cout << setw(4) << adjacencyMatrix[i][j] << "  ";
        }
        cout << endl;
      }
    };

    // List Printing Functions
    void printVertices() {
      int size = vertices.size();
      int i;
      cout << endl << "Vertices:" << endl;
      for (i = 0; i < size; i++) {
        cout << vertices.at(i)->id << endl;
      }
    };
    void printEdges() {
      int size = edges.size();
      int i;
      cout << endl << "Edges:" << endl;
      for (i = 0; i < size; i++) {
        cout << edges.at(i)->v1->id << "-->" <<  edges.at(i)->v2->id << " : " << edges.at(i)->distance << endl;
      }
    };

    // Garbage Collection
    void reprocess() {
      clearVertices();
      clearEdges();
      ProcessAdjacencyMatrix();
    };
    void clearVertices() {
      vertices.erase(vertices.begin(), vertices.end());
    };
    void clearEdges() {
      edges.erase(edges.begin(), edges.end());
    };

    void dijkstra() {
      int size, i, distance;

      while (vertices.size() > 0) {
        Vertex* smallest = findSmallestVertex();
        vector<Vertex*>* adjacentVertices = findAdjacentRemainingVertices(smallest);
        size = adjacentVertices->size();
        for (i=0; i<size; i++) {
          Vertex* adjacent = adjacentVertices->at(i);
          distance = Distance(smallest, adjacent) + smallest->distance;
          if (distance < adjacent->distance) {
            adjacent->distance    = distance;
            adjacent->predecessor = smallest;
          }
        }
        delete adjacentVertices;
      }
    };

    // Dijkstra Helper Functions
    Vertex* findSmallestVertex() {
      int size = vertices.size();
      if (size == 0) return NULL;
      int i;
      int smallestIndex = 0;
      Vertex* smallest = vertices.at(smallestIndex);

      for (i=0;i<size;i++) {
        if (vertices.at(i)->distance < smallest->distance) {
          smallest = vertices.at(i);
          smallestIndex = i;
        }
      }
      vertices.erase(vertices.begin() + smallestIndex);
      return smallest;
    };
    vector<Vertex*>* findAdjacentRemainingVertices(Vertex* v) {
      vector<Vertex*>* adjacentVertices = new vector<Vertex*>();
      int size = edges.size();
      int i;

      for (i=0;i<size;i++) {
        Edge* edge = edges.at(i);
        Vertex* adjacent = NULL;
        if (v == edge->v1) {
          adjacent = edge->v2;
        } else if (v == edge->v2) {
          adjacent = edge->v1;
        }
        if (adjacent && inVertices(adjacent)) {
          adjacentVertices->push_back(adjacent);
        }
      }
      return adjacentVertices;
    };
    int Distance(Vertex* one, Vertex* another) {
      int size, i;
      size = edges.size();

      for (i=0;i<size;i++) {
        Edge* e = edges.at(i);
        if (e->Connects(one, another))
          return e->distance;
      }
      return INT_MAX;
    };
    bool inVertices(Vertex* v) {
      int size = vertices.size();
      int i;
      
      for (i=0;i<size;i++)
        if (v == vertices.at(i))
          return true;
      return false;
    };
    void PrintShortestPath(int v) {
      if (v < static_vertices.size()) {
        Vertex* e = static_vertices.at(v);
        cout << "Path Length: " << e->distance << endl;
        while (e) {
          cout << e->id;
          if (e->predecessor != NULL)
            cout << " --> ";
          e = e->predecessor;
        }
        cout << endl;
      }
    };
    void setOrigin(int v) {
      if (v < vertices.size()) {
        vertices.at(v)->distance = 0;
      }
    };

  private:
    vector<Vertex*>             vertices;
    vector<Vertex*>             static_vertices;
    vector<Edge*>               edges;
    vector< vector<int> >       adjacencyMatrix;
};

int main(int argc, const char *argv[]) {
  // Variables
	ifstream		                infile;     // input file
	ofstream		                outfile;    // output file
  Graph                       greg;
  queue<Pair>                 jobs;
  Pair                        newJob;

  // File IO block
	infile.open("in.data");	
	outfile.open("out.data");
	if(infile.fail() || outfile.fail())
		cout << "couldn't open input or output file" << endl;

  greg.readAdjacencyMatrix(infile);
  greg.printAdjacencyMatrix();
  greg.ProcessAdjacencyMatrix();

  greg.setOrigin(9);
  greg.dijkstra();
  greg.PrintShortestPath(0);
  

  //while (infile) {
    //infile >> newJob.x >> newJob.y;
    //jobs.push(newJob);
  //}



  //// Print Jobs
  //cout << endl;
  //cout << "Jobs:" << endl;
  //while (!jobs.empty()) {
    //cout << jobs.front().x << "  " << jobs.front().y << endl;
    //jobs.pop();
  //}

  //while (!jobs.empty()) {
    //deque<Vertex>               toBeChecked;
    //Vertex                      *v;
    //Vertex                      emptyVertex;
    //emptyVertex.init();
    //int u;

    //// Add all vertices to toBeChecked
    //for (i = 0;i < max;i++) {
      //emptyVertex.number = i;
      //toBeChecked.push_back(emptyVertex);
    //}

    //while (!toBeChecked.empty()) {
      //v = toBeChecked.front();
      //for (u = 0;u < max;u++) {
        //if (adjacency[v->number][u] != 0, and inTBC(u, toBeChecked) {
          //if (adjacency[v->number][u].distance > v->distance + adjacency[v->number][u]
        //}
      //}

    //}
      
    //cout << jobs.front().x << "  " << jobs.front().y << endl;
    

    //jobs.pop();
  //}
  //outfile.close();
  return 0;
}
