#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <iomanip>
#include "Edge.cpp"

using namespace std;

class Graph {
public:
  int V, E;
  double minlat, maxlat, minlon, maxlon;
  vector<Node> ns;
  vector<vector<Edge>> es;

  Graph(){;}

  Graph (int _V) : V(_V) {
    ns = vector<Node>(V);
    es = vector<vector<Edge>>(V, vector<Edge>());
  }

  Graph (const string &filename) {
    ifstream ifs(filename);
    ifs >> V >> E;
    ns = vector<Node>(V);
    es = vector<vector<Edge>>(V, vector<Edge>());
    for (int i = 0; i < V; i++) ifs >> ns[i].lat >> ns[i].lon;
    for (int i = 0; i < E; i++) {
      int s, t;
      double w;
      cin >> s >> t >> w;
      es[s].push_back((Edge){s, t, w});
    }
  }

  void serialize(const string &filename) {
    ofstream ofs(filename);
    ofs << setprecision(10) << fixed;
    ofs << V << " " << E << endl;
    for (int i = 0; i < V; i++) ofs << ns[i].lat << " " << ns[i].lon << endl;
    for (auto edges : es) for (auto e : edges) ofs << e.from << " " << e.to << " " << e.w << endl;
  }
};
