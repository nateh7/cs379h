#include "wave_pipeline.h"
#include "utils.h"
#include "graph.h"

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

void waveGeometryInit(const std::string& file) {
    int cur_square = 0;
	
    for (int y = -10; y <= 10; y++) {
		for (int x = -10; x <= 10; x++) {
            wave_vertices.push_back(glm::vec4(x, -5, y, 1));
        }
    }
	
	for (int i = 0; i < wave_vertices.size() - 23; i++) {
		if (i % 21 < 19) {
			wave_faces.push_back(glm::uvec3(i + 21, i + 1, i ));
			wave_faces.push_back(glm::uvec3(i + 22, i + 1, i + 21));
		}
		
		
	}
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

	Graph g(wave_vertices, wave_faces);
	printf("number of vertices: %d\n", wave_vertices.size());
	printf("wave vertex 0: (%f, %f, %f)\n", wave_vertices[0].x, wave_vertices[0].y, wave_vertices[0].z);
	printf("wave vertex 30: (%f, %f, %f)\n", wave_vertices[30].x, wave_vertices[30].y, wave_vertices[30].z);
	Vertex goal = g.aStarAlgorithm(0, 30);
	printf("dist: %f\n", g.vertices[30].prev->estimateToDestination);

	/*
	for (int i = 0; i < 10; i++) {
		printf("vertex %d neighbors: \n", i);
		for(int n : g.vertices[i].neighbors) {
			printf("vertex %d neighbor: %d\n", i, n);
		}
	}*/

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