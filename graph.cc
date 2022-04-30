#include "graph.h"
#include <iostream>
#include <climits>
#include <stack>
#include <algorithm>
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

Graph::Graph(){};

Graph::~Graph(){
  while (!vertices.empty()) {
    map<string, Vertex *>::iterator it = vertices.begin();
    Vertex *toDelete = it->second;
    vertices.erase(it->first);
    delete toDelete;
  }

  while (!edges.empty()) {
    Edge *toDelete = edges.back();
    edges.pop_back();
    delete toDelete;
  }
};

void Graph::insertCity(string cityName) {
  if (vertices.empty()) {
    firstCity = cityName;
  }
  vertices[cityName] = new Vertex(cityName);
};

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

struct Graph::SortByDistance {
  bool operator()(Vertex* lhs, Vertex* rhs) {
    return lhs->distance > rhs->distance;
  }
} sortByDistance;


void Graph::shortestPath() {
  // First is orgin, second is predecessor
  map<string, string> predecessors;
  // Keep track of shortest distance
  vector<Vertex *> townsToVisit;
  for (map<string, Vertex *>::iterator it = vertices.begin(); it != vertices.end(); ++it) {
    if (it->first != firstCity) {
      it->second->distance = float(INT_MAX);
      townsToVisit.push_back(it->second);
    } else {
      it->second->distance = float(0);
      townsToVisit.push_back(it->second);
    }
  }
  
  Vertex *currentVertex;

  while (!townsToVisit.empty()) {
    sort(townsToVisit.begin(), townsToVisit.end(), sortByDistance);
    currentVertex = townsToVisit.back();
    townsToVisit.pop_back();

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



vector<Vertex *> Graph::depthFirstTraverse(Vertex *startVertex) {
  vector<Vertex *> DFS;
  stack<Vertex *> cityStack;
  map<Vertex *, bool> visited;
  cityStack.push(startVertex);
  int ordering = 1;
  while (!cityStack.empty()) {
    Vertex *currentVertex = cityStack.top();
    cityStack.pop();
    if (visited.find(currentVertex) == visited.end()) {
      currentVertex->distance = ordering;
      DFS.push_back(currentVertex);
      ordering++;
      visited[currentVertex] = true;
      for (int i = 0; i < currentVertex->neighborEdges.size(); i++) {
        cityStack.push(currentVertex->neighborEdges[i]->getOppositeEndpoint(currentVertex));
      }
    }
  }
  return DFS;
}

void Graph::findArticulationPoints() {
  vector<Vertex *> DFS = depthFirstTraverse(vertices[firstCity]);

}

Vertex *Edge::getOppositeEndpoint(Vertex *vertex) {
  if (vertex == endpoints[0]) {
    return endpoints[1];
  } else {
    return endpoints[0];
  }
}



