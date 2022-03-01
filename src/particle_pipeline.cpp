#include "particle_pipeline.h"
#include "utils.h"

GLuint particleVAO;
GLuint particle_program_id = 0;
enum {kVertexBuffer, kIndexBuffer, kNumVbos};
GLuint particle_buffer_objects[kNumVbos];

std::vector<glm::vec4> particle_vertices;
std::vector<glm::uvec3> particle_faces;

glm::vec4 light_position(0.0f, 0.0f, 110.0f, 1.0f);

MatrixPointers mats;
GLint projection_matrix_location = 0;
GLint view_matrix_location = 0;
GLint model_matrix_location = 0;
GLint light_position_location = 0;

const char* particle_vertex_shader =
#include "shaders/particle.vert"
;

const char* particle_geometry_shader =
#include "shaders/particle.geom"
;

const char* particle_fragment_shader =
#include "shaders/particle.frag"
;

void particleGeometryInit(const std::string& file) {
	loadObj(file, particle_vertices, particle_faces, 0); 
}

void particleGLInit() {
	/* Initialize particle OpenGL Program */
	
	// Setup VAO for particle program
	glGenVertexArrays(1, &particleVAO);
	// Bind VAO for particle program
	glBindVertexArray(particleVAO);
	
	// Generate VBOs for particle program
	glGenBuffers(kNumVbos, &particle_buffer_objects[0]);

	// Setup particle vertex data in VBO
	glBindBuffer(GL_ARRAY_BUFFER, particle_buffer_objects[kVertexBuffer]);
	// Describe vertex data to OpenGL
	glBufferData(GL_ARRAY_BUFFER, 
				sizeof(float) * particle_vertices.size() * 4, particle_vertices.data(),
				GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);


	// Setup element array buffer. 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, particle_buffer_objects[kIndexBuffer]);
	// Describe elemnt array buffer to OpenGL
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
					sizeof(uint32_t) * particle_faces.size() * 3,
					particle_faces.data(), GL_STATIC_DRAW);

	// Setup vertex shader.
	GLuint vertex_shader_id = 0;
	const char* vertex_source_pointer = particle_vertex_shader;
	vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader_id, 1, &vertex_source_pointer, nullptr);
	glCompileShader(vertex_shader_id);


	// Setup geometry shader.
	GLuint geometry_shader_id = 0;
	const char* geometry_source_pointer = particle_geometry_shader;
	geometry_shader_id = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometry_shader_id, 1, &geometry_source_pointer, nullptr);
	glCompileShader(geometry_shader_id);


	// Setup fragment shader.
	GLuint fragment_shader_id = 0;
	const char* fragment_source_pointer = particle_fragment_shader;
	fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader_id, 1, &fragment_source_pointer, nullptr);
	glCompileShader(fragment_shader_id);

	// Setup program for the particles, and get its locations.
	particle_program_id = glCreateProgram();
	glAttachShader(particle_program_id, vertex_shader_id);
	glAttachShader(particle_program_id, geometry_shader_id);
	glAttachShader(particle_program_id, fragment_shader_id);

	// Bind attributes.
	glBindAttribLocation(particle_program_id, 0, "vertex_position");
	glBindAttribLocation(particle_program_id, 1, "idx");
	glBindFragDataLocation(particle_program_id, 0, "fragment_color");
	glLinkProgram(particle_program_id);


	// Get the uniform locations.
	projection_matrix_location =
		glGetUniformLocation(particle_program_id, "projection");


	view_matrix_location = 
		glGetUniformLocation(particle_program_id, "view");

	
	model_matrix_location = 
		glGetUniformLocation(particle_program_id, "model");

	light_position_location = 
		glGetUniformLocation(particle_program_id, "light_position");

}

void setupParticleProgram(GUI gui) {
	// Switch VAO
	glBindVertexArray(particleVAO);

	// Switch Program
	glUseProgram(particle_program_id);
	
	// Pass uniforms
	mats = gui.getMatrixPointers();

	glUniformMatrix4fv(projection_matrix_location, 1, GL_FALSE,
		&((*mats.projection)[0][0]));

	glUniformMatrix4fv(view_matrix_location, 1, GL_FALSE,
		&((*mats.view)[0][0]));
	
	glUniformMatrix4fv(model_matrix_location, 1, GL_FALSE,
		&((*mats.model)[0][0]));
	
	glUniform4fv(light_position_location, 1, &light_position[0]);

	glDrawElements(GL_TRIANGLES, 3 * particle_faces.size(), GL_UNSIGNED_INT, 0);
}