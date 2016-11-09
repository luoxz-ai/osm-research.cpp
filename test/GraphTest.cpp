#include <iostream>
#include <string>
#include <cstdlib>
#include "../src/Graph.cpp"

using namespace std;

int main(void) {
  Graph g("kyoto.in");
  g.visualize();
}
