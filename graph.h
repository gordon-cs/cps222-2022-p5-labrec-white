#include <string>
#include <vector>
#include <map>
#include <queue>
using std::string;
using std::vector;
using std::map;
using std::queue;


class Edge;
class Vertex;

class Graph {
  public:

    Graph();
    ~Graph(); // TODO

    // Create a new city, by default this will have no roads
    // Params:
    // cityName: the name of the city to be created
    void insertCity(string cityName);

    // Create a new road connecting two cities
    // Params:
    // cityA: the name of the city connected to cityB
    // cityB: the name of the city connected to cityA
    // distance: the "miles" or distance between cityA and cityB
    // isBridge: whether this road contains a bride or not
    void insertRoad(string cities[2], float distance, bool isBridge);

    // Traverse the cities and roads in the graph in a breadth first traversal
    void breadthFirstTraverse();

    // Uses the Dijkstra shortest path algorithm to find the greedy solution 
    // for the shortest distance from a given point to the rest of the graph
    void shortestPath();

    // Traverse the cities and roads in the graph in depth first traversal
    queue<Vertex *> depthFirstTraverse(Vertex *startVertex);
    
  private:
    // First City inputted
    string firstCity;
    // Vertex List
    map<string, Vertex *> vertices;
    // Edge List
    vector<Edge *> edges;
    // Helper structure for implementing min heap for shortest path
    // that sorts the vertices passed in by their distance
    struct SortByDistance;
};

// Helper class to store edge information to be used with class Graph
class Edge {
  public:
    friend class Graph;
    Edge(Vertex *a, Vertex *b, float edgeDistance, bool edgeIsBridge);
    
    // Returns the enpoint at the other end of the edge than the passed in vertex
    Vertex *getOppositeEndpoint(Vertex *vertex);
  
  private:
    // The distance between the vertices connected by the edge
    float distance;
    // The two vertices at both ends of the bridge
    Vertex *endpoints[2];
    // Whether the edge contains a bridge
    bool isBridge;
};

// Helper class to store vertex information to be used with class Graph
class Vertex {
  public:
    friend class Graph;
    // Constructor, creates new vertex with a set name
    Vertex(string name);
  private:
    // The name of the vertex
    string name;
    // The list of edges that connect to this vertex's neighbors
    vector<Edge *> neighborEdges;
    // Stores the distance for the particular vertex for the shortest
    // path algorithm to use
    float distance;
};