#include <GL/glew.h>

#include "glm/gtx/io.hpp"
#include "surface_graph_pipeline.h"
#include "planar_graph_pipeline.h"
#include "particle_pipeline.h"
#include "wave_pipeline.h"
#include "gui.h"

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

	waveGeometryInit(argv[1]);
	waveGLInit();

	graphGeometryInit(argv[1]);
	graphGLInit();

	

	
	while (!glfwWindowShouldClose(window)) {
		configureViewport(window);

		gui.updateMatrices();
		
		//setupParticleProgram(gui);
		setupWaveProgram(gui);
		setupGraphProgram(gui);
	
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
