#include <GL/glew.h>

#include "glm/gtx/io.hpp"
#include "surface_graph_pipeline.h"
#include "planar_graph_pipeline.h"
#include "particle_pipeline.h"
#include "wave_pipeline.h"
#include "gui.h"
#include "graph.h"
#include "main.h"
#include "utils.h"

#include <fstream>
#include <iostream>
#include <cstring>
#include <vector>
#include <time.h>

int time_step = 0;
int total_opened_nodes = 0;
float t = 0;
int grid_dimension = 81;
int num_edges = 2 * (grid_dimension - 1) * (grid_dimension - 1);
int cur_vertex_id = grid_dimension - 1;
const int end_vertex_id = grid_dimension*grid_dimension-grid_dimension;
float total_execution_time = 0.f;
float dt = 0.001f;
float total_dist = 0.f;

void rewindOneTimeStep() {
	/*
	std::tuple<std::vector<glm::vec4>, std::vector<glm::uvec3>> wave_geometry = waveGeometryUpdate(--time_step, grid_dimension);
	std::vector<glm::vec4> wave_vertices = std::get<0>(wave_geometry);
	std::vector<glm::uvec3> wave_faces = std::get<1>(wave_geometry);
	Graph g(wave_vertices, wave_faces);

	Vertex goal = g.aStarAlgorithm(100, 300, t);

	Vertex* current = &goal;
	std::vector<glm::vec4> path_vertices;
	while (current != nullptr) {
		path_vertices.push_back(current->position);
		current = current->prev;
	}
	surfaceGraphGeometryUpdate(path_vertices);*/
}

void advanceOneTimeStep() {
	t += dt;
	time_step++;
	std::tuple<std::vector<glm::vec4>, std::vector<glm::uvec3>> wave_geometry = waveGeometryUpdate(t, grid_dimension);
	std::vector<glm::vec4> wave_vertices = std::get<0>(wave_geometry);
	std::vector<glm::uvec3> wave_faces = std::get<1>(wave_geometry);
	Graph g(wave_vertices, wave_faces, grid_dimension, dt);

	clock_t start = clock();
	Vertex goal = g.aStarAlgorithm(cur_vertex_id, end_vertex_id, t);
	double execution_time =  (double) (clock() - start) / CLOCKS_PER_SEC;

	total_opened_nodes += g.opened_nodes;
	total_execution_time += execution_time;
	printf("A* execution time at time step %d: %f seconds\n", time_step, execution_time);
	printf("Number of opened nodes at time step %d: %d\n", time_step, g.opened_nodes);
	Vertex* current = &goal;
	std::vector<glm::vec4> path_vertices;
	while (current != nullptr) {
		/*
		printf("vertex id: %d\n", current->id);
		printf("path position: (%f, %f, %f)\n", current->position.x, current->position.y, current->position.z);*/

		path_vertices.push_back(current->position);
		if (current->prev != nullptr && current->prev->id == cur_vertex_id) {
			//printf("current id: %d\n", current->id)
			// TODO: calculate total distance traveled on time-varying shortest path
			float dist = glm::length(g.vertices[current->id].position - g.vertices[cur_vertex_id].position);
			printf("Distance traveled at time step %d: %f\n", time_step, dist);
			total_dist += dist;
			cur_vertex_id = current->id;
			break;
		}
		
		current = current->prev;
	}
	if (cur_vertex_id == end_vertex_id) {
		printf("===================================\n");
		printf("Number of time steps to destination: %d\n", time_step);
		printf("Number of vertices in graph: %ld\n", g.vertices.size());
		printf("Number of edges in graph: %d\n", num_edges);
		printf("Total A* computation time: %f seconds\n", total_execution_time);
		printf("Total number of opened nodes: %d\n", total_opened_nodes);
		printf("Maximum possible number of opened nodes: %ld\n", g.vertices.size() * time_step);
		printf("Total distance traveled: %f units\n", total_dist);
		
	}
	surfaceGraphGeometryUpdate(path_vertices);

}


int main(int argc, char* argv[])
{
	if (argc < 2) {
		std::cerr << "Input model file is missing" << std::endl;
		std::cerr << "Usage: " << argv[0] << " <OBJ file>" << std::endl;
		return -1;
	}

	GLFWwindow *window = init_glefw();
	GUI gui(window, window_width, window_height);

	std::tuple<std::vector<glm::vec4>, std::vector<glm::uvec3>> wave_geometry = waveGeometryInit(0, grid_dimension);
	std::vector<glm::vec4> wave_vertices = std::get<0>(wave_geometry);
	std::vector<glm::uvec3> wave_faces = std::get<1>(wave_geometry);

	waveGLInit();

	
	Graph g(wave_vertices, wave_faces, grid_dimension, dt);

	clock_t start = clock();
	Vertex goal = g.aStarAlgorithm(cur_vertex_id, end_vertex_id, t);
	double execution_time =  (double) (clock() - start) / CLOCKS_PER_SEC;

	// Use backpointers to create vector of path vertices
	Vertex* current = &goal;
	std::vector<glm::vec4> path_vertices;
	while (current != nullptr) {
		path_vertices.push_back(current->position);
		current = current->prev;
	}

	// compute length of path from vertex positions
	float path_length = 0.0f;
	for(int i = 0; i < path_vertices.size() - 1; i++) {
		path_length += glm::length(path_vertices[i] - path_vertices[i + 1]);
	}

	printf("Number of vertices in graph: %ld\n", g.vertices.size());
	printf("Number of edges in graph: %d\n", num_edges);
	printf("A* execution time on initial surface: %f seconds\n", execution_time);
	printf("Number of opened nodes on initial surface: %d\n", g.opened_nodes);
	printf("Length of shortest path: %f units\n", path_length);

	surfaceGraphGeometryInit(path_vertices);
	surfaceGraphGLInit();
	/*
	graphGeometryInit(argv[1]);
	graphGLInit();*/

	

	
	while (!glfwWindowShouldClose(window)) {
		configureViewport(window);

		gui.updateMatrices();
		
		//setupParticleProgram(gui);
		setupWaveProgram(gui);
		
		setupSurfaceGraphProgram(gui);
		//setupGraphProgram(gui);
	
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
