#version 400

#define PI 3.14159265359

in vec3 f_Position;
in vec2 f_UV;
in vec3 f_Normal;

uniform vec3 u_Albedo;
uniform float u_Metallic;
uniform float u_Roughness;

uniform vec3 u_LightColour;
uniform vec3 u_LightPosition;
uniform vec3 u_CameraPosition;

uniform float u_AmbientStrength;

out vec4 FragColour;

// Normal distribution function:
// Approximates the microfacets' divergence from the halfway vector using the Trowbridge-Reitz GGX distribution.
float DistributionGGX(vec3 N, vec3 H, float a) {
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float nom    = a2;
    float denom  = (NdotH2 * (a2 - 1.0) + 1.0);
    denom        = PI * denom * denom;
	
    return nom / denom;
}

// Geometry functions:
// Describe the self-shadowing properties of the microfacets using Smith's Schlick-GGX function.
float GeometrySchlickGGX(float NdotV, float roughness) {
    float nom   = NdotV;
    float denom = NdotV * (1.0 - roughness) + roughness;
	
    return nom / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = GeometrySchlickGGX(NdotV, roughness);
    float ggx2 = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

// Fresnel equation:
// Describes the ratio of surface reflection at various surface angles using the Fresnel-Schlick approximation.
// The reflectance increases when viewed at a very sheer angle.
vec3 FresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{		
    vec3 N = f_Normal;
    vec3 V = normalize(u_CameraPosition - f_Position);

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, u_Albedo, u_Metallic);
	           
    // reflectance equation
    vec3 Lo = vec3(0.0);

    vec3 L = normalize(u_LightPosition - f_Position);
    vec3 H = normalize(V + L);
    float distance    = length(u_LightPosition - f_Position);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance     = u_LightColour * attenuation;        
    
    // cook-torrance brdf
    float NDF = DistributionGGX(N, H, u_Roughness);        
    float G   = GeometrySmith(N, V, L, u_Roughness);      
    vec3 F    = FresnelSchlick(max(dot(H, V), 0.0), F0);       
    
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - u_Metallic;	  
    
    vec3 numerator    = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
    vec3 specular     = numerator / max(denominator, 0.001);  
        
    // add to outgoing radiance Lo
    float NdotL = max(dot(N, L), 0.0);
    Lo += (kD * u_Albedo / PI + specular) * radiance * NdotL; 
  
    vec3 ambient = u_AmbientStrength * u_Albedo;
    vec3 colour = ambient + Lo;
	
    colour = colour / (colour + vec3(1.0));
    colour = pow(colour, vec3(1.0/2.2));  
   
    FragColour = vec4(colour, 1.0);
}