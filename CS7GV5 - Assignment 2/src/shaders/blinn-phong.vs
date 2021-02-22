#version 400

in vec3 v_Position;
in vec2 v_UV;
in vec3 v_Normal;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

out vec3 f_Position;
out vec2 f_UV;
out vec3 f_Normal;

void main() {
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(v_Position, 1.0);

	f_Position = (u_ModelMatrix * vec4(v_Position, 1.0f)).xyz;
	f_UV = v_UV;
	f_Normal = normalize(v_Normal);
}