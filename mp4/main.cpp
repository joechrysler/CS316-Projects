//*****************************************************************************
//   CS316           Machine Problem 4        Joe Chrysler  
//
//   Dr. Lee                             December 15, 2010
//
//*****************************************************************************
// Classes
//*****************************************************************************
//  Job     - Defines the structure of a processing job and the functions
//            that handle its processing
//  Heap    - Defines the structure of a heap and the methods for keeping it
//            organized properly.  Heap can take any type of variable as an
//            input, so long as it defines the > operator.
//
//*****************************************************************************
//  Global Variables
//*****************************************************************************
//   infile             - reference to the input file
//   outfile            - reference to the output file
//   waiting_time       - The total number of clock ticks spent waiting
//   turnaround_time    - The total number of clock ticks spent processing jobs
//   numJobs            - Total number of jobs processed
//   clock              - Number of ticks elapsed since program beginning
//   pq                 - Primary heap variable
//   nextJob            - Holds the top job from the heap while it's being
//                        processed
//
//*****************************************************************************
//  Development History
//*****************************************************************************
//
//  This program took a long time to finish because I initially tried
//  constructing the heap like a pointer-based binary tree.  When I arrived at
//  formulas which would give me the index of a given node's parent and
//  children for an array-based heap, development progressed very rapidly.  The
//  algorithms were fun to design.  I ran into a bit of trouble when
//  constructing the heap re-organizing functions in enqueu and dequeue.  When
//  I tested these functions on input files where every job had the same
//  priority, my heap processed the jobs exactly backwards.  I then discovered
//  that I had to take the order in which jobs were added into the queue into
//  account when checking whether a parent node was larger than a child during
//  the move-down process.  This was a fun program.
//
//  Shalom,
//  Joe
//  
//*****************************************************************************
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <stack>
#include <list>
#include <math.h>

using namespace std;

class Job {
  // Represents a processing job
  //
  // Variables:
  //  priority    - The priority of the current job
  //  id          - The process ID of the current job
  //  recency     - Keeps track of the order jobs are read in.  Used during
  //                heap-reorganization as a tie-breaker for jobs of equal
  //                priority.
  //  added       - Clock tick when this job was added
  //  began       - Clock tick when this job began to be processed
  //  finished    - Clock tick when this job finished being processed
  //

	public:
    // Constructors
    //  Initialized the job with default or given data
		Job() {
			clear();
		};
    Job(int inPriority, int inId, int inRuntimeEstimate, int inRecency, int clock) {
      priority = inPriority;
      id = inId;
      runtime_estimate = inRuntimeEstimate;
      recency = inRecency; 
      added = clock;
      began = 0;
      finished = 0;
    };
		void clear() {
			priority = id = runtime_estimate = began = recency = added = finished = 0;
		};

    // Read/Write
    //  Read a job from an input file or
    //  Write that the job has been read.
		void	read(ifstream &infile) {
			// jc add error checking for empty files
			infile >> priority >> id >> runtime_estimate;
		};
    void write(ofstream &outfile) {
      if (priority != 0)
        outfile << "input read: " << setw(5) << left << priority
                << setw(5) << right << id
                << setw(5) << right << runtime_estimate << endl;
    };

