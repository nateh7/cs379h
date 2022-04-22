
#include <GL/glew.h>
#include "gui.h"
#include <string>
#include <vector>
#include <tuple>

void waveGLInit();
void setupWaveProgram(GUI gui);
std::tuple<std::vector<glm::vec4>, std::vector<glm::uvec3>> waveGeometryInit(float t);
std::tuple<std::vector<glm::vec4>, std::vector<glm::uvec3>> waveGeometryUpdate(float t);
