#version 330 core
// vertex attributes
layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texcoord;

// matrices
uniform mat4 lightSpaceMatrix;
uniform mat4 model_matrix;

void main()
{
	gl_Position = lightSpaceMatrix * model_matrix * vec4(position, 1.0);
}