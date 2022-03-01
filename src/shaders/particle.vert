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
	vec3 vertex_pos = vertex_position.xyz;

	float particle_size = 25.f;	
	gl_Position = vec4(particle_size * vertex_pos, 1);
	vs_light_direction = light_position - gl_Position;

}
)zzz"

