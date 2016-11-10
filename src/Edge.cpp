#include <iostream>
#include <string>
#include <map>
#include "Node.cpp"

using namespace std;

class Edge {
public:
  int from, to;
  double w;
  // map<string, string> attrs;
};

bool operator < (const Edge &x, const Edge &y) {
  return x.from == y.from ? x.to < y.to : x.from < y.from;
}
