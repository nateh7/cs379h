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
int cur_vertex_id = 100;
const int end_vertex_id = 300;
float total_execution_time = 0.f;

void rewindOneTimeStep() {
	printf("calling rewind\n");
	std::tuple<std::vector<glm::vec4>, std::vector<glm::uvec3>> wave_geometry = waveGeometryUpdate(--time_step);
	std::vector<glm::vec4> wave_vertices = std::get<0>(wave_geometry);
	std::vector<glm::uvec3> wave_faces = std::get<1>(wave_geometry);
	Graph g(wave_vertices, wave_faces);
	printf("number of vertices: %d\n", wave_vertices.size());
	printf("wave vertex 0: (%f, %f, %f)\n", wave_vertices[0].x, wave_vertices[0].y, wave_vertices[0].z);
	printf("wave vertex 30: (%f, %f, %f)\n", wave_vertices[30].x, wave_vertices[30].y, wave_vertices[30].z);
	Vertex goal = g.aStarAlgorithm(100, 300);

	Vertex* current = &goal;
	std::vector<glm::vec4> path_vertices;
	while (current != nullptr) {
		path_vertices.push_back(current->position);
		printf("path position: (%f, %f, %f)\n", current->position.x, current->position.y, current->position.z);
		current = current->prev;
	}
	surfaceGraphGeometryUpdate(path_vertices);
}

void advanceOneTimeStep() {
	printf("calling advance\n");
	t += 0.001f;
	time_step++;
	std::tuple<std::vector<glm::vec4>, std::vector<glm::uvec3>> wave_geometry = waveGeometryUpdate(t);
	std::vector<glm::vec4> wave_vertices = std::get<0>(wave_geometry);
	std::vector<glm::uvec3> wave_faces = std::get<1>(wave_geometry);
	Graph g(wave_vertices, wave_faces);
	printf("number of vertices: %d\n", wave_vertices.size());
	printf("wave vertex 0: (%f, %f, %f)\n", wave_vertices[0].x, wave_vertices[0].y, wave_vertices[0].z);
	printf("wave vertex 30: (%f, %f, %f)\n", wave_vertices[30].x, wave_vertices[30].y, wave_vertices[30].z);
	clock_t start = clock();
	Vertex goal = g.aStarAlgorithm(cur_vertex_id, end_vertex_id);
	double execution_time =  (double) (clock() - start) / CLOCKS_PER_SEC;

	total_opened_nodes += g.opened_nodes;
	total_execution_time += execution_time;
	printf("A* execution time at time step %d: %f\n", execution_time, time_step);
	Vertex* current = &goal;
	std::vector<glm::vec4> path_vertices;
	while (current != nullptr) {
		path_vertices.push_back(current->position);
		if (current->prev != nullptr && current->prev->id == cur_vertex_id) {
			printf("current id: %d\n", current->id);
			cur_vertex_id = current->id;
			break;
		}
		printf("vertex id: %d\n", current->id);
		//printf("path position: (%f, %f, %f)\n", current->position.x, current->position.y, current->position.z);
		current = current->prev;
	}
	if (cur_vertex_id == end_vertex_id) {
		printf("number of time steps to dest: %d\n", time_step);
		printf("number of vertices in graph: %d\n", g.vertices.size());
		printf("total A* computation time: %f\n", total_execution_time);
		printf("total number of opened nodes: %d\n", total_opened_nodes);
		printf("maximum possible number of opened nodes: %d\n", g.vertices.size() * time_step);
		
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


	//particleGeometryInit(argv[1]);
	//particleGLInit();

	std::tuple<std::vector<glm::vec4>, std::vector<glm::uvec3>> wave_geometry = waveGeometryInit(0);
	std::vector<glm::vec4> wave_vertices = std::get<0>(wave_geometry);
	std::vector<glm::uvec3> wave_faces = std::get<1>(wave_geometry);

	waveGLInit();

	
	Graph g(wave_vertices, wave_faces);
	printf("number of vertices: %d\n", wave_vertices.size());
	printf("wave vertex 0: (%f, %f, %f)\n", wave_vertices[0].x, wave_vertices[0].y, wave_vertices[0].z);
	printf("wave vertex 30: (%f, %f, %f)\n", wave_vertices[30].x, wave_vertices[30].y, wave_vertices[30].z);
	Vertex goal = g.aStarAlgorithm(100, 300);

	Vertex* current = &goal;
	std::vector<glm::vec4> path_vertices;
	while (current != nullptr) {
		path_vertices.push_back(current->position);
		printf("path position: (%f, %f, %f)\n", current->position.x, current->position.y, current->position.z);
		current = current->prev;
	}

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
