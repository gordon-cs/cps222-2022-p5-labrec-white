#include "graph.h"
#include <functional>
#include <vector>
#include <set>
#include <queue>
#include <iostream>
#include <algorithm>
#include <climits>
#include <stack>
using std::queue;
using std::set;
using std::cout;
using std::endl;
using std::priority_queue;
using std::find;
using std::stack;
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
  queue<Vertex *> citiesToVisit;

  citiesToVisit.push(vertices[firstCity]);
  vertices[firstCity]->seen = true;

  while(!citiesToVisit.empty()) {
    Vertex *current = citiesToVisit.front();
    citiesToVisit.pop();

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
        citiesToVisit.push(vertices[neighborCity->name]);
        vertices[neighborCity->name]->seen = true;
      }       
    }
  }
}

void Graph::minSpan() {
  map<string, Vertex *> cluster;
  set<Edge *> adjToCluster;
  vector<Edge *> spanningRoads;

  Vertex * currentVertex = vertices[firstCity];

  while (cluster.size() < vertices.size()) {
    
    cluster[currentVertex->name] = currentVertex;
    for (int i = 0; i < edges.size(); i++) {
      if (edges[i]->containsVertex(currentVertex)) {
        adjToCluster.erase(edges[i]);
      }
    }
    // Add adjacent edges of currentVertex
    for (int i = 0; i < currentVertex->neighborEdges.size(); i++) {
      if (cluster.find(currentVertex->neighborEdges[i]->endpoints[0]->name) == cluster.end() ^
      cluster.find(currentVertex->neighborEdges[i]->endpoints[1]->name) == cluster.end()) {
        adjToCluster.insert(currentVertex->neighborEdges[i]);
      }
    }
    
    // Choose shortest adjacent edge
    Edge *lowEdge = *adjToCluster.begin();
    for (set<Edge *>::iterator it = adjToCluster.begin(); it != adjToCluster.end(); ++it) {
      if (lowEdge->distance > (*it)->distance) {
        lowEdge = *it;
      }
    }
    spanningRoads.push_back(lowEdge);

    if (cluster.find(lowEdge->endpoints[0]->name) == cluster.end()) {
      cluster[lowEdge->endpoints[0]->name] = lowEdge->endpoints[0];
      currentVertex = lowEdge->endpoints[0];
    } else {
      cluster[lowEdge->endpoints[1]->name] = lowEdge->endpoints[1];
      currentVertex = lowEdge->endpoints[1];
    }
  }

  // Print route
  cout << 
    "The road upgrading goal can be achieved at minimal cost by upgrading:"
    << endl;
  for (int i = 0; i < spanningRoads.size(); i++) {
    cout << "        " << spanningRoads[i]->endpoints[0]->name << " to " << 
      spanningRoads[i]->endpoints[1]->name << endl;
  }
}

struct Graph::SortByDistance {
  bool operator()(Vertex* lhs, Vertex* rhs) {
    return lhs->distance[0] > rhs->distance[0];
  }
} sortByDistance;

void Graph::shortestPath() {
  // First is orgin, second is predecessor
  map<string, string> predecessors;
  // Keep track of shortest distance
  vector<Vertex *> citiesToVisit;
  for (map<string, Vertex *>::iterator it = vertices.begin(); it != vertices.end(); ++it) {
    if (it->first != firstCity) {
      it->second->distance[0] = float(INT_MAX);
      citiesToVisit.push_back(it->second);
    } else {
      it->second->distance[0] = float(0);
      citiesToVisit.push_back(it->second);
    }
  }
  
  Vertex *currentVertex;

  while (!citiesToVisit.empty()) {
    sort(citiesToVisit.begin(), citiesToVisit.end(), sortByDistance);
    currentVertex = citiesToVisit.back();
    citiesToVisit.pop_back();

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
      stack<string> citiesOnPath;

      string city = it->first;
      while (city != firstCity) {
        citiesOnPath.push(city);
        city = predecessors[city];
      }
      citiesOnPath.push(firstCity);
      cout << "    " << "The shortest path from " << firstCity << " to " << 
        it->first << " is " << it->second->distance[0] << " mi:" << endl;
      while (!citiesOnPath.empty()) {
        cout << "        " << citiesOnPath.top() << endl;
        citiesOnPath.pop();
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
    cout << "   If all bridges fail, the following cities would form an isolated group:" << endl;
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

bool Edge::containsVertex(Vertex *vertex) {
  for (int i = 0; i < 2; i++) {
    if (endpoints[i] == vertex) {
      return true;
    }
  }
  return false;
}