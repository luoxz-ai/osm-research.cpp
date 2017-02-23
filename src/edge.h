#ifndef EDGE_H_
#define EDGE_H_

#include "node.h"

struct Edge {
public:
  int from, to;
  double w;
  // map<string, string> attrs;
};

bool operator < (const Edge &x, const Edge &y) {
  return x.from == y.from ? x.to < y.to : x.from < y.from;
}

#endif /// EDGE_H_
