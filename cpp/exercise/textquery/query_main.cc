#include <fstream>
#include <iostream>
#include <sstream>

#include "make_plural.h"
#include "text_query.h"

using namespace std;

void runQueries(ifstream &infile) {
  TextQuery tq(infile);
  while (true) {
    cout << "enter word to look for, or q to quit:";
    string s;
    if (!(cin >> s) || s == "q") break;
    print(cout, tq.query(s)) << endl;
  }
}

int main(int argc, char **argv) {
  ifstream infile;
  if (argc < 2 || !(infile.open(argv[1]), infile)) {
    cerr << "No input file!" << endl;
    return 1;
  }
  runQueries(infile);
  return 0;
}