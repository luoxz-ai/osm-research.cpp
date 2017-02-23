#include <iostream>
#include <fstream>
#include <cstdio>
#include <iomanip>
#include <cstring>

#include "graph.h"

Graph::Graph() {;}

Graph::Graph (int _V) : V(_V) {
  ns = std::vector<Node>(V);
  es = std::vector<std::vector<Edge>>(V, std::vector<Edge>());
}

Graph::Graph (const std::string& filename) {
  std::cout << "Extracting nodes from " + filename + "..." << std::endl;
  std::ifstream ifs(filename);
  ifs >> V >> E;
  ns = std::vector<Node>(V);
  es = std::vector<std::vector<Edge>>(V, std::vector<Edge>());
  ifs >> minlat >> maxlat >> minlon >> maxlon;
  for (int i = 0; i < V; i++) {
    ifs >> ns[i].lat >> ns[i].lon;
    if (i % (V / 10) == 0) {
      std::cout << ".";
      std::cout << std::flush;
    }
  }
  std::cout << std::endl << "Extracting edges from " + filename + "..." << std::endl;
  for (int i = 0; i < E; i++) {
    int s, t;
    double w;
    ifs >> s >> t >> w;
    es[s].push_back((Edge){s, t, w});
    if (i % (E / 10) == 0) {
      std::cout << ".";
      std::cout << std::flush;
    }
  }
  std::cout << std::endl;
}

void Graph::Serialize(const std::string& filename) {
  std::ofstream ofs(filename);
  ofs << std::setprecision(10) << std::fixed;
  ofs << V << " " << E << std::endl;
  ofs << minlat << " " << maxlat << " " << minlon << " " << maxlon << std::endl;
  std::cout << "Serializing nodes to " + filename + "..." << std::endl;
  for (int i = 0; i < V; i++) ofs << ns[i].lat << " " << ns[i].lon << std::endl;
  std::cout << std::endl << "Serializing edges from " + filename + "..." << std::endl;
  for (auto edges : es) for (auto e : edges) ofs << e.from << " " << e.to << " " << e.w << std::endl;
}
