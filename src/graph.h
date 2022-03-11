#include <set>
#include <vector>
#include "glm/glm.hpp"

class Vertex; 

class Edge {
    Vertex* start;
    Vertex* end;
    float weight;
};

class Vertex {
public:
    glm::vec4 position;
    std::set<int> neighbors;
    Vertex prev;
    bool visited;
    float dist;
    float estimateToDestination;
};



class Graph {
public: 
    std::vector<Vertex> vertices;
    Graph(std::vector<glm::vec4> positions, std::vector<glm::uvec3> faces);
    aStarAlgorithm(int startIdx, int endIdx);
    calculateHeuristic(Vertex start, Vertex end);
};
