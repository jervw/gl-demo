#version 330 core

struct Material  
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform Material material;

void main()
{    
    FragColor = vec4(material.diffuse, 1.0);
}
