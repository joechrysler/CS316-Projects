#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
//#include "graph.h"

using namespace std;

class Graph {
  public:
    vector< vector<int> >       adjacencyMatrix;
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

    void printAdjacencyMatrix(ofstream &outfile) {
      int i, j;

      for (i = 0; i < adjacencyMatrix.size(); i++) {
        for (j = 0; j < adjacencyMatrix[i].size(); j++) {
          cout << setw(4) << adjacencyMatrix[i][j] << "  ";
        }
        cout << endl;
      }
    };
};

int main(int argc, const char *argv[])
{
	ifstream		                infile;     // input file
	ofstream		                outfile;    // output file
  Graph                       greg;

  // File IO block
	infile.open("in.data");	
	outfile.open("out.data");
	if(infile.fail() || outfile.fail())
		cout << "couldn't open input or output file" << endl;

  greg.readAdjacencyMatrix(infile);
  greg.printAdjacencyMatrix(outfile);


  
  return 0;
}
