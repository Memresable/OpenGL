#version 330 core
out vec4 FragColor;

in vec3 Normals;
in vec3 FragPos;
in vec2 TexCoord;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;

struct Material
{
	sampler2D diffuse;
	sampler2D specular;

	float shininess;
};

uniform Material material;

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform DirLight dirLight;

struct PointLight
{
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[4];

struct SpotLight
{
	vec3 position;
	vec3 direction;

	float cutoff;
	float outercutoff;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform SpotLight spotLight;

vec3 CalcuDirLight(DirLight light, vec3 normal, vec3 viewDir);

vec3 CalcuPointLight(PointLight light, vec3 normal, vec3 FragPos, vec3 viewDir);

vec3 CalcuSpotLight(SpotLight light, vec3 norm, vec3 FragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normals);
    vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = vec3(0.0f);

	result += CalcuDirLight(dirLight, norm, viewDir);

	result += CalcuPointLight(pointLights[0], norm, FragPos, viewDir);

	result += CalcuSpotLight(spotLight, norm, FragPos, viewDir);

	FragColor = vec4(result, 1.0f);
}

vec3 CalcuSpotLight(SpotLight light, vec3 norm, vec3 FragPos, vec3 viewDir)
{
	// Ambient
	vec3 ambient = texture(material.diffuse, TexCoord).rgb * light.ambient;
	
	// Light from Cube
	vec3 lightDir = normalize(lightPos - FragPos);

	float theta = dot(lightDir, normalize(-light.direction));

	// Direct Light
	float diffAngle = max(dot(norm, lightDir), 0.0);
	vec3 diff = light.diffuse * (diffAngle * texture(material.diffuse, TexCoord).rgb);

	// Specular
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = (texture(material.specular, TexCoord).rgb * spec) * light.specular;

    float epsilon = (light.cutoff - light.outercutoff);
    float intensity = clamp((theta - light.outercutoff) / epsilon, 0.0, 1.0);
    diff  *= intensity;
    specular *= intensity;
    
    // attenuation
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient  *= attenuation;
    diff     *= attenuation;
    specular *= attenuation;

	return (ambient + diff + specular);
}

vec3 CalcuPointLight(PointLight light, vec3 normal, vec3 FragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - FragPos);

	// Diffuse Lighting
	float diff = max(dot(normal, lightDir), 0.0);

	// Specular Lighting
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	// Attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	// Combining The Results
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb;
	vec3 specular = light.specular * spec * texture(material.specular, TexCoord).rgb;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

vec3 CalcuDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);

	// Diffuse Lighting
	float diff = max(dot(normal, lightDir), 0.0);

	// Specular Lighting
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	// Combine The Results
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb;
	vec3 specular = light.specular * spec * texture(material.specular, TexCoord).rgb;

	return (ambient + diffuse + specular);
}