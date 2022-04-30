#include "graph.h"
#include <iostream>
#include <climits>
#include <stack>
using std::stack;
using std::cout;
using std::endl;
using std::priority_queue;


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
      Vertex *neighborCity = road->getOppositeEndpoint(current);
      
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
  cout << endl;
};

struct Graph::LessThanByDistance {
  bool operator()(const Vertex* lhs, const Vertex* rhs) const
  {
    return lhs->distance > rhs->distance;
  }
};


void Graph::shortestPath() {
  // First is orgin, second is predecessor
  map<string, string> predecessors;
  // Keep track of shortest distance
  priority_queue<Vertex *, vector<Vertex *>, LessThanByDistance> townsToVisit;
  for (map<string, Vertex *>::iterator it = vertices.begin(); it != vertices.end(); ++it) {
    if (it->first != firstCity) {
      it->second->distance = float(INT_MAX);
      townsToVisit.push(it->second);
    } else {
      it->second->distance = float(0);
      townsToVisit.push(it->second);
    }
  }
  
  Vertex *currentVertex;
  int index = 0;

  while (!townsToVisit.empty()) {
    currentVertex = townsToVisit.top();
    townsToVisit.pop();
    cout << index++ << " " << currentVertex->name << endl;

    for (int i = 0; i < currentVertex->neighborEdges.size(); i++) {
      float edgeWeight = currentVertex->neighborEdges[i]->distance;
      float altPathDistance = currentVertex->distance + edgeWeight;

      Vertex *adjacentVertex = currentVertex->neighborEdges[i]->getOppositeEndpoint(currentVertex);

      if (altPathDistance < adjacentVertex->distance) {
        adjacentVertex->distance = altPathDistance;
        predecessors[adjacentVertex->name] = currentVertex->name;
      }
    }
  }
  
  // Print information

  cout << "The shortest path from " + firstCity + " are:" << endl << endl;

  for (map<string, Vertex *>::iterator it = vertices.begin(); it != vertices.end(); ++it) {
    if (it->first != firstCity) {
      stack<string> townsOnPath;

      string town = it->first;
      while (town != firstCity) {
        townsOnPath.push(town);
        town = predecessors[town];
      }
      townsOnPath.push(firstCity);
      cout << "    " << "The shortest path from " << firstCity << " to " << 
        it->first << " is " << it->second->distance << " mi:" << endl;
      while (!townsOnPath.empty()) {
        cout << "        " << townsOnPath.top() << endl;
        townsOnPath.pop();
      }
    }
  }
}

queue<Vertex *> Graph::depthFirstTraverse(Vertex *startVertex) {
  queue<Vertex *> DFS;
  stack<Vertex *> cityStack;
  map<Vertex *, bool> visited;
  cityStack.push(startVertex);
  while (!cityStack.empty()) {
    Vertex *currentVertex = cityStack.top();
    cityStack.pop();
    if (visited.find(currentVertex) == visited.end()) {
      DFS.push(currentVertex);
      visited[currentVertex] = true;
      for (int i = 0; i < currentVertex->neighborEdges.size(); i++) {
        cityStack.push(currentVertex->neighborEdges[i]->getOppositeEndpoint(currentVertex));
      }
    }
  }
  return DFS;
}

Vertex *Edge::getOppositeEndpoint(Vertex *vertex) {
  if (vertex == endpoints[0]) {
    return endpoints[1];
  } else {
    return endpoints[0];
  }
}



