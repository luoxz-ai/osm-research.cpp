#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <iomanip>
#include <cstring>
#include <vector>
#include "Edge.cpp"

using namespace std;

class Graph {
public:
  int V, E;
  double minlat = 1000, maxlat = -1000, minlon = 1000, maxlon = -1000;
  vector<Node> ns;
  vector<vector<Edge>> es;

  Graph(){;}

  Graph (int _V) : V(_V) {
    ns = vector<Node>(V);
    es = vector<vector<Edge>>(V, vector<Edge>());
  }

  Graph (const string &filename) {
    cout << "Extracting nodes from " + filename + "..." << endl;
    ifstream ifs(filename);
    ifs >> V >> E;
    ns = vector<Node>(V);
    es = vector<vector<Edge>>(V, vector<Edge>());
    ifs >> minlat >> maxlat >> minlon >> maxlon;
    for (int i = 0; i < V; i++) {
      ifs >> ns[i].lat >> ns[i].lon;
      if (i % (V / 10) == 0) {
        cout << ".";
        cout << flush;
      }
    }
    cout << endl << "Extracting edges from " + filename + "..." << endl;
    for (int i = 0; i < E; i++) {
      int s, t;
      double w;
      ifs >> s >> t >> w;
      es[s].push_back((Edge){s, t, w});
      if (i % (E / 10) == 0) {
        cout << ".";
        cout << flush;
      }
    }
    cout << endl;
  }

  void Serialize(const string &filename) {
    ofstream ofs(filename);
    ofs << setprecision(10) << fixed;
    ofs << V << " " << E << endl;
    ofs << minlat << " " << maxlat << " " << minlon << " " << maxlon << endl;
    cout << "Serializing nodes to " + filename + "..." << endl;
    for (int i = 0; i < V; i++) ofs << ns[i].lat << " " << ns[i].lon << endl;
    cout << endl << "Serializing edges from " + filename + "..." << endl;
    for (auto edges : es) for (auto e : edges) ofs << e.from << " " << e.to << " " << e.w << endl;
  }
};
