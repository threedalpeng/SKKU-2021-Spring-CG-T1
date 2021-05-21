#ifdef GL_ES
	#ifndef GL_FRAGMENT_PRECISION_HIGH	// highp may not be defined
		#define highp mediump
	#endif
	precision highp float; // default precision needs to be defined
#endif

// input from vertex shader
in vec4 epos;
in vec3 norm;
in vec2 tc;

// the only output variable
out vec4 fragColor;

uniform mat4	view_matrix;
uniform float	shininess;
uniform vec4	light_position, Ia, Id, Is;	// light
uniform vec4	Ka, Kd, Ks;					// material properties
uniform bool	b_shaded;

uniform vec4	color;
uniform bool	b_colored;

// texture sampler
uniform sampler2D TEX0;		// = diffuseTexture
uniform sampler2D TEX1;		// for alpha
uniform bool	b_texture;
uniform bool 	b_alpha_tex;


// for shadow
uniform bool		b_shadow;
uniform sampler2D 	shadowMap;
uniform vec3 		lightPos;
uniform vec3 		viewPos;


vec4 phong( vec3 l, vec3 n, vec3 h, vec4 Kd )
{
	vec4 Ira = Ka*Ia;									// ambient reflection
	vec4 Ird = max(Kd*dot(l,n)*Id,0.0);					// diffuse reflection
	vec4 Irs = max(Ks*pow(dot(h,n),shininess)*Is,0.0);	// specular reflection
	return Ira + Ird + Irs;
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
	float bias = 0.00001;
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    
    if(projCoords.z > 1.0)  shadow = 0.0;

    return shadow;
}

void main()
{
	vec4 iKd = texture( TEX0, tc );	// Kd from image
	vec4 iA = texture(TEX1, tc);
	if (b_shaded && !b_colored && b_texture) {
		// light position in the eye space
		vec4 lpos = view_matrix*light_position;

		vec3 n = normalize(norm);	// norm interpolated via rasterizer should be normalized again here
		vec3 p = epos.xyz;			// 3D position of this fragment
		vec3 l = normalize(lpos.xyz-(lpos.a==0.0?vec3(0):p));	// lpos.a==0 means directional light
		vec3 v = normalize(-p);		// eye-epos = vec3(0)-epos
		vec3 h = normalize(l+v);	// the halfway vector

		fragColor = phong( l, n, h, iKd );
	}

	else if(!b_shaded && b_colored && !b_texture) {
		fragColor = color;
	}
	else if(!b_shaded && b_colored && b_texture)
	{
		fragColor = iKd * color;
	}
	else if(b_shaded && b_colored)
	{
		// light position in the eye space
		vec4 lpos = view_matrix*light_position;

		vec3 n = normalize(norm);	// norm interpolated via rasterizer should be normalized again here
		vec3 p = epos.xyz;			// 3D position of this fragment
		vec3 l = normalize(lpos.xyz-(lpos.a==0.0?vec3(0):p));	// lpos.a==0 means directional light
		vec3 v = normalize(-p);		// eye-epos = vec3(0)-epos
		vec3 h = normalize(l+v);	// the halfway vector

		if (b_texture) {
			fragColor = phong( l, n, h, iKd ) * color;
		}
		else {
			fragColor = phong( l, n, h, color );
		}
	}
	// else if(b_texture){	// !!! bug !!!!
	else {
		fragColor = iKd;
	}

	if (b_alpha_tex) {
		fragColor.a = iA.r;
	}
}
