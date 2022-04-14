#include "graph.h"
#include <queue>
#include <iostream>
using std::queue;
using std::cout;
using std::endl;

Edge::Edge(Vertex *a, Vertex *b, int edgeDistance, bool edgeIsBridge) {
  endpoints[0] = a;
  endpoints[1] = b;
  distance = edgeDistance;
  isBridge = edgeIsBridge;
}

Vertex::Vertex(string name)
: name(name) {
}

void Graph::insertCity(string cityName) {
  vertices[cityName] = new Vertex(cityName);
};

Graph::Graph(){};
Graph::~Graph(){};

void Graph::insertRoad(string cities[2], int distance, bool isBridge) {
  Vertex *cityA = vertices[cities[0]];
  Vertex *cityB = vertices[cities[1]];
  Edge *road = new Edge(cityA, cityB, distance, isBridge);
  edges.push_back(road);
  cityA -> neighborEdges.push_back(road);
  cityB -> neighborEdges.push_back(road);
};

void Graph::breadthFirstTraverse() {
  cout << "The input data is:" << endl << endl;

  queue<Vertex *> townsToVisit;
  map<string, bool> visited;

  townsToVisit.push(vertices.begin()->second);

  while(!townsToVisit.empty()) {
    Vertex *current = townsToVisit.front();
    townsToVisit.pop();

    // Visit vertex
    cout << current->name << endl;

    // Enqueue adjacent vertices
    for (int i = 0; i < current -> neighborEdges.size(); i++) {
      // Get name of connected vertex
      // Need to check to be sure we are not getting current endpoing
      Edge *road = current -> neighborEdges[i];
      Vertex *neighborCity = road -> endpoints[0];
      if (neighborCity -> name == current -> name) {
        neighborCity -> name = road -> endpoints[1] -> name;
      }
      
      // Print road information
      cout << "        " << neighborCity -> name << " " << road -> distance << " mi"; 
      if (road -> isBridge) {
        cout << " via bridge" << endl;
      } else {
        cout << endl;
      }
      
      // Enqueue vertex
      if (visited.find(neighborCity -> name) != visited.end()) {
        townsToVisit.push(vertices[neighborCity -> name]);
        visited[neighborCity -> name] = true;
      }       
    }
  }
};