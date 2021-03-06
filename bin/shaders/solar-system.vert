// vertex attributes
layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texcoord;

// outputs of vertex shader = input to fragment shader
out vec4 epos;	// eye-space position
out vec3 norm;	// per-vertex normal before interpolation
out vec2 tc;	// texture coordinate

// matrices
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} vs_out;

uniform mat4 lightSpaceMatrix;


void main()
{
	vs_out.FragPos = vec3(model_matrix * vec4(position, 1.0));
	vs_out.Normal = transpose(inverse(mat3(model_matrix))) * normal;
	vs_out.TexCoords = texcoord;
	vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);

	vec4 wpos = model_matrix * vec4(position,1);
	epos = view_matrix * wpos;
	gl_Position = projection_matrix * epos;

	// pass tc and eye-space normal to fragment shader
	norm = normalize(mat3(view_matrix*model_matrix)*normal);
	tc = texcoord;
}