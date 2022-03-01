R"zzz(
#version 330 core
out vec4 fragment_color;
in vec4 face_normal;
in vec4 light_direction;
in vec4 world_position;
void main() {
	fragment_color = vec4(0, 1, 0, 1.0);
	if (world_position.x == 0) {
		fragment_color = vec4(1, 0, 0, 1.0);
	}
}
)zzz"
