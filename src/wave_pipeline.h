
#include <GL/glew.h>
#include "gui.h"
#include <string>
#include <vector>
#include <tuple>

void waveGLInit();
void setupWaveProgram(GUI gui);
glm::vec4 create_wave(float amplitude, float frequency, glm::vec2 direction, float speed, glm::vec2 pos, float time);
std::tuple<std::vector<glm::vec4>, std::vector<glm::uvec3>> waveGeometryInit(float t, int grid_dimension);
std::tuple<std::vector<glm::vec4>, std::vector<glm::uvec3>> waveGeometryUpdate(float t, int grid_dimension);

