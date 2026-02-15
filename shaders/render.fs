#version 330 core
in vec3 normal;
out vec4 FragColor;

uniform vec4 ambient;

void main() {
	FragColor = vec4((vec3(1.0, 1.0, 1.0) + normal) / 2.0, 1.0);
}

