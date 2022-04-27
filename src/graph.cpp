#include "graph.h"
#include <queue>
#include <limits>
#include <stdio.h>

Graph::Graph(std::vector<glm::vec4> positions, std::vector<glm::uvec3> faces) {
    for (int i = 0; i < positions.size(); i++) {
        Vertex v;
        v.position = positions[i];
        v.id = i;
        v.dist = std::numeric_limits<float>::max();
        v.visited = false;
        v.prev = nullptr;
        vertices.push_back(v);
    }
    for (int i = 0; i < faces.size(); i++) {
        int a = faces[i][0];
        int b = faces[i][1];
        int c = faces[i][2];

        vertices[a].neighbors.insert(b);
        vertices[a].neighbors.insert(c);

        vertices[b].neighbors.insert(a);
        vertices[b].neighbors.insert(c);

        vertices[c].neighbors.insert(a);
        vertices[c].neighbors.insert(b);

    }

}

float Graph::calculateHeuristic(Vertex start, Vertex end, float t) {
    // 3D Euclidean Distance as Heuristic
    //return glm::length(start.position - end.position);
    // 3D Manhattan Distance as Heuristic
    /*
    glm::vec4 diff = start.position - end.position;
    return abs(diff.x) + abs(diff.y) + abs(diff.z) + abs(diff.w);
    */
    // No heursitic -> Dijkstra's algorithm
    return 0;
    

}

// TODO: Benchmark basic A*
Vertex Graph::aStarAlgorithm(int startIdx, int endIdx, float t) {

    Vertex startVertex = vertices[startIdx];
    Vertex endVertex = vertices[endIdx];
    opened_nodes = 0;
    vertices[startIdx].dist = 0;
    vertices[startIdx].estimateToDestination = calculateHeuristic(startVertex, endVertex, t);
    
    // TODO: order priority queue by heuristic value
    std::priority_queue<Vertex, std::vector<Vertex>, VertexComparator> nodesToVisit;
    nodesToVisit.push(vertices[startIdx]);
    while(nodesToVisit.size() > 0) {
        
        Vertex currentMin = nodesToVisit.top();
        nodesToVisit.pop();

        vertices[currentMin.id].visited = true;
        
        

        if (currentMin.id == endVertex.id) {
            for (int i = 0; i < vertices.size(); i++) {
                if(vertices[i].visited == true) {
                    opened_nodes++;
                }
            }
            return currentMin;
        }
            
        
        std::set<int> neighbors = currentMin.neighbors;

        for (int n : neighbors) {
            Vertex& neighbor = vertices[n];
            if (vertices[n].visited)
                continue;
            
            float distToNeighbor = currentMin.dist + glm::length(currentMin.position - neighbor.position);

            if (distToNeighbor >= vertices[n].dist)
                continue;
            vertices[n].prev = &vertices[currentMin.id];
            vertices[n].dist = distToNeighbor;
            //printf("dist to neighbor: %d", distToNeighbor);
            vertices[n].estimateToDestination = distToNeighbor + calculateHeuristic(vertices[n], vertices[endIdx], t);

            nodesToVisit.push(vertices[n]);
            
        }
        
    }

    
    for (int i = 0; i < vertices.size(); i++) {
        if(vertices[i].visited == true) {
            opened_nodes++;
        }
    }
    
    return endVertex;
}