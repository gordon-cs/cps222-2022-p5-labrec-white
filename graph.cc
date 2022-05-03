#include "graph.h"
#include <functional>
#include <vector>
#include <set>
#include <queue>
#include <iostream>
#include <algorithm>
using std::queue;
using std::set;
using std::cout;
using std::endl;
using std::priority_queue;
using std::find;

Edge::Edge(Vertex *a, Vertex *b, float edgeDistance, bool edgeIsBridge) {
  endpoints[0] = a;
  endpoints[1] = b;
  distance = edgeDistance;
  isBridge = edgeIsBridge;
}

Vertex::Vertex(string name)
: name(name) {
}

void Graph::insertCity(string cityName) {
  if (vertices.empty()) {
    firstCity = cityName;
  }
  vertices[cityName] = new Vertex(cityName);
};

Graph::Graph(){};
Graph::~Graph(){};

void Graph::insertRoad(string cities[2], float distance, bool isBridge) {
  Vertex *cityA = vertices[cities[0]];
  Vertex *cityB = vertices[cities[1]];
  Edge *road = new Edge(cityA, cityB, distance, isBridge);
  edges.push_back(road);
  cityA->neighborEdges.push_back(road);
  cityB->neighborEdges.push_back(road);
};

void Graph::breadthFirstTraverse() {
  cout << "The input data is:" << endl << endl;

  queue<Vertex *> townsToVisit;
  map<string, bool> visited;

  townsToVisit.push(vertices[firstCity]);
  visited[firstCity] = true;

  while(!townsToVisit.empty()) {
    Vertex *current = townsToVisit.front();
    townsToVisit.pop();

    // Visit vertex
    cout << current->name << endl;
    // Enqueue adjacent vertices
    for (int i = 0; i < current->neighborEdges.size(); i++) {
      // Get name of connected vertex
      // Need to check to be sure we are not getting current vertex
      Edge *road = current->neighborEdges[i];
      Vertex *neighborCity = road->endpoints[0];
      if (neighborCity->name == current->name) {
        neighborCity = road->endpoints[1];
      }
      
      // Print road information
      cout << "        " << neighborCity->name << " " << road->distance << " mi"; 
      if (road->isBridge) {
        cout << " via bridge" << endl;
      } else {
        cout << endl;
      }
      
      // Enqueue vertex
      if (visited.find(neighborCity->name) == visited.end()) {
        townsToVisit.push(vertices[neighborCity->name]);
        visited[neighborCity->name] = true;
      }       
    }
  }
}

void Graph::minSpan() {
  map<string, Vertex *> cluster;
  map<int, vector<Edge *> > edges;
  vector<int> distances;
  auto iter = vertices.begin();
  vector<Edge *> shortestRoute;

  // Be able to order edges by order
  int counter = 0;
  int inCounter = 0;
  while(iter != vertices.end()) {
    cout << counter++ << endl;
    vector<Edge *> adjEdges = iter->second->neighborEdges;
    for (int i = 0; i < adjEdges.size(); i++) {
      edges[adjEdges[i]->distance].push_back(adjEdges[i]);
      auto inCollection = std::find(distances.begin(), 
                                    distances.end(), 
                                    adjEdges[i]->distance);
      if (inCollection == end(distances)) {
        cout << "inCount " << ++inCounter << endl;;
        distances.push_back(adjEdges[i]->distance);
      }
    }
    iter++;
  }
  cluster[firstCity] = vertices[firstCity];

  sort(distances.begin(), distances.end());

  int clusterSize = 0;
  while (!distances.empty()) {
    // Get min length
    auto edgeLenIt = distances.begin();
    int edgeLen = *edgeLenIt;
    distances.erase(edgeLenIt);
    cout << "Here" << endl;
    for (int i = 0; i < edges[edgeLen].size(); i++) {
      cout << "in" << endl;
      Edge *currentE = edges[edgeLen][i];
    

      // I think this will only iterate through once, 
      // but if a vertex was before a vertex adjacent to the cluster, 
      // it won't go back and check it
      if ((cluster.find(currentE->endpoints[0]->name) == cluster.end()) ^
          (cluster.find(currentE->endpoints[1]->name) == cluster.end())) {
            // Add edge and vertex to cluster
        shortestRoute.push_back(currentE);
        clusterSize++;
        if (cluster.find(currentE->endpoints[0]->name) == cluster.end()) {
          cluster[currentE->endpoints[0]->name] = currentE->endpoints[0];
        } else {
          cluster[currentE->endpoints[1]->name] = currentE->endpoints[1];
        }
      }
    } 
  }
    
  // Print route
  cout << 
    "The road upgrading goal can be achieved at minimal cost by upgrading: "
    << endl;
  for (int i = 0; i < shortestRoute.size(); i++) {
    cout << "        " << shortestRoute[i]->endpoints[0]->name << " to " << 
      shortestRoute[i]->endpoints[1]->name << endl;
  }
}