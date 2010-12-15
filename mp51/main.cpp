//*****************************************************************************
//   CS316           Machine Problem 5-1      Joe Chrysler  
//
//   Dr. Lee                             December 15, 2010
//
//*****************************************************************************
// Classes
//*****************************************************************************
//  Vertex - Defines the structure of a vertex
//  Edge   - Defines the structure of an edge
//  Graph  - Defines the structure of the graph and the methods that operate
//           on it
//*****************************************************************************
//  Global Variables
//*****************************************************************************
//   infile   - reference to the input file
//   outfile  - reference to the output file
//   gopher   - reference to the main graph object
//   jobs     - queue of ordered pairs representing jobs from the infile
//   newJob   - used for reading in jobs from the infile
//
//*****************************************************************************
//  Development History
//*****************************************************************************
//
//  This program was fun to write, but frustrating in a lot of places.
//  I started out by trying to record only nodes, not edges.  That failed
//  pretty miserably when I started implementing Dijkstra's algorithm.  Then
//  I researched the topic a bit more and learned that it was a lot easier if
//  the program kept track of both nodes and edges.  After I made that change,
//  the implementation went much more smoothly.
//
//  Shalom,
//  Joe
//  
//*****************************************************************************
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <queue>
#include <climits>
#include <string>
//#include "graph.h"

using namespace std;

const bool DEBUG = true;

struct Pair {
  // Defines the structure of a job from the input file.  After reading it in,
  // we want to find the shortest path from x to y.
  int x;
  int y;
};


class Vertex {
  // Represents a vertex in the graph
  //
  // Variables:
  //  distance    - The current distance from the origin node
  //  id          - The number associated with this node (V0, V1, V2, etc.)
  //  predecessor - Pointer to the vertex that comes before this one in
  //                Dijkstra's shortest path.
  //  visited     - Flag that indicates whether this vertex has been visited
  //                during the Dijkstra algorithm
  public:
    Vertex() {
      // Initialize the vertex with default values
      id            = -1;
      distance      = INT_MAX;
      predecessor   = NULL;
      visited       = false;
    };
    Vertex(int inId) {
      // Initialize the vertex with a given ID
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
  // Represents an edge that connets two vertices in the graph
  //
  // Variables:
  //  distance    - The length of this edge (used for shortest path algorithm)
  //  *v1         - Pointer to the first vertex in this edge
  //  *v2         - Pointer to the second vertex in this edge
  //
  public:
    Edge() {
      // Default constructor
      v1 = NULL;
      v2 = NULL;
      distance = INT_MAX;
    };
    Edge(Vertex* inVertex1, Vertex* inVertex2, int inDistance) {
      // Initialize the edge with pointers to 2 given vertices and a distance
      v1 = inVertex1;
      v2 = inVertex2;
      distance = inDistance;
    };

    bool Connects(Vertex* inVertex1, Vertex* inVertex2) {
      // Checks whether or not two given vertices are connected by this edge.
      return ((inVertex1 == this->v1 && inVertex2 == this->v2) || (inVertex2 == this->v1 && inVertex1 == this->v2));
    };


    Vertex    *v1;
    Vertex    *v2;
    int     distance;
};

class Graph {
  // Represents the overall graph object and contains all functions for the
  // shortest path algorithm.
  // 
  //  vertices          - A vector of vertices that have yet to be processed in
  //                      the current graph
  //  static_vertices   - An almost constant vector of all vertices in this
  //                      graph.
  //  edges             - A vector the edges in this graph
  //  adjacencyMatrix   - An in memory representation of the adjacency matrix
  //                      that built this graph
  //
  public:
    // Adders
    //  These functions add a new vertex or edge to the graph's internal
    //  vectors.
    void addVertex(int newId) {
      Vertex* newVertex = new Vertex(newId);
      vertices.push_back(newVertex);
      static_vertices.push_back(newVertex);
    };
    void addEdge(Vertex* newV1, Vertex* newV2, int newDistance) {
      Edge* newEdge = new Edge(newV1, newV2, newDistance);
      edges.push_back(newEdge);
    };

    // Adjacency Matrix
    //  These functions read a valid adjacency matrix from the input file,
    //  process an in-memory adjacency matrix into a vector of edges and
    //  vertices and print a graphical representation of an adjacency matrix.
    //  They do their work simply and well.
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
    void printAdjacencyMatrix(ofstream& outfile) {
      int i, j;

      outfile << endl << "Adjacency Matrix:" << endl;
      for (i = 0; i < adjacencyMatrix.size(); i++) {
        for (j = 0; j < adjacencyMatrix[i].size(); j++) {
          outfile << setw(4) << adjacencyMatrix[i][j] << "  ";
        }
        outfile << endl;
      }
    };

