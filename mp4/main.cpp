#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <stack>
#include <list>

using namespace std;

class Job {

	public:
		Job() {
			clear();
		};
		void clear() {
			priority = id = runtime_estimate = runtime = 0;
		};

		void	readJob(ifstream &infile) {
			// jc add error checking for empty files
			infile >> priority >> id >> runtime_estimate;
		};
		void	prettyprint() {
			cout << "priority: " << priority					<< endl
				   << "id:       " << id								<< endl
					 << "estimate: " << runtime_estimate	<< endl
					 << "runtime:  " << runtime						<< endl
					 << "--------------"									<< endl;
		};
		void print() {
			cout << priority << " | ";
		};
		bool operator< (Job& other) {
			return (priority < other.priority);
		};
    bool operator== (Job& other) {
      return (id == other.id);
    };

	private:
		int	priority;
		int	id;
		int runtime_estimate;
		int runtime;
};

class Tree {
  public:

    void add(Job newNode) {

    };

    Job   node;
    Tree  *l;
    Tree  *r;
};

//class Heap {
	//public:
		//Heap() {
			//data.clear();
			//last = lastNonLeaf = insertionPoint = root = 0;
		//};

		//void add(Job& newJob) {
			//data.push_back(newJob);
			//updateLastNonLeafNode();
			
		//};

		//void print() {
			//vector<Job>::iterator it;
			//for (it = data.begin(); it != data.end(); it++) {
				//it->print();
			//};
			//cout << endl;
		//}

		//void updateLastNonLeafNode() {
			//lastNonLeaf = data[(data.size()/2) - 1];
		//};

	//private:
		//vector<Job>   data;
    //vector<Job>   lastRow;
		//long          lastNonLeaf;
		//long          insertionPoint;
		//long          root;
//};



int main() {
	//long				clock = 0;
	//Heap				pq;

	// === I/O file block ===
	ifstream		infile;	
	ofstream		outfile;
	infile.open("in.data");	
	outfile.open("out.data");
	if(infile.fail() || outfile.fail()) {
		cout << "couldn't open input or output file" << endl;
		return 1;
	}
	//=== end ===
	
	//while (1) {
		//for (int i = 0; i < 10; i++) {
			//Job newJob;
			//newJob.readJob(infile);
			//pq.add(newJob);
		//}
		//pq.print();
		//return 0;
	//}
	return 0;
}

