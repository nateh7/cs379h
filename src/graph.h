#include <vector>
#include "glm/glm.hpp"

class Vertex; 

class Edge {
    Vertex* start;
    Vertex* end;
    float weight;
};

class Vertex {
    glm::vec4 position;
    std::vector<Edge> neighbors;
};






class Graph {
public: 
    std::vector<Vertex> vertices;
    Graph(std::vector<glm::vec4> positions);
};
