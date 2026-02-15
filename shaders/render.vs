#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in int boneID;

const int MAX_BONES = 100;
uniform mat4 boneMatricies[MAX_BONES];

out vec3 normal;

uniform mat4 projection;
uniform mat4 view;

void main() {
	mat4 model = boneMatricies[boneID];
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	normal = aNormal;
}

