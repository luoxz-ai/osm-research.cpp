#include <iostream>
#include <string>
#include <cstdlib>
#include "../src/Graph.cpp"
#include "../src/Common.cpp"

using namespace std;

int main(void) {
  Graph g("kyoto.in");
  map<int, int> cns;
  map<Edge, int> ces;
  double d;
  vector<Edge> path;
  int s = 435, t = 5;
  tie(d, path) = Common::Dijkstra(g, s, t);
  cns[s] = 0, cns[t] = 2;
  for (auto e : path) ces[e] = 1;
  g.Visualize("out.png", cns, ces);
}
