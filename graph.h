#include <string>
#include <vector>
#include <map>
#include <queue>
#include <set>
using std::set;
using std::string;
using std::vector;
using std::map;
using std::queue;


class Edge;
class Vertex;

class Graph {
  public:

    Graph();
    ~Graph();

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

    // Finds all connected components if all edges which are bridges are 
    // destroyed
    void connectedComponents();

    // Preforms worst case analysis on if any city was destroyed
    void analyzeBiconnectivity();
    
  private:

    // Recursive helper method for finding articulation points
    // params:
    // articPoints: vector passed by reference which will be 
    // currentVertex: the vertex that we are currently indexing
    // parent: the current vertex's parent
    // index: the index to count the nodes on the way down
    void findArticulationPoints(vector<Vertex *> &articPoints, 
                                  Vertex *currentVertex, int *index,
                                  Vertex *parent);

    // Recursive helper for connected component analysis
    void findComponent(vector<Vertex *> &components, Vertex *currentVertex);

    // A helper method to set all vertices "seen" attribute to false in preparation for traversal
    void setUnseen();

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

    // Constructor, create new edge between the given vertices
    Edge(Vertex *a, Vertex *b, float edgeDistance, bool edgeIsBridge);
    
    // Returns the endpoint at the other end of the edge than the passed in vertex
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
    // path and articulation point algorithm. Index 0 is the only index
    // used shortest in path. For the articulation points algorithm, index
    // 0 stores Num(v) and index 1 stores Low(v)
    float distance[2];

    // Stores whether this vertex has been seen or not for traversals
    bool seen;
};