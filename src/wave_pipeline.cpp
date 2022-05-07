#include "wave_pipeline.h"
#include "utils.h"


GLuint waveVAO;
GLuint wave_program_id = 0;
enum {kVertexBuffer, kIndexBuffer, kNumVbos};
GLuint wave_buffer_objects[kNumVbos];

std::vector<glm::vec4> wave_vertices;
std::vector<glm::uvec3> wave_faces;

glm::vec4 wave_light_position(0.0f, 0.0f, 110.0f, 1.0f);

MatrixPointers wave_mats;
GLint wave_projection_matrix_location = 0;
GLint wave_view_matrix_location = 0;
GLint wave_model_matrix_location = 0;
GLint wave_light_position_location = 0;

const char* wave_vertex_shader =
#include "shaders/wave.vert"
;

const char* wave_geometry_shader =
#include "shaders/wave.geom"
;

const char* wave_fragment_shader =
#include "shaders/wave.frag"
;

glm::vec4 create_wave(float amplitude, float frequency, glm::vec2 direction, float speed, glm::vec2 pos, float time) 
{
	float steepness = 1.2;
	float q = 1 / (frequency * amplitude * steepness);
	float partial_x = q * frequency * direction.x * amplitude * cos(dot(direction, pos) * frequency + speed * time);
	float partial_z = q * frequency * direction.y * amplitude * cos(dot(direction, pos) * frequency + speed * time);
	return glm::vec4(partial_x, amplitude * sin(dot(direction, pos) * frequency + speed * time), partial_z, 0);
}

std::tuple<std::vector<glm::vec4>, std::vector<glm::uvec3>> waveGeometryInit(float t, int grid_dimension) {
	
	
	int min_z = -(grid_dimension / 2);
	int max_z = (grid_dimension / 2);
	int min_x = -(grid_dimension / 2);
	int max_x = (grid_dimension / 2);
    for (int z = min_z; z <= max_z; z+=1) {
		for (int x = min_x; x <= max_x; x+=1) {
			glm::vec4 wave_vertex = glm::vec4(x, -5, z, 1);
			glm::vec2 xz_pos(x, z);

// Surface #1 
 
			glm::vec2 wave_dir(1, 0);
			wave_vertex += create_wave(.5, 1, wave_dir, 25, xz_pos, t);
			wave_dir = glm::vec2(0, 1);
			wave_vertex += create_wave(.5, 1, wave_dir, 25, xz_pos, t);
			wave_dir = glm::vec2(.55, .83);
			wave_vertex += create_wave(.5, 0.2, wave_dir, 90, xz_pos, t);
			wave_dir = glm::vec2(-.89, -.45);
			wave_vertex += create_wave(.45, 0.1, wave_dir, 5, xz_pos, t);

// Surface #2
/*
			glm::vec2 wave_dir(1, 0);
			wave_vertex += create_wave(.5, 1, wave_dir, 25, xz_pos, t);
*/
// Surface #3
/*
			glm::vec2 wave_dir(1, 0);
			wave_vertex += create_wave(2, 1, wave_dir, 25, xz_pos, t);
			wave_dir = glm::vec2(0, 1);
			wave_vertex += create_wave(2, 1, wave_dir, 25, xz_pos, t);
			wave_dir = glm::vec2(.55, .83);
			wave_vertex += create_wave(.5, 0.2, wave_dir, 90, xz_pos, t);
			wave_dir = glm::vec2(-.89, -.45);
			wave_vertex += create_wave(.45, 0.1, wave_dir, 5, xz_pos, t);
*/
// Surface #4
/*
			glm::vec2 wave_dir(1, 0);
			wave_vertex += create_wave(0.2, 1, wave_dir, 25, xz_pos, t);
			wave_dir = glm::vec2(0, 1);
			wave_vertex += create_wave(0.2, 1, wave_dir, 25, xz_pos, t);
			wave_dir = glm::vec2(.55, .83);
			wave_vertex += create_wave(.1, 0.2, wave_dir, 90, xz_pos, t);
			wave_dir = glm::vec2(-.89, -.45);
			wave_vertex += create_wave(.1, 0.1, wave_dir, 5, xz_pos, t);
*/
			wave_vertices.push_back(wave_vertex);
        }
    }
	
	for (int i = 0; i <= wave_vertices.size() - (grid_dimension + 2); i++) {
		if (i % grid_dimension <= (grid_dimension - 2)) {
			wave_faces.push_back(glm::uvec3(i + grid_dimension, i + 1, i));
			wave_faces.push_back(glm::uvec3(i + grid_dimension + 1, i + 1, i + grid_dimension));
		}	
	}

	return std::make_tuple(wave_vertices, wave_faces);
}


