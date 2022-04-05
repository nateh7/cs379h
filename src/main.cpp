#include <GL/glew.h>

#include "glm/gtx/io.hpp"
#include "surface_graph_pipeline.h"
#include "planar_graph_pipeline.h"
#include "particle_pipeline.h"
#include "wave_pipeline.h"
#include "gui.h"
#include "graph.h"

#include "utils.h"

#include <fstream>
#include <iostream>
#include <cstring>
#include <vector>

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

	std::tuple<std::vector<glm::vec4>, std::vector<glm::uvec3>> wave_geometry = waveGeometryInit(argv[1]);
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
