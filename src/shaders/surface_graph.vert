R"zzz(#version 330 core

in vec4 vertex_position;
in int idx;
uniform samplerBuffer sph_tbo_pos;
uniform vec4 light_position;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

out vec4 vs_light_direction;
out int vs_idx;

void main() {
	
	vs_idx = idx;
	gl_Position = projection * view * model * (vertex_position + vec4(0, 0.1, 0, 0));
	vs_light_direction = light_position - vertex_position;

}
)zzz"

