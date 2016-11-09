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
    cout << endl << "Finished extracting nodes. " << endl << "Extracting edges from " + filename + "..." << endl;
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
    cout << endl << "Finished extracting edges. " << endl;
  }

  void serialize(const string &filename) {
    ofstream ofs(filename);
    ofs << setprecision(10) << fixed;
    ofs << V << " " << E << endl;
    ofs << minlat << " " << maxlat << " " << minlon << " " << maxlon << endl;
    cout << "Serializing nodes to " + filename + "..." << endl;
    for (int i = 0; i < V; i++) ofs << ns[i].lat << " " << ns[i].lon << endl;
    cout << endl << "Finished serializing nodes. " << endl << "Serializing edges from " + filename + "..." << endl;
    for (auto edges : es) for (auto e : edges) ofs << e.from << " " << e.to << " " << e.w << endl;
    cout << "Finished serializing edges. " << endl;
  }

  void visualize(const string &filename = "out.png") {
    double SCALE = 5000;
    double X = SCALE, Y = X / ((maxlon - minlon) / 360.0) * ((maxlat - minlat) / 180.0);
    cout << X << " " << Y << endl;
    ofstream ofs("tmp5159372184691193478.dot");
    ofs << "graph {" << endl << "  graph[size=\"" << X << "," << Y << "\"];" << endl;
    ofs << "  node[fixedsize = true,width = 0.0001, height = 0.0001];" << endl;
    // 0 [pos="-150,-150", label=""]
    cout << "generating .dot file..." << endl;
    for (int i = 0; i < V; i++) {
      if (ns[i].lat > maxlat or ns[i].lat < minlat) cout << minlat << " " << maxlat << " " << ns[i].lat << endl;
      double nlat = (ns[i].lat - minlat) / (maxlat - minlat) * Y;
      double nlon = (ns[i].lon - minlon) / (maxlon - minlon) * X;
      ofs << "  " << i << " [pos=\"" << nlon << "," << nlat << "\", label=\"\"]" << endl;
    }
    cout << "Finished generating .dot file." << endl;
    for (auto edges : es) for (auto e : edges) if (e.from < e.to) ofs << "  " << e.from << " -- " << e.to << ";" << endl;
    ofs << "}" << endl;
    system(("neato -s1 -n1 -Tpng tmp5159372184691193478.dot -o " + filename).c_str());
    //system("rm tmp5159372184691193478.dot");
    system(("open " + filename).c_str());
  }
};
