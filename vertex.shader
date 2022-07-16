#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec2 aTexCoords;

out vec3 Position;

out vec3 FragPos;
out vec3 Normals;
out vec2 TexCoord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform vec3 lightPos;

void main()
{
    gl_Position = Projection * View * Model * vec4(aPos, 1.0);
	FragPos = vec3(Model * vec4(aPos, 1.0f));
	Normals = mat3(transpose(inverse(Model))) * aNormals;
	TexCoord = aTexCoords;
}