#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <queue>
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
    vector<Vertex>              toBeChecked;
    vector<Vertex>::iterator    v;
    Vertex                      emptyVertex;
    emptyVertex.init();

    for (i = 0;i < max;i++) {
      emptyVertex.number = i;
      toBeChecked.push_back(emptyVertex);
    }
    

  }
  outfile.close();
  return 0;
}