std::tuple<std::vector<glm::vec4>, std::vector<glm::uvec3>> waveGeometryUpdate(float t, int grid_dimension) {
	wave_vertices.clear();
	wave_faces.clear();

    waveGeometryInit(t, grid_dimension);
	// Setup wave vertex data in VBO
	glBindBuffer(GL_ARRAY_BUFFER, wave_buffer_objects[kVertexBuffer]);
	// Describe vertex data to OpenGL
	glBufferData(GL_ARRAY_BUFFER, 
				sizeof(float) * wave_vertices.size() * 4, wave_vertices.data(),
				GL_STATIC_DRAW);

		// Setup element array buffer. 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wave_buffer_objects[kIndexBuffer]);
	// Describe elemnt array buffer to OpenGL
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
					sizeof(uint32_t) * wave_faces.size() * 3,
					wave_faces.data(), GL_STATIC_DRAW);

	return std::make_tuple(wave_vertices, wave_faces);
}

void waveGLInit() {
	/* Initialize wave OpenGL Program */
	
	// Setup VAO for wave program
	glGenVertexArrays(1, &waveVAO);
	// Bind VAO for wave program
	glBindVertexArray(waveVAO);
	
	// Generate VBOs for wave program
	glGenBuffers(kNumVbos, &wave_buffer_objects[0]);

	// Setup wave vertex data in VBO
	glBindBuffer(GL_ARRAY_BUFFER, wave_buffer_objects[kVertexBuffer]);
	// Describe vertex data to OpenGL
	glBufferData(GL_ARRAY_BUFFER, 
				sizeof(float) * wave_vertices.size() * 4, wave_vertices.data(),
				GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);


	// Setup element array buffer. 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wave_buffer_objects[kIndexBuffer]);
	// Describe elemnt array buffer to OpenGL
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
					sizeof(uint32_t) * wave_faces.size() * 3,
					wave_faces.data(), GL_STATIC_DRAW);

	// Setup vertex shader.
	GLuint vertex_shader_id = 0;
	const char* vertex_source_pointer = wave_vertex_shader;
	vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader_id, 1, &vertex_source_pointer, nullptr);
	glCompileShader(vertex_shader_id);


	// Setup geometry shader.
	GLuint geometry_shader_id = 0;
	const char* geometry_source_pointer = wave_geometry_shader;
	geometry_shader_id = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometry_shader_id, 1, &geometry_source_pointer, nullptr);
	glCompileShader(geometry_shader_id);


	// Setup fragment shader.
	GLuint fragment_shader_id = 0;
	const char* fragment_source_pointer = wave_fragment_shader;
	fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader_id, 1, &fragment_source_pointer, nullptr);
	glCompileShader(fragment_shader_id);

	// Setup program for the waves, and get its locations.
	wave_program_id = glCreateProgram();
	glAttachShader(wave_program_id, vertex_shader_id);
	glAttachShader(wave_program_id, geometry_shader_id);
	glAttachShader(wave_program_id, fragment_shader_id);

	// Bind attributes.
	glBindAttribLocation(wave_program_id, 0, "vertex_position");
	glBindAttribLocation(wave_program_id, 1, "idx");
	glBindFragDataLocation(wave_program_id, 0, "fragment_color");
	glLinkProgram(wave_program_id);


	// Get the uniform locations.
	wave_projection_matrix_location =
		glGetUniformLocation(wave_program_id, "projection");


	wave_view_matrix_location = 
		glGetUniformLocation(wave_program_id, "view");

	
	wave_model_matrix_location = 
		glGetUniformLocation(wave_program_id, "model");

	wave_light_position_location = 
		glGetUniformLocation(wave_program_id, "light_position");

}

void setupWaveProgram(GUI gui) {

	// Switch VAO
	glBindVertexArray(waveVAO);

	// Switch Program
	glUseProgram(wave_program_id);
	
	// Pass uniforms
	wave_mats = gui.getMatrixPointers();

	glUniformMatrix4fv(wave_projection_matrix_location, 1, GL_FALSE,
		&((*wave_mats.projection)[0][0]));

	glUniformMatrix4fv(wave_view_matrix_location, 1, GL_FALSE,
		&((*wave_mats.view)[0][0]));
	
	glUniformMatrix4fv(wave_model_matrix_location, 1, GL_FALSE,
		&((*wave_mats.model)[0][0]));
	
	glUniform4fv(wave_light_position_location, 1, &wave_light_position[0]);

	glDrawElements(GL_TRIANGLES, 3 * wave_faces.size(), GL_UNSIGNED_INT, 0);
}