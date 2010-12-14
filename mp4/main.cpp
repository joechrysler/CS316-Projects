#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <stack>
#include <list>

using namespace std;

class Job {

	public:
    // Constructors
		Job() {
			clear();
		};
    Job(int inPriority, int inId, int inRuntimeEstimate) {
      priority = inPriority;
      id = inId;
      runtime_estimate = inRuntimeEstimate;
      runtime = 0;
    };
		void clear() {
			priority = id = runtime_estimate = runtime = 0;
		};

		void	read(ifstream &infile) {
			// jc add error checking for empty files
			infile >> priority >> id >> runtime_estimate;
		};

    // Printing functions for debugging
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
    void linePrint() {
      cout << setw(5) << priority 
           << setw(6) << id
           << setw(5) << runtime_estimate << endl;
    };
    void printId() {
      cout << id;
    };

    // Operator definitions
		bool operator< (Job& other) {
			return (priority < other.priority);
		};
    bool operator<= (Job& other) {
			return (priority <= other.priority);
    };
    bool operator>= (Job& other) {
			return (priority >= other.priority);
    };
		bool operator> (Job& other) {
			return (priority > other.priority);
		};
    bool operator== (Job& other) {
      return (id == other.id);
    };
    bool operator!= (Job& other) {
      return (id != other.id);
    };

	private:
		int	priority;
		int	id;
		int runtime_estimate;
		int runtime;
};

class Heap {
  public:
    Heap() {
      data.clear();
    };

    void add(Job& newJob) {
      data.push_back(newJob);
    };

    void enqueue(Job& newJob) {
      //newJob.linePrint();
      data.push_back(newJob);
      Job temp;
      int iTemp;
      int iNewJob = data.size() -1;
      int iParent = (iNewJob)/2;

      while (iNewJob != 0 && data[iNewJob] > data[iParent]) {
        // Swap data with parent
        temp = data[iNewJob];
        data[iNewJob] = data[iParent];
        data[iParent] = temp;

        // Swap indexes to keep the loop functional
        iNewJob = iParent;
        iParent = (iNewJob)/2;
      }
    };
    Job dequeue() {
      Job temp = data[0];
      Job swap;
      int iParent = 0;
      int lChild = 2*(iParent+1)-1;
      int rChild = 2*(iParent+1);
      int iChild;

      // Swap last element into root
      data[0] = data[data.size()-1];
      data.pop_back();

      // jc - try setting all priorities to 1
      while (!isLeaf(iParent)
            && (data[iParent] < data[lChild] || data[iParent] < data[rChild])) {
        iChild = (data[lChild] <= data[rChild])?
          rChild:
          lChild;

        swap = data[iParent];
        data[iParent] = data[iChild];
        data[iChild] = swap;

        iParent = iChild;
        lChild = 2*(iParent+1)-1;
        rChild = 2*(iParent+1);
      }
      return temp;
    };
    void print() {
      vector<Job>::iterator it;
      for (it = data.begin(); it != data.end(); it++) {
        it->print();
      };
      cout << endl;
    }

    bool empty() {
      return (data.size() == 0);
    };
    void dropLast() {
      data.pop_back();
    };

  private:
    bool hasLeftChild(int i) {
      return 2*(i+1) < data.size()+1;
    };
    bool hasRightChild(int i) {
      return 2*(i+1)+1 < data.size()+1;
    };
    bool isLeaf(int i) {
      return !(hasLeftChild(i) || hasRightChild(i));
    };

    vector<Job>   data;
};

int main() {
	long				clock = 0;
	Heap				pq;

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
	
  for (int i = 0; i < 10; i++) {
    Job newJob;
    newJob.read(infile);
    pq.enqueue(newJob);
  }
  cout <<left<< setw(20) << "Jobs queued";
  pq.print();

  while (infile){
    Job newJob;
    newJob.read(infile);
    pq.enqueue(newJob);
  }
  pq.dropLast();

  cout <<left<< setw(20) << "Jobs queued";
  pq.print();

  while (!pq.empty()) {
    Job topJob = pq.dequeue();
    cout << "popped job ";
    // jc - Why is it popping job 0/
    topJob.printId();
    cout << endl;
  }
	return 0;
}
