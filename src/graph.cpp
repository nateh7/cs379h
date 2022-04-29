#include "graph.h"
#include <queue>
#include <limits>
#include <stdio.h>
#include "wave_pipeline.h"

Graph::Graph(std::vector<glm::vec4> positions, std::vector<glm::uvec3> faces, int g_dim, float delta_time) {
    grid_dimension = g_dim;
    dt = delta_time;
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
    
    /*
    
    // Predicted position of start vertex at next time step on Surface #1 
    int start_id = start.id;
    float start_grid_x = start_id % grid_dimension - (grid_dimension / 2);
    float start_grid_z = start_id / grid_dimension - (grid_dimension / 2);

    glm::vec2 start_xz_pos(start_grid_x, start_grid_z);
    glm::vec4 predicted_start_pos(start_grid_x, -5, start_grid_z, 1);
        glm::vec2 wave_dir(1, 0);
        predicted_start_pos += create_wave(.5, 1, wave_dir, 25, start_xz_pos, t + dt);
        wave_dir = glm::vec2(0, 1);
        predicted_start_pos += create_wave(.5, 1, wave_dir, 25, start_xz_pos, t + dt);
        wave_dir = glm::vec2(.55, .83);
        predicted_start_pos += create_wave(.5, 0.2, wave_dir, 90, start_xz_pos, t + dt);
        wave_dir = glm::vec2(-.89, -.45);
        predicted_start_pos += create_wave(.45, 0.1, wave_dir, 5, start_xz_pos, t + dt);
    
    // Predicted position of end vertex at next time step on Surface #1

    int end_id = end.id;
    float end_grid_x = end_id % grid_dimension - (grid_dimension / 2);
    float end_grid_z = end_id / grid_dimension - (grid_dimension / 2);
    glm::vec2 end_xz_pos(end_grid_x, end_grid_z);
    glm::vec4 predicted_end_pos(end_grid_x, -5, end_grid_z, 1);
        wave_dir = glm::vec2(1, 0);
        predicted_end_pos += create_wave(.5, 1, wave_dir, 25, end_xz_pos, t + dt);
        wave_dir = glm::vec2(0, 1);
        predicted_end_pos += create_wave(.5, 1, wave_dir, 25, end_xz_pos, t + dt);
        wave_dir = glm::vec2(.55, .83);
        predicted_end_pos += create_wave(.5, 0.2, wave_dir, 90, end_xz_pos, t + dt);
        wave_dir = glm::vec2(-.89, -.45);
        predicted_end_pos += create_wave(.45, 0.1, wave_dir, 5, end_xz_pos, t + dt);


    //printf("start %d position at current time step: (%f, %f, %f)\n", start_id, start.position.x, start.position.y, start.position.z);
    //printf("start %d position at next time step: (%f, %f, %f)\n", start_id, predicted_start_pos.x, predicted_start_pos.y, predicted_start_pos.z);
    //printf("end position at current time step: (%f, %f, %f)\n", end.position.x, end.position.y, end.position.z);
    //printf("end position at next time step: (%f, %f, %f)\n", predicted_end_pos.x, predicted_end_pos.y, predicted_end_pos.z);
    // Time-Advanced 3D Euclidean Distance Heuristic for Surface #1
    // return glm::length(predicted_start_pos - predicted_end_pos);
    // Time-Advanced 3D Manhattan Distance Heuristic for Surface #1
    glm::vec4 diff = predicted_start_pos - predicted_end_pos;
    return abs(diff.x) + abs(diff.y) + abs(diff.z);
*/

    // 3D Euclidean Distance as Heuristic
    return glm::length(start.position - end.position);
    // 3D Manhattan Distance as Heuristic
    /*
    glm::vec4 diff = start.position - end.position;
    return abs(diff.x) + abs(diff.y) + abs(diff.z);*/
    
    // No heursitic -> Dijkstra's algorithm
    //return 0;
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