#include <iostream>
#include <string>
#include <cstdlib>
#include "../src/Graph.cpp"
#include "../src/Common.cpp"

using namespace std;

int main(void) {
  Graph g("kyoto.in");
  map<int, int> cns;
  map<pair<int, int>, int> ces;
  double d;
  vector<Edge> path;
  int s = 435 % 1000, t = 5;
  tie(d, path) = Common::Dijkstra(g, s, t);
  cout << d << endl;
  cns[s] = 0, cns[t] = 2;
  for (auto e : path) ces[pair<int, int>(e.from, e.to)] = 1;
  g.Visualize("out.png", cns, ces);
}
