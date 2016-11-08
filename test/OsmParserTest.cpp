#include <iostream>
#include <string>
#include <cstdlib>
#include "../src/OsmParser.cpp"

using namespace std;

int main(void) {
  OsmParser::Parse("rokkaku.osm");
}
