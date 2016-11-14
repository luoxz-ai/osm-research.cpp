#include <iostream>
#include <cmath>
#include <queue>
#include <algorithm>

#define rad2deg(a) ((a)/M_PI * 180.0) /* rad を deg に換算するマクロ関数 */
#define deg2rad(a) ((a)/180.0 * M_PI) /* deg を rad に換算するマクロ関数 */

using namespace std;

// Setting
string dpath = "../data/";


class Common {
public:

  // Return distance[m] between node a and b
  static double Dist(Node a, Node b) {
    double PI = 3.141592653589793;
    double P = ( a.lat+b.lat ) / 2 * PI / 180;
    double dP = ( a.lat-b.lat ) * PI / 180;
    double dR = ( a.lon-b.lon ) * PI / 180;
    double M = 6334834 / sqrt( ( 1-0.006674 * sin( P ) * sin( P ) ) * ( 1-0.006674 * sin( P ) * sin( P ) ) * ( 1-0.006674 * sin( P ) * sin( P ) ) );
    double N = 6377397 / sqrt( 1-0.006674 * sin( P ) * sin( P ) );
    double D = sqrt( ( M * dP ) * ( M * dP ) + ( N * cos( P ) * dR ) * ( N * cos( P ) * dR ) );
    return D;
  }

  // Return the nearest node from lat/lon on g
  static double GetNN(double lat, double lon) {
    return -1;
  }

  // Get Shortest Path from s to t with Dijkstra
  static pair<int, vector<Edge>> Dijkstra(const Graph &g, int s, int t) {
    vector<double> d(g.V, 1e18);
    d[s] = 0;
    typedef pair<double,int> P;
    priority_queue<P, vector<P>, greater<P> > que;
    que.push(P(0, s));
    while (!que.empty()) {
      double dist = que.top().first;
      int v = que.top().second;
      que.pop();
      if (d[v] < dist) continue;
      for (auto e : g.es[v]) {
        if (d[e.to] > d[v] + e.w) {
          d[e.to] = d[v] + e.w;
          que.push(P(d[e.to], e.to));
        }
      }
    }
    vector<Edge> path;
    int now = t;
    while(now != s) {
      for (auto e : g.es[now]) {
        if (abs(e.w + d[e.to] - d[now]) < 1e-8) {
          path.push_back(e);
          now = e.to;
        }
      }
    }
    reverse(path.begin(), path.end());
    for (auto &e : path) swap(e.from, e.to);
    return pair<int, vector<Edge>>(d[t], path);
  }

};
