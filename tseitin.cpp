/*
Tseitin transformation of a CNF boolean formula with a single XOR clause to an
equivalent CNF formula (with additional variables)

Input file must be in DIMACS format other than:
The XOR clause must appear after all CNF clauses, and the XOR clause must be
denoted by an 'x ' at the beginning of each XOR line (in VV, the clause is of
the form x1 xor ... xor xj xor 1)

The number of clauses should be the number of CNF clauses plus 1 (to account for
the XOR clause)
*/

#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

//argv[1] = input file name
//argv[2] = output file name
int main(int argc, char* argv[]) {
  assert(argc == 3);
  
  ifstream in(argv[1]);
  string s;
  getline(in, s);
  while (s[0] == 'c') getline(in, s);
  istringstream is(s);
  vector<string> vs;
  string word;
  while (is >> word) vs.push_back(word);
  assert(vs[0] == "p" and vs[1] == "cnf" and vs.size() == 4);
  
  cerr << "Input parsing begun" << endl;
  
  int var = atoi(vs[2].c_str());
  int cls = atoi(vs[3].c_str());
  
  cerr << var << " " << cls << endl;
  
  for (int i = 0; i < cls - 1; ++i) {
    getline(in, s);
    cerr << s << endl;
  }
  
  cerr << "Input parsing continuing" << endl;
  
  vector<int> x;
  int v;
  in >> s;
  assert(s == "x");
  while (in >> v) {
    cerr << v << " ";
    x.push_back(v);
  }
  cerr << endl;
  x.pop_back(); //remove trailing 0 in XOR line
  
  for (int i = 0; i < x.size(); ++i) cerr << x[i] << " "; cerr << endl;
  
  cerr << "Input parsing finished" << endl;
  
  in.close();
  in.open(argv[1]);
  getline(in, s);
  while (s[0] == 'c') getline(in, s);
  ofstream out(argv[2]);
  out << "p cnf " << var + x.size() - 1 << " " << cls + 4*(x.size()-1) << endl;
  
  cerr << "Output begun" << endl;
  
  for (int i = 0; i < cls - 1; ++i) {
    getline(in, s);
    out << s << endl;
  }
  
  cerr << "Output CNF clauses" << endl;
  
  /*
  (x1 xor x2 xor ... xor xk xor 1) transforms to
  (y1 iff x1 xor x2) and (y2 iff y1 xor x3) and ... and
  (yk-1 iff yk-2 xor xk) and (yk-1 xor 1)
  
  x iff y xor z = !(x xor y xor z)
  = (!x or !y or z) and (!x or y or z) and (x or !y or !z) and (!x or !y or !z)
  */
  
  cerr << var << " " << cls << " " << x.size() << endl;
  for (int i = 0; i < x.size(); ++i) cerr << x[i] << " "; cerr << endl;
  
  cerr << var+1 << " " << x[0] << " " << x[1] << endl;
  out << (-1)*(var+1) << " " << (-1)*x[0] << " " << x[1] << " 0\n";
  out << (-1)*(var+1) << " " << x[0] << " " << (-1)*x[1] << " 0\n";
  out << var+1 << " " << (-1)*x[0] << " " << (-1)*x[1] << " 0\n";
  out << (-1)*(var+1) << " " << (-1)*x[0] << " " << (-1)*x[1] << " 0\n";
  for (int i = 1; i < x.size() - 1; ++i) {
    cerr << var+i+1 << " " << var+i << " " << x[i+1] << endl;
    out << (-1)*(var+i+1) << " " << (-1)*(var+i) << " " << x[i+1] << " 0\n";
    out << (-1)*(var+i+1) << " " << var+i << " " << (-1)*x[i+1] << " 0\n";
    out << var+1 << " " << (-1)*(var+i) << " " << (-1)*x[i+1] << " 0\n";
    out << (-1)*(var+i+1) << " " << (-1)*(var+i) << " " << (-1)*x[i+1] <<" 0\n";
  }
  cerr << var << " " << x.size()-1 << endl;
  int size = x.size();
  out << (-1)*(var + size - 1) << " 0\n";
  out.close();
  
  return 0;
}
