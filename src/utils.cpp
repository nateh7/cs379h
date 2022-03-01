#include "utils.h"


const std::string window_title = "Gerstner Wave Pathfinder";

void
loadObj(const std::string& file, std::vector<glm::vec4>& vertices,
	std::vector<glm::uvec3>& indices, int cur_vertex)
{
	std::ifstream obj_file(file);
	std::string line;

	while (getline(obj_file, line)) {
		if (line.at(0) == 'v') {
			const char* buf = line.c_str();
			char* ptr = (char*)malloc(strlen(buf) + 1);
			strcpy(ptr, buf);
			char* token = strtok(ptr, " ");

			std::vector<float> line_vertices;
			int i = 0;
			while (token != NULL) {
				if (i > 0) {
					line_vertices.push_back(atof(token));
				}
				token = strtok(NULL, " ");
				i++;
			}


			vertices.push_back(glm::vec4(line_vertices[0], line_vertices[1], line_vertices[2], 1.0f));
			free(ptr);
		}
		else if (line.at(0) == 'f') {
			const char* buf = line.c_str();
			char* ptr = (char*)malloc(strlen(buf) + 1);
			strcpy(ptr, buf);
			char* token = strtok(ptr, " ");

			std::vector<int> line_indicies;
			int i = 0;
			while (token != NULL) {
				if (i > 0) {
					line_indicies.push_back(atoi(token));
				}
				token = strtok(NULL, " ");
				i++;
			}


			indices.push_back(glm::uvec3(cur_vertex + line_indicies[0] - 1, cur_vertex + line_indicies[1] - 1, cur_vertex + line_indicies[2] - 1));
			free(ptr);
		}
	}
}

void ErrorCallback(int error, const char* description) {
	std::cerr << "GLFW Error: " << description << "\n";
}

GLFWwindow* init_glefw()
{

	if (!glfwInit())
		exit(EXIT_FAILURE);
	glfwSetErrorCallback(ErrorCallback);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // Disable resizing, for simplicity
	glfwWindowHint(GLFW_SAMPLES, 4);
	auto ret = glfwCreateWindow(window_width, window_height, window_title.data(), nullptr, nullptr);

	glfwMakeContextCurrent(ret);
	glewExperimental = GL_TRUE;
	
	glewInit();
	glGetError();  // clear GLEW's error for it
	glfwSwapInterval(1);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	std::cout << "Renderer: " << renderer << "\n";
	std::cout << "OpenGL version supported:" << version << "\n";

	return ret;
}


void configureViewport(GLFWwindow *window) {
	glfwGetFramebufferSize(window, &window_width, &window_height);
	glViewport(0, 0, window_width, window_height);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_BACK);
}