#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <stack>
#include <list>
#include <math.h>

using namespace std;

class Job {

	public:
    // Constructors
		Job() {
			clear();
		};
    Job(int inPriority, int inId, int inRuntimeEstimate, int inRecency) {
      priority = inPriority;
      id = inId;
      runtime_estimate = inRuntimeEstimate;
      runtime = 0;
      recency = inRecency; 
    };
		void clear() {
			priority = id = runtime_estimate = runtime = recency = 0;
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
			cout << priority << "." << recency << " | ";
		};
    void linePrint() {
      cout << setw(5) << priority 
           << setw(6) << id
           << setw(5) << runtime_estimate << endl;
    };
    void printId() {
      cout << id << " | ";
    };

    void setRecency(int r) {
      recency = r;
    };

    // Operator definitions
    bool larger(Job& other) {
      if (priority > other.priority) {
        //cout << "P." << priority << ">" << "P." << other.priority << endl;
        return true;
      }
      if (priority == other.priority && recency < other.recency) {
        //cout << "R." << recency << ">" << "R." << other.recency << endl;
        return true;
      }
      return false;
    };
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
    int recency;
		int runtime_estimate;
		int runtime;
};

class Heap {
  public:
    Heap() {
      data.clear();
      n = 0;
    };

    void add(Job& newJob) {
      data.push_back(newJob);
    };

    void enqueue(Job& newJob) {
      newJob.setRecency(++n);
      //newJob.linePrint();
      data.push_back(newJob);
      int iNewJob = data.size()-1;
      int iParent = parent(iNewJob);

      while (iNewJob != 0 && data[iNewJob].larger(data[iParent])) {
        swap(iNewJob, iParent);
        iNewJob = iParent;
        iParent = parent(iNewJob);
      }
    };
    Job dequeue() {
      Job temp = data[0];
      int iParent = 0;
      int lChild = 2*(iParent+1)-1;
      int rChild = 2*(iParent+1);
      int iChild;

      // Swap last element into root
      data[0] = data[data.size()-1];
      data.pop_back();

      // jc - try setting all priorities to 1
      while (!isLeaf(iParent)
            && (data[lChild].larger(data[iParent]) || data[rChild].larger(data[iParent]))) {
        iChild = (data[lChild].larger(data[rChild]))?
          lChild:
          rChild;

        swap(iParent,iChild);

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
    void printId() {
      vector<Job>::iterator it;
      for (it = data.begin(); it != data.end(); it++) {
        it->printId();
      };
      cout << endl;
    }

    bool empty() {
      return (data.size() == 0);
    };
    void dropLast() {
      data.pop_back();
    };
    void moveDown(int first, int last) {
      int largest = 2*first + 1;
      while (largest <= last) {
        if (largest < last && data[largest] < data[largest + 1])
          largest++;
        if (data[first] < data[largest]) {
          // swap parent and child
          swap(first,largest);
          first = largest;
          largest = 2*first+1;
        }
        else largest = last+1;
      }
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
    int parent(int child) {
      if (child > 0)
        return ceil(child/2.0)-1;
      return 0;
    };
    void swap(int one, int other) {
      Job temp;

      temp = data[one];
      data[one] = data[other];
      data[other] = temp;
    };

    vector<Job>   data;
    int           n;
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
