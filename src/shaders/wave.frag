R"zzz(
#version 330 core
out vec4 fragment_color;
in vec4 face_normal;
in vec4 light_direction;
in vec4 world_position;
in vec3 bary;
void main() {
	float dot_nl = clamp(dot(vec3(light_direction), vec3(face_normal)), 0, 1);
	vec3 ambient = vec3(0.2, 0.6, 1) * .5;
	vec3 diffuse = dot_nl * vec3(0.2, 0.6, 1);
	fragment_color = vec4(diffuse + ambient, 1.0);
	float min_bary = min(min(bary.x, bary.y), bary.z);
	if (min_bary < 0.03) {
		fragment_color = vec4(0, 1, 0, 1);
	}
}
)zzz"
