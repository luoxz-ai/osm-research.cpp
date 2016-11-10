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

  int PIC_SIZE = 5000;
  vector<string> COLORS = {
    "#c0392b", "#2980b9", "#16a085", "#8e44ad", "#f39c12", "#2c3e50", "#f1c40f", "#3498db", "#2ecc71", "#7f8c8d"
  };

  int pic_w, pic_h;
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
    pic_w = PIC_SIZE;
    pic_h = pic_w / ((maxlon - minlon) / 360.0) * ((maxlat - minlat) / 180.0);
  }

  void SetPicSize(int sz) {
    PIC_SIZE = sz;
    pic_w = PIC_SIZE;
    pic_h = pic_w / ((maxlon - minlon) / 360.0) * ((maxlat - minlat) / 180.0);
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


  void Visualize(const string &filename = "out.png", map<int, int> cns = {}, map<Edge, int> _ces = {}) {

    map<Edge, int> ces;
    for (auto p : _ces) {
      Edge e = p.first;
      if (e.from > e.to) ces[(Edge){e.to, e.from, e.w}] = p.second;
      else ces[(Edge){e.from, e.to, e.w}] = p.second;
    }

    ofstream ofs("tmp5159372184691193478.dot");
    ofs << "graph {" << endl << "  graph[size=\"" << pic_w << "," << pic_h << "\"];" << endl;
    ofs << "  node[fixedsize = true, width = 0.001, height = 0.001];" << endl;
    cout << "generating .dot file..." << endl;
    for (int i = 0; i < V; i++) {
      double nlat = (ns[i].lat - minlat) / (maxlat - minlat) * pic_h;
      double nlon = (ns[i].lon - minlon) / (maxlon - minlon) * pic_w;
      if (cns.count(i)) ofs << "  " << i << " [pos=\"" << nlon << "," << nlat << "\", label=\"\", style=filled, height=0.7, width=0.7, fillcolor=\""+COLORS[cns[i]]+"\", color=\""+COLORS[cns[i]]+"\"]" << endl;
      else ofs << "  " << i << " [pos=\"" << nlon << "," << nlat << "\", label=\"\"]" << endl;
    }

    for (auto edges : es) {
      for (auto e : edges) {
        if (e.from < e.to) {
          if (ces.count(e)) {
            string c = COLORS[ces[e]];
            ofs << "  " << e.from << " -- " << e.to << " [color=\""+c+"\", penwidth=10];" << endl;
          }
          else ofs << "  " << e.from << " -- " << e.to << ";" << endl;
        }
      }
    }

    ofs << "}" << endl << flush;
    cout << "Finished generating .dot file." << endl;
    cout << "Generating .png file..." << endl;
    system(("neato -s1 -n1 -Tpng tmp5159372184691193478.dot -o " + filename).c_str());
    system("rm tmp5159372184691193478.dot");
  }


};
