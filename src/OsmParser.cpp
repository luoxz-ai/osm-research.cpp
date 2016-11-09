#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <vector>
#include "Graph.cpp"
#include "Common.cpp"
#include "../lib/XmlInspector/XmlInspector.hpp"
#include "UnionFind.cpp"

using namespace std;

class OsmParser {

  // Get pairs<key, value> from tag <tagname k="v" ... >
  static map<string, string> GetAttributes(Xml::Inspector<Xml::Encoding::Utf8Writer> &inspector) {
    Xml::Inspector<Xml::Encoding::Utf8Writer>::SizeType i;
    map<string, string> res;
    for (i = 0; i < inspector.GetAttributesCount(); ++i) {
      res[inspector.GetAttributeAt(i).Name] = inspector.GetAttributeAt(i).Value;
    }
    return res;
  }

public:

  // Parse .osm file and
  static void Parse(const string &input_filename, const string &output_dirname = "./") {

    // Open input file
    ifstream infile(input_filename.c_str());
    if (!infile.good()) {
      cout << "Input file \'"+input_filename+"\' does not exist. " << endl;
      return;
    }

    // Get filename without extension
    if (input_filename.substr((int)input_filename.size() - 4) != ".osm") {
      cout << "Invalid filename of  \'"+input_filename+"\'. the filename must finish with \'.osm\'. " << endl;
      return;
    }
    string name_without_osm = input_filename.substr(0, (int)input_filename.size() - 4);

    // Open output file
    // TODO: Check if the output dir exists and writable
    string output_filename = output_dirname + name_without_osm + ".in";
    ofstream outfile(output_filename);


    // First: summarize graph information (node num, edge num, nodes)
    cout << "Extracting from .osm file..." << endl << "Extracting nodes..." << endl;
    typedef long long ll;
    set<ll> nids;
    map<ll, map<string, string>> metas;
    Xml::Inspector<Xml::Encoding::Utf8Writer> inspector(input_filename);
    ll id;
    map<string, string> attrs, bounds;
    map<ll, int> idtoidx;
    vector<ll> ns;
    Graph baseG;
    int baseE = 0;

    // Parsing part
    while (inspector.Inspect()) {
      auto attrs2 = GetAttributes(inspector);
      auto inspected = inspector.GetInspected();
      string name = inspector.GetName();
      // Start Tag: e.g. <node>
      if (inspected == Xml::Inspected::StartTag) {
        attrs = attrs2;
        if (name == "node") {
          id = stoll(attrs["id"]);
          nids.insert(id);
        }
      }
      // End Tag: e.g. </node>
      else if (inspected == Xml::Inspected::EndTag) {
        if (name == "node") {
          metas[id] = attrs;
        } else if (name == "way") {
          if (idtoidx.empty()) {
            int cnt = 0;
            baseG = Graph(nids.size());
            for (ll id : nids) {
              baseG.ns[cnt] = (Node){cnt, stof(metas[id]["lat"]), stof(metas[id]["lon"])};
              idtoidx[id] = cnt++;
            }
            cout << "Finished extracting nodes." << endl << "Extacting edges..." << endl;
          }
          for (int i = 0; i < (int)ns.size() - 1; i++) {
            int aidx = idtoidx[ns[i]];
            int bidx = idtoidx[ns[i + 1]];
            double dist = Common::Dist(baseG.ns[aidx], baseG.ns[bidx]);
            baseG.es[aidx].push_back((Edge){aidx, bidx, dist});
            baseG.es[bidx].push_back((Edge){bidx, aidx, dist});
          }
          baseE += (int)ns.size() - 1;
          ns.clear();
        }
      }
      // Empty Tag: e.g. <node ... />
      else if (inspected == Xml::Inspected::EmptyElementTag) {
        if (name == "node") {
          id = stoll(attrs2["id"]);
          nids.insert(id);
          metas[id] = attrs2;
        } else if (name == "tag") {
          for (auto kv : attrs2) attrs[kv.first] = kv.second;
        } else if (name == "nd"){
          ns.push_back(stoll(attrs2["ref"]));
        } else if (name == "bounds") {
          bounds = GetAttributes(inspector);
        }
      }
    }

    if (inspector.GetErrorCode() != Xml::ErrorCode::None)
    {
        std::cout << "Error: " << inspector.GetErrorMessage() <<
            " At row: " << inspector.GetRow() <<
            ", column: " << inspector.GetColumn() << ".\n";
    }

    cout << "Finished extracting graph. (nodes: " << baseG.ns.size() << ", edges: " << baseE << ")" << endl;

    // Normalizing Part
    // Find the maximum connected component
    int baseV = baseG.ns.size();
    UnionFind uf(baseV);
    for (auto edges : baseG.es) {
      for (auto e : edges) {
        uf.merge(e.from, e.to);
      }
    }
    map<int, int> mp;
    for (int i = 0; i < baseV; i++) mp[uf.root(i)]++;
    int V = 0, root = -1;
    for (auto p : mp) if (p.second > V) {
      V = p.second;
      root = p.first;
    }

    // Build a new graph
    int E = 0;
    Graph g(V);
    map<int, int> tonidx;
    int cnt = 0;
    // assign new ids
    for (int i = 0; i < baseV; i++) {
      if (uf.root(i) == root) {
        tonidx[i] = cnt;
        g.ns[cnt++] = baseG.ns[i];
      }
    }
    // span edges
    for (int i = 0; i < baseV; i++) {
      if (uf.root(i) == root) {
        for (auto e : baseG.es[i]) {
          int from = tonidx[i], to = tonidx[e.to];
          g.es[from].push_back((Edge){from, to, e.w});
          E++;
        }
      }
    }

    g.E = E;
    for (auto nd : g.ns) {
      g.minlat = min(g.minlat, nd.lat - 0.001);
      g.maxlat = max(g.maxlat, nd.lat + 0.001);
      g.minlon = min(g.minlon, nd.lon - 0.001);
      g.maxlon = max(g.maxlon, nd.lon + 0.001);
    }

    cout << g.minlat << " " << g.maxlon << endl;

    cout << "Finished normalizing graph. (nodes: " << V << ", edges: " << E << ")" << endl;
    cout << "Serializing graph..." << endl;

    g.serialize(output_filename);
    cout << "Finished serializing graph." << endl;
    // g.visualize();
  }
};
