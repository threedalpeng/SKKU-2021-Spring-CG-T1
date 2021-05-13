#ifdef GL_ES
	#ifndef GL_FRAGMENT_PRECISION_HIGH	// highp may not be defined
		#define highp mediump
	#endif
	precision highp float; // default precision needs to be defined
#endif

in	vec2 tc;

out	vec4 fragColor;

uniform sampler2D	TEXT;
uniform vec4		text_color;

void main()
{    
    float alpha = texture( TEXT, tc ).r;
	if (alpha < 0.01){
		fragColor = vec4( 1.0f , 0, 0, alpha );
	}	
	else fragColor = text_color * vec4( 1, 1, 1, alpha );
}  
