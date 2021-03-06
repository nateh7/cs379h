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
    int id;
    glm::vec4 position;
    std::set<int> neighbors;
    Vertex* prev;
    bool visited;
    float dist;
    float estimateToDestination;
};

class VertexComparator 
{
    public:
        int operator() (const Vertex& v1, const Vertex& v2) 
        {
            return v1.estimateToDestination > v2.estimateToDestination;
        }
};


class Graph {
private:
    int grid_dimension;
    float dt;
public: 
    std::vector<Vertex> vertices;
    Graph(std::vector<glm::vec4> positions, std::vector<glm::uvec3> faces, int g_dim, float delta_time);
    Vertex aStarAlgorithm(int startIdx, int endIdx, float t);
    float calculateHeuristic(Vertex start, Vertex end, float t);
    int opened_nodes;
};
