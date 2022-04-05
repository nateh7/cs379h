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

void create_wave(in float amplitude, in float frequency, in vec2 direction, in float speed, vec2 pos) 
{
	float q = 1 / (frequency * amplitude * 1.2);
	float partial_x = q * frequency * direction.x * amplitude * cos(dot(direction, pos) * frequency + speed * 0);
	float partial_z = q * frequency * direction.y * amplitude * cos(dot(direction, pos) * frequency + speed * 0);
	gl_Position.x += partial_x;
	gl_Position.z += partial_z;
	gl_Position.y += amplitude * sin(dot(direction, pos) * frequency + speed * 0);
}

void main() {
	
	vs_idx = idx;
	vec3 vertex_pos = vertex_position.xyz;
	gl_Position = vertex_position;
	/*
	vec2 xz_pos = vec2(vertex_position.x, vertex_position.z);
	vec2 wave_dir = vec2(1, 0);
	create_wave(.5, 1, wave_dir, 25, xz_pos);
	wave_dir = vec2(0, 1);
	create_wave(.5, 1, wave_dir, 25, xz_pos);
	wave_dir = vec2(.2, .3);
	create_wave(.5, 1, wave_dir, 90, xz_pos);
	wave_dir = vec2(-.4, -.2);
	create_wave(.45, 1, wave_dir, 5, xz_pos);*/
	
	vs_light_direction = light_position - gl_Position;

}
)zzz"

