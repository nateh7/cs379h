#include "surface_graph_pipeline.h"
#include "utils.h"
#include "graph.h"

GLuint surfaceGraphVAO;
GLuint surface_graph_program_id = 0;
enum {kVertexBuffer, kIndexBuffer, kNumVbos};
GLuint surface_graph_buffer_objects[kNumVbos];

std::vector<glm::vec4> surface_graph_vertices;
std::vector<GLuint> surface_graph_faces;

glm::vec4 surface_graph_light_position(0.0f, 0.0f, 110.0f, 1.0f);

MatrixPointers surface_graph_mats;
GLint surface_graph_projection_matrix_location = 0;
GLint surface_graph_view_matrix_location = 0;
GLint surface_graph_model_matrix_location = 0;
GLint surface_graph_light_position_location = 0;

const char* surface_graph_vertex_shader =
#include "shaders/surface_graph.vert"
;

const char* surface_graph_geometry_shader =
#include "shaders/surface_graph.geom"
;

const char* surface_graph_fragment_shader =
#include "shaders/surface_graph.frag"
;

void surfaceGraphGeometryInit(std::vector<glm::vec4> path_vertices) {
	for (int i = 0; i < path_vertices.size(); i++) {
		surface_graph_vertices.push_back(path_vertices[i]);
		surface_graph_faces.push_back(i);
		if (i < path_vertices.size() - 1) {
			surface_graph_faces.push_back(i + 1);
		}
	}
}

void surfaceGraphGLInit() {
	/* Initialize graph OpenGL Program */
	
	// Setup VAO for graph program
	glGenVertexArrays(1, &surfaceGraphVAO);
	// Bind VAO for graph program
	glBindVertexArray(surfaceGraphVAO);
	
	// Generate VBOs for graph program
	glGenBuffers(kNumVbos, &surface_graph_buffer_objects[0]);

	// Setup graph vertex data in VBO
	glBindBuffer(GL_ARRAY_BUFFER, surface_graph_buffer_objects[kVertexBuffer]);
	// Describe vertex data to OpenGL
	glBufferData(GL_ARRAY_BUFFER, 
				sizeof(float) * surface_graph_vertices.size() * 4, surface_graph_vertices.data(),
				GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);


	// Setup element array buffer. 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, surface_graph_buffer_objects[kIndexBuffer]);
	// Describe elemnt array buffer to OpenGL
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
					sizeof(GLuint) * surface_graph_faces.size(),
					surface_graph_faces.data(), GL_STATIC_DRAW);

	// Setup vertex shader.
	GLuint vertex_shader_id = 0;
	const char* vertex_source_pointer = surface_graph_vertex_shader;
	vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader_id, 1, &vertex_source_pointer, nullptr);
	glCompileShader(vertex_shader_id);


	// Setup geometry shader.
	GLuint geometry_shader_id = 0;
	const char* geometry_source_pointer = surface_graph_geometry_shader;
	geometry_shader_id = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometry_shader_id, 1, &geometry_source_pointer, nullptr);
	glCompileShader(geometry_shader_id);


	// Setup fragment shader.
	GLuint fragment_shader_id = 0;
	const char* fragment_source_pointer = surface_graph_fragment_shader;
	fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader_id, 1, &fragment_source_pointer, nullptr);
	glCompileShader(fragment_shader_id);

	// Setup program for the graphs, and get its locations.
	surface_graph_program_id = glCreateProgram();
	glAttachShader(surface_graph_program_id, vertex_shader_id);
	//glAttachShader(surface_graph_program_id, geometry_shader_id);
	glAttachShader(surface_graph_program_id, fragment_shader_id);

	// Bind attributes.
	glBindAttribLocation(surface_graph_program_id, 0, "vertex_position");
	glBindAttribLocation(surface_graph_program_id, 1, "idx");
	glBindFragDataLocation(surface_graph_program_id, 0, "fragment_color");
	glLinkProgram(surface_graph_program_id);


	// Get the uniform locations.
	surface_graph_projection_matrix_location =
		glGetUniformLocation(surface_graph_program_id, "projection");


	surface_graph_view_matrix_location = 
		glGetUniformLocation(surface_graph_program_id, "view");

	
	surface_graph_model_matrix_location = 
		glGetUniformLocation(surface_graph_program_id, "model");

	surface_graph_light_position_location = 
		glGetUniformLocation(surface_graph_program_id, "light_position");

}

void setupSurfaceGraphProgram(GUI gui) {
	// Switch VAO
	glBindVertexArray(surfaceGraphVAO);

	// Switch Program
	glUseProgram(surface_graph_program_id);
	
	// Pass uniforms
	surface_graph_mats = gui.getMatrixPointers();

	glUniformMatrix4fv(surface_graph_projection_matrix_location, 1, GL_FALSE,
		&((*surface_graph_mats.projection)[0][0]));

	glUniformMatrix4fv(surface_graph_view_matrix_location, 1, GL_FALSE,
		&((*surface_graph_mats.view)[0][0]));
	
	glUniformMatrix4fv(surface_graph_model_matrix_location, 1, GL_FALSE,
		&((*surface_graph_mats.model)[0][0]));
	
	glUniform4fv(surface_graph_light_position_location, 1, &surface_graph_light_position[0]);
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glDrawElements(GL_LINES, surface_graph_faces.size(), GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}