//*****************************************************************************
//   CS316           Machine Problem Extra    Joe Chrysler  
//
//   Dr. Lee                             December 15, 2010
//
//*****************************************************************************
// Classes
//*****************************************************************************
//  Symbol  - Defines structure for a symbol and functions to process it
//  Tree    - Defines structure for a self-arranging tree and functions
//            too process it
//
//*****************************************************************************
//  Global Variables
//*****************************************************************************
//  infile      - reference to the input file
//  outfile     - reference to the output file
//  value       - holding variable for the value part of an input run
//  type        - holding variable for the type part of an input run
//  trash       - input file contains several delimiter characters, this is
//                a dumping place for those characters
//  s           - The symbol currently being worked on
//  *root       - The root node of the symbol tree
//  phaseEnd    - flag indicating the end of a phase
//  error       - flag indicating that there is an error in the current symbol
//
//*****************************************************************************
//  Development History
//*****************************************************************************
//
//  This program didn't take very long at all.  I've built self-arranging tree
//  structures many times before, so that part was easy.  The symbol checking
//  was fairly easy to build too, since I had experience building similar
//  structures for CS451 - Compiler Theory.  I didn't run into any trouble
//  building this program.
//
//  Shalom,
//  Joe
//  
//*****************************************************************************
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;


class Symbol {
  // This class defines the structure for a symbol and the functions necessary
  // to process that symbol
  //
  // Variables:
  //  type    - The type of symbol this is
  //  value   - The value of this symbol
  //

  public:
    // Setters
    //  These functions set the internal variables of the symbol to default or
    //  given values
    void clear() {
      type = value = "";
    };
  void set(string inValue, string inType) {
    type = inType;
    value = inValue;
  };
  void set(string inValue) {
    value = inValue;
  };
  void setType(string inType) {
    type = inType;
  };

  // Printers
  //  These functions print various versions of the symbol to a given output
  //  file
  void print(ofstream& outfile) {
    outfile << setw(10) << left << value
            << " : "
            << type << endl;
  };
  void dprint(ofstream& outfile) {
    outfile << setw(10) << left << value << " : " << type << endl;
  };
  void error(ofstream& outfile, string message) {
    outfile << "** error " << value
            << " : " << type
            << " -- " << message
            << endl << endl;
  }

  // Getters
  //  These functions provides a way for external code to access private
  //  variables in a read-only manner
  string getType() {
    return type;
  };

  // Flaggers
  //  These functions check whether certain conditions are true or false and
  //  return the result
  bool isReserved() {
    if (value == "VAR"    ||
        value == "BEGIN"  ||
        value == "END")
      return true;
    return false;
  };
  bool isValidLength() {
    return (value.size() <= 6);
  };
  bool isValidType() {
    if (type == "BOOLEAN" ||
        type == "CHAR"    ||
        type == "INTEGER" ||
        type == "REAL")
      return true;
    return false;
  };
  bool empty() {
    return (value == "");
  };

  // Operators
  //  These define operators for use in the tree structure
  bool operator> (Symbol &other){
    return (value > other.value);
  }
  bool operator== (Symbol &other) {
    return (value == other.value);
  };

  private:
    string  type;
    string  value;
};

class Tree {
  // This class defines the structure of a self-maintaining tree node.  All
  // operations performed on any tree node will affect all subtrees of that
  // node.
  //
  // Variables:
  //  n - the data part of the node
  //  l - pointer to left child
  //  r - pointer to right child
  //
  public:
    // Constructors
    //  These functions initialize the tree with default or given values
  Tree() {
    n.clear();
    l = r = NULL;
  };

    // Add
    //  Takes a given symbol and inserts it at the proper place in the tree.
  void add(Symbol &newSymbol) {
    if (n.empty()) {
      n = newSymbol;
    }
    else {
      if (newSymbol > n) {
        if (r == NULL)
          r = new Tree();
        r->add(newSymbol);
      }
      if (n > newSymbol) {
        if (l == NULL)
          l = new Tree();
        l->add(newSymbol);
      }
    }
  };

    // Find
    //  Locates a given symbol in the tree and returns a pointer to it.
    //  Returns a null pointer if the symbol does not exist in the tree
  Tree* find(Symbol &newSymbol) {
    if (n == newSymbol)
      return this;
    if (r != NULL && newSymbol > n)
      return r->find(newSymbol);
    if (l != NULL && n > newSymbol)
      return l->find(newSymbol);
    if (n.empty())
      return NULL;
    return NULL;
  };

    // Printer
    //  Prints each symbol in the tree in order LNR
  void printInOrder(ofstream& outfile) {
    if (l != NULL)
      l->printInOrder(outfile);
    n.dprint(outfile);
    if (r != NULL)
      r->printInOrder(outfile);
  };

  public:
    Symbol  n;
    Tree    *l;
    Tree    *r;
};
int main(int argc, const char *argv[]) {
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
  
  // Variables
  string    value = "";
  string    type = "";
  string    trash = "";
  Symbol    s;
  Tree      *root = new Tree();
  bool      phaseEnd = false;
  bool      error = false;

  infile >> value;
  s.set(value);
  if (s.isReserved()) {
    outfile << "=========================" << endl;
    outfile << "Phase 1 - Symbol Building" << endl;
    outfile << "=========================" << endl;
  };

  // Phase 1
  while (infile && !phaseEnd) {
    infile >> value;
    s.set(value);
    if (s.isReserved())
      phaseEnd = true;
    else {
      infile >> trash >> type;
      type = type.substr(0, type.length()-1);
      s.setType(type);
      s.print(outfile);
      if (root->find(s) != NULL) {
        s.error(outfile, "attempt to redeclare the symbol");
        error = true;
      }
      if (!s.isValidType()) {
        s.error(outfile, "unknown type");
        error = true;
      }
      if (!s.isValidLength()) {
        s.error(outfile, "symbol too long");
        error = true;
      }
      if (!error) {
        root->add(s);
      }
    }
    error = false;
  }
  outfile << "=========================" << endl;
  outfile << "Phase 1 - End            " << endl;
  outfile << "=========================" << endl;
  phaseEnd = false;

  // Phase 2
  outfile << endl << endl;
  outfile << "=========================" << endl;
  outfile << "Phase 2 - Traversal      " << endl;
  outfile << "=========================" << endl;
  root->printInOrder(outfile);
  outfile << "=========================" << endl;
  outfile << "Phase 2 - End            " << endl;
  outfile << "=========================" << endl;
  

  // Phase 3
  outfile << endl << endl;
  outfile << "=========================" << endl;
  outfile << "Phase 3 - Processing     " << endl;
  outfile << "=========================" << endl;
  while (infile && !phaseEnd) {
    infile >> value;
    s.set(value);
    if (s.isReserved())
      phaseEnd = true;
    else {
      infile >> trash >> type;
      type = type.substr(0, type.length()-1);
      s.setType(type);
      if (root->find(s) == NULL)
        s.error(outfile, "attempt to reference undefined symbol");
      else {
        if (root->find(s)->n.getType() != s.getType())
          s.error(outfile, "type mismatch");
      }
    }
  }
  outfile << "=========================" << endl;
  outfile << "Phase 3 - End            " << endl;
  outfile << "=========================" << endl;
  return 0;
}
