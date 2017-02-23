#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <iomanip>
#include <cstring>
#include <vector>
#include <map>
#include "graph.h"

class Visualizer {
public:
  int PIC_SIZE = 5000;
  std::vector<std::string> COLORS = {
    "#c0392b", "#2980b9", "#16a085", "#8e44ad", "#f39c12", "#2c3e50", "#f1c40f", "#3498db", "#2ecc71", "#7f8c8d"
  };
  int pic_w, pic_h;
  Graph g;

  Visualizer(const Graph &_g) : g(_g) {
    pic_w = PIC_SIZE;
    pic_h = pic_w / ((g.maxlon - g.minlon) / 360.0) * ((g.maxlat - g.minlat) / 180.0);
  }

  void SetPicSize(int sz) {
    PIC_SIZE = sz;
    pic_w = PIC_SIZE;
    pic_h = pic_w / ((g.maxlon - g.minlon) / 360.0) * ((g.maxlat - g.minlat) / 180.0);
  }

  void ShowNodesAndEdges(const std::string& filename = "out.dot", std::map<int, int> cns = {}, std::map<Edge, int> _ces = {}) {

    std::map<Edge, int> ces;
    for (auto p : _ces) {
      Edge e = p.first;
      if (e.from > e.to) ces[(Edge){e.to, e.from, e.w}] = p.second;
      else ces[(Edge){e.from, e.to, e.w}] = p.second;
    }

    std::ofstream ofs(filename);
    ofs << "graph {" << std::endl << "  graph[size=\"" << pic_w << "," << pic_h << "\"];" << std::endl;
    ofs << "  node[fixedsize = true, width = 0.001, height = 0.001];" << std::endl;
    std::cout << "generating .dot file..." << std::endl;
    for (int i = 0; i < g.V; i++) {
      double nlat = (g.ns[i].lat - g.minlat) / (g.maxlat - g.minlat) * pic_h;
      double nlon = (g.ns[i].lon - g.minlon) / (g.maxlon - g.minlon) * pic_w;
      if (cns.count(i)) ofs << "  " << i << " [pos=\"" << nlon << "," << nlat << "\", label=\"\", style=filled, height=0.7, width=0.7, fillcolor=\""+COLORS[cns[i]]+"\", color=\""+COLORS[cns[i]]+"\"]" << std::endl;
      else ofs << "  " << i << " [pos=\"" << nlon << "," << nlat << "\", label=\"\"]" << std::endl;
    }

    for (auto edges : g.es) {
      for (auto e : edges) {
        if (e.from < e.to) {
          if (ces.count(e)) {
            std::string c = COLORS[ces[e]];
            ofs << "  " << e.from << " -- " << e.to << " [color=\""+c+"\", penwidth=10];" << std::endl;
          }
          else ofs << "  " << e.from << " -- " << e.to << ";" << std::endl;
        }
      }
    }

    ofs << "}" << std::endl << std::flush;
    std::cout << "Finished generating .dot file." << std::endl;
    // cout << "Generating .png file..." << endl;
    // system(("neato -s1 -n1 -Tpng tmp5159372184691193478.dot -o " + filename).c_str());
    // system("rm tmp5159372184691193478.dot");
  }
};
