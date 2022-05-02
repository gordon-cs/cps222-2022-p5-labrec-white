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
  // Set all vertices to unseen before traversing
  setUnseen();
  cout << "The input data is:" << endl << endl;
  queue<Vertex *> townsToVisit;

  townsToVisit.push(vertices[firstCity]);
  vertices[firstCity]->seen = true;

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
      if (!vertices[neighborCity->name]->seen) {
        townsToVisit.push(vertices[neighborCity->name]);
        vertices[neighborCity->name]->seen = true;
      }       
    }
  }
  cout << endl;
};

struct Graph::SortByDistance {
  bool operator()(Vertex* lhs, Vertex* rhs) {
    return lhs->distance[0] > rhs->distance[0];
  }
} sortByDistance;

void Graph::shortestPath() {
  // First is orgin, second is predecessor
  map<string, string> predecessors;
  // Keep track of shortest distance
  vector<Vertex *> townsToVisit;
  for (map<string, Vertex *>::iterator it = vertices.begin(); it != vertices.end(); ++it) {
    if (it->first != firstCity) {
      it->second->distance[0] = float(INT_MAX);
      townsToVisit.push_back(it->second);
    } else {
      it->second->distance[0] = float(0);
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
      float altPathDistance = currentVertex->distance[0] + edgeWeight;

      Vertex *adjacentVertex = currentVertex->neighborEdges[i]->getOppositeEndpoint(currentVertex);

      if (altPathDistance < adjacentVertex->distance[0]) {
        adjacentVertex->distance[0] = altPathDistance;
        predecessors[adjacentVertex->name] = currentVertex->name;
      }
    }
  }
  
  // Print information

  cout << "The shortest paths from " + firstCity + " are:" << endl << endl;

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
        it->first << " is " << it->second->distance[0] << " mi:" << endl;
      while (!townsOnPath.empty()) {
        cout << "        " << townsOnPath.top() << endl;
        townsOnPath.pop();
      }
    }
  }
  cout << endl;
}

void Graph::connectedComponents() {
  setUnseen();
  vector<vector<Vertex *>> components;
  for (int i = 0; i < edges.size(); i++) {
    if (edges[i]->isBridge) {
      for (int j = 0; j < 2; j++) {
        if (!edges[i]->endpoints[j]->seen) {
          vector<Vertex *> component;
          findComponent(component, edges[i]->endpoints[j]);
          components.push_back(component);
        }
      }
    }
  }

  cout << "Connected components in event of a major storm are:" << endl;
  for (int i = 0; i < components.size(); i++) {
    cout << "   If all bridges fail, the following towns would form an isolated group:" << endl;
    for (int j = 0; j < components[i].size(); j++) {
      cout << "        " << components[i][j]->name << endl;
    }
    cout << endl;
  }
}

void Graph::findComponent(vector<Vertex *> &component, Vertex *currentVertex) {
  currentVertex->seen = true;
  component.push_back(currentVertex);
  for (int i = 0; i < currentVertex->neighborEdges.size(); i++) {
    Vertex *neighbor = currentVertex->neighborEdges[i]->getOppositeEndpoint(currentVertex);
    if (!neighbor->seen) {
      if (!currentVertex->neighborEdges[i]->isBridge) {
        findComponent(component, neighbor);
      }
    }
  }
}



void Graph::setUnseen() {
  for (map<string, Vertex *>::iterator it = vertices.begin(); it != vertices.end(); ++it) {
    it->second->seen = false;
  }
}

Vertex *Edge::getOppositeEndpoint(Vertex *vertex) {
  if (vertex == endpoints[0]) {
    return endpoints[1];
  } else {
    return endpoints[0];
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
      currentVertex->distance[0] = ordering;
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

