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

struct Vertex {
  int           distance;
  int           number;
  Vertex        *predecessor;
  bool          visited;

  void init() {
    number        = 0;
    distance      = INT_MAX;
    predecessor   = NULL;
    visited       = false;
  };
};

void inTBC(int v, deque<Vertex> &tbc) {
  deque<Vertex>::iterator x;
  bool found = false;

  for (x = tbc.begin(); x != tbc.end(); x++)
    if (x.number == v)
      found = true;
  return found;
};

int main(int argc, const char *argv[])
{
  // Variables
  int                         max = 0;    // Size of adjacency matrix
  int                         i, j;       // Counters
	ifstream		                infile;     // input file
	ofstream		                outfile;    // output file
  queue<Pair>                 jobs;
  Pair                        thisJob;

  // File IO block
	infile.open("in.data");	
	outfile.open("out.data");
	if(infile.fail() || outfile.fail())
		cout << "couldn't open input or output file" << endl;

  // Parse infile
  infile >> max;
  int adjacency[max][max];

  for (i = 0; i < max; i++)
    for (j = 0; j < max; j++)
      infile >> adjacency[i][j];

  while (infile) {
    infile >> thisJob.x >> thisJob.y;
    jobs.push(thisJob);
  }

  // Debug Prints
  if (DEBUG) {
    cout << endl;
    cout << "Adjacency Matrix:" << endl;
    for (i = 0; i < max; i++) {
      for (j = 0; j < max; j++) {
        cout << setw(4) << adjacency[i][j] << "  ";
      }
      cout << endl;
    }
  }
  //if (DEBUG) {
    //cout << endl;
    //cout << "Jobs:" << endl;
    //while (!jobs.empty()) {
      //cout << jobs.front().x << "  " << jobs.front().y << endl;
      //jobs.pop();
    //}
  //}


  while (!jobs.empty()) {
    deque<Vertex>               toBeChecked;
    Vertex                      *v;
    Vertex                      emptyVertex;
    emptyVertex.init();
    int u;

    // Add all vertices to toBeChecked
    for (i = 0;i < max;i++) {
      emptyVertex.number = i;
      toBeChecked.push_back(emptyVertex);
    }

    while (!toBeChecked.empty()) {
      v = toBeChecked.front();
      for (u = 0;u < max;u++) {
        if (adjacency[v->number][u] != 0, and inTBC(u, toBeChecked) {
          if (adjacency[v->number][u].distance > v->distance + adjacency[v->number][u]
        }
      }

    }
      
    cout << jobs.front().x << "  " << jobs.front().y << endl;
    

    jobs.pop();
  }
  outfile.close();
  return 0;
}
