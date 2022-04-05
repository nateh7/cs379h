R"zzz(#version 330 core
layout (lines) in;
layout (line_strip, max_vertices = 2) out;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform vec4 light_position;
in vec4 vs_light_direction[];
in vec4 vs_camera_direction[];
out vec4 light_direction;
out vec4 camera_direction;
out vec4 world_position;
out vec4 vertex_normal;

void main() {
	int n = 0;

	for (n = 0; n < gl_in.length(); n++) {
		light_direction = normalize(vs_light_direction[n]);
		camera_direction = normalize(vs_camera_direction[n]);
		world_position = gl_in[n].gl_Position;
		vertex_normal = vs_normal[n];
		gl_Position = projection * view * model * gl_in[n].gl_Position;
		EmitVertex();
	}
	EndPrimitive();
}
)zzz"
