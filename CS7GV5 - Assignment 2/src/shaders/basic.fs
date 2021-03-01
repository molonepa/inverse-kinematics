#version 400

uniform vec3 u_BaseColour;

out vec4 FragColour;

void main() {
	FragColour = vec4(u_BaseColour, 1.0);
}