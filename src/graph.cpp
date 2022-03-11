#include "graph.h"
#include <queue>

Graph::Graph(std::vector<glm::vec4> positions, std::vector<glm::uvec3> faces) {
    for (int i = 0; i < positions.size(); i++) {
        Vertex v;
        v.position = positions[i];
        vertices.push_back(v);
    }

    for (int i = 0; i < faces.size(); i++) {
        int a = faces[i][0];
        int b = faces[i][1];
        int c = faces[i][2];

        Vertex va = vertices[a];
        Vertex vb = vertices[b];
        Vertex vc = vertices[c];

        va.neighbors.insert(b);
        va.neighbors.insert(c);

        vb.neighbors.insert(a);
        vb.neighbors.insert(c);

        vc.neighbors.insert(a);
        vc.neighbors.insert(b);

    }
}

Graph::calculateHeuristic(Vertex start, Vertex end) {
    // 3D Euclidean Distance as Heuristic
    return glm::length(start.position - end.position);
}

Graph::aStarAlgorithm(int startIdx, int endIdx) {

    Vertex startVertex;
    Vertex endVertex;

    startVertex.dist = 0;
    startVertex.estimateToDestination = calculateHeuristic(startVertex, endVertex);

    queue<Vertex> nodesToVisit;

    while(nodesToVisit.size() > 0) {
        Vertex currentMin = nodesToVisit.pop();

        if (currentMin == endNode)
            break;
        
        vector<Vertex> neighbors = currentMin.neighbors;

        for (Vertex neighbor : neighbors) {
            if (neighbor.visited)
                continue;
            neighbor.visited = true;
            float distToNeighbor = currentMin.dist + glm::length(currentMin.position - neighbor.position);
            if (distToNeighbor >= neighbor.dist)
                continue;
            neighbor.prev = currentMin;
            neighbor.dist = distToNeighbor;
            neighbor.estimateToDestination = distToNeighbor + calculateHeuristic(neighbor, endNode);

            nodesToVisit.insert(neighbor);
        }
    }
}