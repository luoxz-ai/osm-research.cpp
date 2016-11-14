#include <iostream>
#include <string>
#include <cstdlib>
#include "../src/Visualizer.cpp"
#include "../src/Common.cpp"

using namespace std;

void test_visualizer(Graph &g);

string area, func;

int main(int argc, char *argv[]) {
  area = argv[1], func = argv[2];
  Graph g(dpath + area + ".in");
  if (func == "visualizer") {
    test_visualizer(g);
  }
  return 0;
}

void test_visualizer(Graph &g) {
  map<int, int> cns;
  map<Edge, int> ces;
  double d;
  vector<Edge> path;
  int s = 435, t = 5;
  tie(d, path) = Common::Dijkstra(g, s, t);
  cns[s] = 0, cns[t] = 2;
  for (auto e : path) ces[e] = 1;
  Visualizer v(g);
  v.ShowNodesAndEdges(area + ".dot", cns, ces);
}
