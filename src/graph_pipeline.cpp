#include "graph_pipeline.h"
#include "utils.h"

GLuint graphVAO;
GLuint graph_program_id = 0;
enum {kVertexBuffer, kIndexBuffer, kNumVbos};
GLuint graph_buffer_objects[kNumVbos];

std::vector<glm::vec4> graph_vertices;
std::vector<glm::uvec3> graph_faces;

glm::vec4 graph_light_position(0.0f, 0.0f, 110.0f, 1.0f);

MatrixPointers graph_mats;
GLint graph_projection_matrix_location = 0;
GLint graph_view_matrix_location = 0;
GLint graph_model_matrix_location = 0;
GLint graph_light_position_location = 0;

const char* graph_vertex_shader =
#include "shaders/graph.vert"
;

const char* graph_geometry_shader =
#include "shaders/graph.geom"
;

const char* graph_fragment_shader =
#include "shaders/graph.frag"
;

void graphGeometryInit(const std::string& file) {
    int cur_square = 0;
	for (int i = -10; i < 10; i++) {
        for (int j = -10; j < 10; j++) {
            graph_vertices.push_back(glm::vec4(1 * i, 5, 1 * j, 1));
            graph_vertices.push_back(glm::vec4(1 * (i + 1), 5, 1 * j, 1));
            graph_vertices.push_back(glm::vec4(1 * i, 5, 1 * (j + 1), 1));
            graph_vertices.push_back(glm::vec4(1 * (i + 1), 5, 1 * (j + 1), 1));

            graph_faces.push_back(glm::uvec3(cur_square * 4 + 2, cur_square * 4 + 1, cur_square * 4));
            graph_faces.push_back(glm::uvec3(cur_square * 4 + 3, cur_square * 4 + 1, cur_square * 4 + 2));
            cur_square++;
        }
    }
}

void graphGLInit() {
	/* Initialize graph OpenGL Program */
	
	// Setup VAO for graph program
	glGenVertexArrays(1, &graphVAO);
	// Bind VAO for graph program
	glBindVertexArray(graphVAO);
	
	// Generate VBOs for graph program
	glGenBuffers(kNumVbos, &graph_buffer_objects[0]);

	// Setup graph vertex data in VBO
	glBindBuffer(GL_ARRAY_BUFFER, graph_buffer_objects[kVertexBuffer]);
	// Describe vertex data to OpenGL
	glBufferData(GL_ARRAY_BUFFER, 
				sizeof(float) * graph_vertices.size() * 4, graph_vertices.data(),
				GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);


	// Setup element array buffer. 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, graph_buffer_objects[kIndexBuffer]);
	// Describe elemnt array buffer to OpenGL
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
					sizeof(uint32_t) * graph_faces.size() * 3,
					graph_faces.data(), GL_STATIC_DRAW);

	// Setup vertex shader.
	GLuint vertex_shader_id = 0;
	const char* vertex_source_pointer = graph_vertex_shader;
	vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader_id, 1, &vertex_source_pointer, nullptr);
	glCompileShader(vertex_shader_id);


	// Setup geometry shader.
	GLuint geometry_shader_id = 0;
	const char* geometry_source_pointer = graph_geometry_shader;
	geometry_shader_id = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometry_shader_id, 1, &geometry_source_pointer, nullptr);
	glCompileShader(geometry_shader_id);


	// Setup fragment shader.
	GLuint fragment_shader_id = 0;
	const char* fragment_source_pointer = graph_fragment_shader;
	fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader_id, 1, &fragment_source_pointer, nullptr);
	glCompileShader(fragment_shader_id);

	// Setup program for the graphs, and get its locations.
	graph_program_id = glCreateProgram();
	glAttachShader(graph_program_id, vertex_shader_id);
	glAttachShader(graph_program_id, geometry_shader_id);
	glAttachShader(graph_program_id, fragment_shader_id);

	// Bind attributes.
	glBindAttribLocation(graph_program_id, 0, "vertex_position");
	glBindAttribLocation(graph_program_id, 1, "idx");
	glBindFragDataLocation(graph_program_id, 0, "fragment_color");
	glLinkProgram(graph_program_id);


	// Get the uniform locations.
	graph_projection_matrix_location =
		glGetUniformLocation(graph_program_id, "projection");


	graph_view_matrix_location = 
		glGetUniformLocation(graph_program_id, "view");

	
	graph_model_matrix_location = 
		glGetUniformLocation(graph_program_id, "model");

	graph_light_position_location = 
		glGetUniformLocation(graph_program_id, "light_position");

}

void setupGraphProgram(GUI gui) {
	// Switch VAO
	glBindVertexArray(graphVAO);

	// Switch Program
	glUseProgram(graph_program_id);
	
	// Pass uniforms
	graph_mats = gui.getMatrixPointers();

	glUniformMatrix4fv(graph_projection_matrix_location, 1, GL_FALSE,
		&((*graph_mats.projection)[0][0]));

	glUniformMatrix4fv(graph_view_matrix_location, 1, GL_FALSE,
		&((*graph_mats.view)[0][0]));
	
	glUniformMatrix4fv(graph_model_matrix_location, 1, GL_FALSE,
		&((*graph_mats.model)[0][0]));
	
	glUniform4fv(graph_light_position_location, 1, &graph_light_position[0]);
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glDrawElements(GL_TRIANGLES, 3 * graph_faces.size(), GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}