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

float Graph::calculateHeuristic(Vertex start, Vertex end) {
    // 3D Euclidean Distance as Heuristic
    return glm::length(start.position - end.position);
}

// TODO: Benchmark basic A*
Vertex Graph::aStarAlgorithm(int startIdx, int endIdx) {

    Vertex startVertex = vertices[startIdx];
    Vertex endVertex = vertices[endIdx];

    vertices[startIdx].dist = 0;
    vertices[startIdx].estimateToDestination = calculateHeuristic(startVertex, endVertex);
    opened_nodes = 1;
    // TODO: order priority queue by heuristic value
    std::priority_queue<Vertex, std::vector<Vertex>, VertexComparator> nodesToVisit;
    nodesToVisit.push(vertices[startIdx]);
    while(nodesToVisit.size() > 0) {
        
        Vertex currentMin = nodesToVisit.top();
        nodesToVisit.pop();

        vertices[currentMin.id].visited = true;
        

        if (currentMin.id == endVertex.id)
            return currentMin;
        
        std::set<int> neighbors = currentMin.neighbors;

        for (int n : neighbors) {
            printf("neighbor id: %d\n", n);
            Vertex& neighbor = vertices[n];
            if (vertices[n].visited)
                continue;
            
            printf("current pos: (%f, %f, %f)\n", currentMin.position.x, currentMin.position.y, currentMin.position.z);
            printf("neighbor pos: (%f, %f, %f)\n", neighbor.position.x, neighbor.position.y, neighbor.position.z);
            printf("position dist: %f\n", glm::length(currentMin.position - neighbor.position));
            float distToNeighbor = currentMin.dist + glm::length(currentMin.position - neighbor.position);

            if (distToNeighbor >= vertices[n].dist)
                continue;
            vertices[n].prev = &vertices[currentMin.id];
            vertices[n].dist = distToNeighbor;
            //printf("dist to neighbor: %d", distToNeighbor);
            vertices[n].estimateToDestination = distToNeighbor + calculateHeuristic(vertices[n], vertices[endIdx]);

            nodesToVisit.push(vertices[n]);
            opened_nodes++;
        }
        printf("opened nodes %d\n", opened_nodes);
    }

    return endVertex;
}