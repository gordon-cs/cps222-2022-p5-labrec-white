#include <iostream>
#include "graph.cc"
using std::cin;
using std::endl;
using std::istream;


bool eof(istream &input) {
  bool done = false;
  char c;
  input >> c;
  if (input.eof()) {
    done = true;
  } else {
    input.unget();
  }
  return done;
}

int main() {
  vector<Graph *> graphs;
  while (!eof(cin)) {
    Graph *graph = new Graph();
    int cityNum;
    int roadNum;
    cin >> cityNum;
    cin >> roadNum; 

    for (int i = 0; i < cityNum; i++) {
      string city;
      cin >> city;
      graph->insertCity(city);
    }

    for (int i = 0; i < roadNum; i++) {
      string city1;
      string city2;
      
      cin >> city1;
      cin >> city2;

      string *cities = new string[2];
      cities[0] = city1;
      cities[1] = city2;

      char bridge;
      bool bridgeBool = true;
      cin >> bridge;
      if (bridge == 'N') {
        bridgeBool = false;
      }
      float distance;
      cin >> distance;
      graph->insertRoad(cities, distance, bridgeBool);
    }
    graphs.push_back(graph);
  }

  for (int i = 0; i < graphs.size(); i++) {
    graphs[i]->breadthFirstTraverse();
    graphs[i]->shortestPath();
    graphs[i]->minSpan();
    graphs[i]->connectedComponents();
    graphs[i]->analyzeBiconnectivity();
    cout << endl;
    cout << "------------------------------------------------------------------";
    cout << endl << endl;
  }
  return 0;
}
