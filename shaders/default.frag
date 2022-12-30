#version 330 core
out vec4 FragColor;

struct Material  
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct DirLight  
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


//in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

//uniform sampler2D texture_diffuse1;
uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLight;

// Directional light calculation
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)  
{
    vec3 lightDir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse * material.diffuse;

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * light.specular * material.specular;

    // ambient shading
    vec3 ambient = light.ambient * material.ambient;

    return (ambient + diffuse + specular);
}

void main()
{    
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // directional light
    vec3 result = CalcDirLight(dirLight, norm, viewDir);

    result += result * material.diffuse;
    FragColor = vec4(result, 1.0);
}