    // Printing functions for debugging
    //  These functions show the status of a Job in various ways
		void	prettyprint() {
			cout << "priority: " << priority					<< endl
				   << "id:       " << id								<< endl
					 << "estimate: " << runtime_estimate	<< endl
					 << "runtime:  " << runTime()	  			<< endl
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

    // Printer
    //  This function prints a nicely formatted listing of a processed job to
    //  the output file
    void printProcessed(ofstream& outfile) {
      outfile << "Process scheduled: id = " << setw(4) << right << id
              << "\tpriority = " << priority
              << "\twait time = " << waitingTime()
              << "\tturnaround time = " << turnaroundTime()
              //<< ", a = " << added
              //<< ", b = " << began
              //<< ", f = " << finished
              << endl;
    };


    // Timing Functions
    //  These functions calculate the various time measurements required for
    //  this assignment
    int waitingTime() {
      return began - added;
    };
    int runTime() {
      return finished - began;
    };
    int turnaroundTime() {
      return waitingTime() + runTime();
    };


    // Setters
    //  These functions merely serve as gateways into the private variables of
    //  this Job.  They modify internal variables.
    void setRecency(int r) {
      recency = r;
    };
    void setAdded(int clock) {
      added = clock;
    };
    void setBegin(int clock) {
      began = clock;
    };
    void setFinished(int clock) {
      finished = clock;
    };

    // Operator Definition
    //  In order for the heap functions to work properly, Job must define the
    //  > operator.  This is that definition.
		bool operator> (Job& other) {
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

    // Reality Check
    //  For some reason, a phantom job is added when reading the last line of
    //  the input file.  This flags that job as being unproocessable.
    bool real() {
      return (priority != 0);
    };
		int runtime_estimate;

	private:
		int	priority;
		int	id;
    int recency;
    int added;
    int began;
    int finished;
};

class Heap {
  // Represents a Priority queue implemented as a Heap
  //
  // Variables:
  //  data  - Holds all the data in the heap.  Instead of implenting the heap
  //          as a series of nodes and pointers, I chose a simpler array (well,
  //          technically vector) based structure.  This is that vector.
  //  n     - Holds the number of jobs currently in the heap.  Used for setting
  //          recency
  //
  public:
    // Constructor
    Heap() {
      data.clear();
      n = 0;
    };

    // Push and Pop
    //  Enqueue serves as the pusher in this heap.  It takes a given job, adds
    //  it to the back of the heap, then bubbles that job up as needed to
    //  restore the heap.
    //
    //  Dequeue does the opposite.  He pops the top job off the heap and then
    //  swaps various elements, restoring the heap property.
    void enqueue(Job& newJob) {
      newJob.setRecency(++n);
      //newJob.linePrint();
      data.push_back(newJob);
      int iNewJob = data.size()-1;
      int iParent = parent(iNewJob);

      while (iNewJob != 0 && data[iNewJob] > data[iParent]) {
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
            && (data[lChild] > data[iParent] || data[rChild] > data[iParent])) {
        iChild = (data[lChild] > data[rChild])?
          lChild:
          rChild;

        swap(iParent,iChild);

        iParent = iChild;
        lChild = 2*(iParent+1)-1;
        rChild = 2*(iParent+1);
      }
      return temp;
    };

    // Printers
    //  These debugging functions perform various printing jobs that
    //  graphically represent the current status of the heap.
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

    // Checks whether the heap is empty
    bool empty() {
      return (data.size() == 1);
    };
    
    // The input function has a nasty habbit of reading in the last element
    // job twice.  This function gets rid of the duplicate.
    void dropLast() {
      data.pop_back();
    };

  private:
    // Baby On Board Stickers
    //  These functions check whether a given node in the heap has a left,
    //  right or both children.
    bool hasLeftChild(int i) {
      return 2*(i+1) < data.size()+1;
    };
    bool hasRightChild(int i) {
      return 2*(i+1)+1 < data.size()+1;
    };
    bool isLeaf(int i) {
      return !(hasLeftChild(i) || hasRightChild(i));
    };

    // ParentTrap
    //  This function calculates the correct index of this nodes parent node
    int parent(int child) {
      if (child > 0)
        return ceil(child/2.0)-1;
      return 0;
    };

    // Swapper
    //  Swaps one node with another
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
  // Variables
  double      waiting_time = 0;
  double      turnaround_time = 0;
  long        numJobs = 0;
	long				clock = 0;
	Heap				pq;
  Job         nextJob;

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

  // Phase One
  clock = 0;
  bool done = false;
  
  for (int i = 0; i < 10; i++) {
    Job newJob;
    newJob.read(infile);
    newJob.write(outfile);
    newJob.setAdded(clock);
    pq.enqueue(newJob);
  }
  outfile << endl << endl;

  // Phase Two 
  while (!pq.empty()) {
    nextJob = pq.dequeue();
    nextJob.setBegin(clock);
    clock += nextJob.runtime_estimate;
    nextJob.setFinished(clock);
    nextJob.printProcessed(outfile);
    waiting_time += nextJob.waitingTime();
    turnaround_time += nextJob.turnaroundTime();
    numJobs++;
    while (infile && !done) {
      Job newJob;
      newJob.read(infile);
      if (newJob.real()) {
        outfile << endl;
        newJob.write(outfile);
        outfile << endl;
      }
      newJob.setAdded(clock);
      pq.enqueue(newJob);
      done = true;
    }
    done = false;
  }

  // Stats Phase
  outfile << endl << endl << "System Stats:" << endl;
  outfile << setw(30) << left << "   average waiting time: " << (waiting_time / numJobs) << " ticks" << endl
          << setw(30) << left << "   average turnaround time: " << (turnaround_time / numJobs) << " ticks" << endl
          << setw(30) << left << "   system throughput: " << (numJobs / (1.0 * clock)) << " jobs per tick" << endl;

	return 0;
}
