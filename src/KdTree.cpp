#include <iostream>
#include <vector>
#include "Graph.cpp"

using namespace std;

class KdNode {
public:
  Node n;
  KdNode *left, *right;
};

class KdTree {
public:
  // Root Node is index 0
  vector<KdNode> ns;

  KdTRee (const Graph &g) {

  }

  Node Search(double lat, double lon) {

    return Node();
  }
};
