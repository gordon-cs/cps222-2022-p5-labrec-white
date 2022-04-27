#include "graph.h"
#include <iostream>
#include <climits>
#include <bits/stdc++.h>
using std::stack;
using std::cout;
using std::endl;
using std::sort;


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

void Graph::shortestPath() {
  map<string, float> distances;
  // First is orgin, second is predecessor
  map<string, string> predecessors;
  queue<Vertex *> townsToVisit = depthFirstTraverse(vertices[firstCity]);
  townsToVisit.push(vertices[firstCity]);
  for (map<string, Vertex *>::iterator it = vertices.begin(); it != vertices.end(); ++it) {
    distances[it->first] = INT_MAX;
  }

  distances[firstCity] = 0;
  
  Vertex *currentVertex;

  while (!townsToVisit.empty()) {
    currentVertex = townsToVisit.front();
    townsToVisit.pop();

    for(int i = 0; i < currentVertex->neighborEdges.size(); i++) {
      float edgeWeight = currentVertex->neighborEdges[i]->distance;
      float altPathDistance = distances[currentVertex->name] + edgeWeight;

      Vertex *adjacentVertex = currentVertex->neighborEdges[i]->getOppositeEndpoint(currentVertex);

      if (altPathDistance < distances[adjacentVertex->name]) {
        distances[adjacentVertex->name] = altPathDistance;
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
        it->first << " is " << distances[it->first] << " mi:" << endl;
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