    // List Printers
    //  These functions loop through the vector of edges and vertices, printing
    //  each one they come to.
    void printVertices(ofstream& outfile) {
      int size = vertices.size();
      int i;
      outfile << endl << "Vertices:" << endl;
      for (i = 0; i < size; i++) {
        outfile << vertices.at(i)->id << endl;
      }
    };
    void printEdges(ofstream& outfile) {
      int size = edges.size();
      int i;
      outfile << endl << "Edges:" << endl;
      for (i = 0; i < size; i++) {
        outfile << edges.at(i)->v1->id << "-->" <<  edges.at(i)->v2->id << " : " << edges.at(i)->distance << endl;
      }
    };

    // Decorators
    //  These functions print a nice heading for the output file and a label
    //  for the shortest path operations.
    void PrintHeading(ofstream& outfile) {
      outfile << left << setw(12) << "  Path" << setw(9) << "Length" << "Diagram" << endl;
      outfile << left << setw(12) << "  ------" << setw(9) << "------" << "-------" << endl;
    };
    void PrintLabel(int o, int e, ofstream& outfile) {
      string something = "";
      outfile << right << setw(3) << o << " to " << setw(5) << left << e << something;
    };

    // Garbage Collection
    //  These functions loop through the edge and vertex vectors, erasing them.
    //  The reprocess function runs both clearVertices and clearEdges, then
    //  reprocesses the adjacency matrix to repopulate them.
    void reprocess() {
      clearVertices();
      clearEdges();
      ProcessAdjacencyMatrix();
    };
    void clearVertices() {
      vertices.erase(vertices.begin(), vertices.end());
      static_vertices.erase(static_vertices.begin(), static_vertices.end());
    };
    void clearEdges() {
      edges.erase(edges.begin(), edges.end());
    };

    // The Star of the Show
    // Mr. Dijkstra
    //  This function performs the logic of Dijkstra's shortest path algorithm.
    //  it assumes that one vertex, the origin, has a distance of 0.
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

    // Dijkstra's assistants
    //  These underpaid methods do a lot of the heavy lifting in the Dijkstra
    //  algorithm.
    //
    //  findSmallestVertex(), for instance, loops through the list of
    //  vertices, looking for the one with the least current distance.
    //
    //  findAdjacentRemainingVertices() looks through the list of edges for any
    //  vertices adjacent to the current one, makes sure that they are in the
    //  list of vertices yet to be processed, then returns a vector of the
    //  results.
    //
    //  Distance() looks in the edges list for an edge containing two given
    //  vertices.  If it finds one, it returns the distance between them.
    //
    //  inVertices() looks through the list of vertices yet to be processed.
    //  If it finds the given vertex, it returns true, otherwise false.
    //
    //  PrintShortestPath() should be called after Dijkstra has done his work.
    //  This humble function takes a vertex, supposes that it is the last node
    //  in the shortest path, then follows the predecessor pointers to the
    //  first node, printing each as it goes along.
    //
    //  setOrigin() is a small, but important beast.  It accepts a vertex id,
    //  locates that vertex in the vertices vector and sets its distance to 0,
    //  effectively making that vertex the origin.
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
    void PrintShortestPath(int v, ofstream& outfile) {
      if (v < static_vertices.size()) {
        Vertex* e = static_vertices.at(v);
        if (e->distance < 0) {
          outfile << "No Shortest Path Exists" << endl;
        } else {
          outfile << left << setw(5) << e->distance << "    ";
          while (e) {
            outfile << e->id;
            if (e->predecessor != NULL)
              outfile << " --> ";
            e = e->predecessor;
          }
          outfile << endl;
        }
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
  Graph                       gopher;
  queue<Pair>                 jobs;
  Pair                        newJob;

  // File IO block
	infile.open("in.data");	
	outfile.open("out.data");
	if(infile.fail() || outfile.fail())
		cout << "couldn't open input or output file" << endl;

  gopher.readAdjacencyMatrix(infile);
  //gopher.printAdjacencyMatrix();
  gopher.ProcessAdjacencyMatrix();

  while (infile) {
    infile >> newJob.x >> newJob.y;
    jobs.push(newJob);
  }

  gopher.PrintHeading(outfile);
  while (jobs.size() > 1) {
    gopher.setOrigin(jobs.front().x);
    gopher.dijkstra();
    gopher.PrintLabel(jobs.front().x, jobs.front().y, outfile);
    gopher.PrintShortestPath(jobs.front().y, outfile);
    gopher.reprocess();
    jobs.pop();
  }
  
  outfile.close();
  return 0;
}
