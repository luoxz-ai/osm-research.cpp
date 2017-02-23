#ifndef GRAPH_H_
#define GRAPH_H_

#include "edge.h"

#include <string>
#include <vector>

class Graph {
public:
  int V, E;
  double minlat = 1000, maxlat = -1000, minlon = 1000, maxlon = -1000;
  std::vector<Node> ns;
  std::vector<std::vector<Edge>> es;

  Graph();

  Graph (int _V);

  Graph (const std::string& filename);

  void Serialize(const std::string& filename);
};

#endif /// GRAPH_H_
