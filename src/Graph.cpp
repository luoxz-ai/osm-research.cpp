#include <iostream>
#include <string>
#include <fstream>
#include "Edge.cpp"

using namespace std;

class Graph {
public:
  int V, E, minlat, maxlat, minlon, maxlon;
  vector<Node> ns;
  vector<vector<Edge>> es;

  Graph(){;}

  Graph (int _V) : V(_V) {
    ns = vector<Node>(V);
    es = vector<vector<Edge>>(V, vector<Edge>());
  }

  void serialize(const string &filename) {
    ofstream ofs(filename);
    ofs << V << " " << E << endl;
    for (int i = 0; i < V; i++) ofs << ns[i].lat << " " << ns[i].lon << endl;
    for (auto edges : es) for (auto e : edges) ofs << e.from << " " << e.to << " " << e.w << endl;
  }

  void deserialize(const string &filename) {

  }
};
