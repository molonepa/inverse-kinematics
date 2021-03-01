#version 400

in vec3 f_Position;
in vec2 f_UV;
in vec3 f_Normal;

uniform sampler2D u_Albedo;

uniform vec3 u_LightColour;
uniform vec3 u_LightPosition;
uniform vec3 u_CameraPosition;

uniform float u_AmbientStrength;
uniform float u_Shininess;

out vec4 FragColour;

void main() {
	vec3 Result = vec3(0.0);

	vec3 LightDirection = normalize(u_LightPosition - f_Position);
	vec3 ViewDirection = normalize(u_CameraPosition - f_Position);
	vec3 HalfwayDirection = normalize(LightDirection + ViewDirection);

	vec3 AmbientComponent = u_AmbientStrength * u_LightColour;
	vec3 DiffuseComponent = max(dot(f_Normal, LightDirection), 0.0) * u_LightColour;
	vec3 SpecularComponent = pow(max(dot(f_Normal, HalfwayDirection), 0.0), u_Shininess) * u_LightColour;

	//Result = (AmbientComponent + DiffuseComponent + SpecularComponent) * texture(u_Albedo, f_UV).rgb;
	Result = u_AmbientStrength * texture(u_Albedo, f_UV).rgb;
	FragColour = vec4(Result, 1.0);
}
