#version 330 core
in vec4 fColor;
in vec2 fUv;
in vec3 fFragPos;
in vec3 fNormal;

struct Material {
    float shininess;
}; 

struct Light {
    vec3 position;  
  
    vec3 color;
    vec3 color_ambient;
	
    float constant;
    float linear;
    float quadratic;
};

uniform sampler2D texture_diffuse0;
uniform sampler2D texture_specular0;
uniform int useVertexColor;
uniform Light light;
uniform Material material;

out vec4 FragColor;

void main()
{
    if (useVertexColor != 0) {
        FragColor = vec4(fColor);
        return;
    }
    vec3 viewPos = vec3(0.0, 0.0, 0.0);
    vec3 normal = normalize(fNormal);
    vec3 mesh_color = texture(texture_diffuse0, fUv).rgb;
    vec3 mesh_spec_color = texture(texture_specular0, fUv).rgb;

    // attenuation
    float d = length(light.position - fFragPos);
    float attenuation = 1.0 / (light.constant + light.linear * d + light.quadratic * (d * d));
    vec3 light_ambient = light.color_ambient;
    vec3 light_diffuse = light.color * attenuation;
    vec3 light_specular = light.color * attenuation;

    // ambient
    vec3 ambient = light_ambient * mesh_color;

    // diffuse 
    vec3 lightDir = normalize(light.position - fFragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light_diffuse * diff * mesh_color;

    // specular
    vec3 viewDir = normalize(viewPos - fFragPos);
    vec3 halfDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfDir), 0.0), material.shininess);
    vec3 specular = light_specular * spec * mesh_spec_color;  

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}