#include <fstream>
#include <iostream>
#include <cstring>
#include <vector>
#include "glm/gtx/io.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void
loadObj(const std::string& file, std::vector<glm::vec4>& vertices,
	std::vector<glm::uvec3>& indices, int cur_vertex);

void ErrorCallback(int error, const char* description);
GLFWwindow* init_glefw();
void configureViewport(GLFWwindow *window);

static int window_width = 960;
static int window_height = 720;;

