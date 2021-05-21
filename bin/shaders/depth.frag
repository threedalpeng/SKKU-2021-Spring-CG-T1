#ifdef GL_ES
	#ifndef GL_FRAGMENT_PRECISION_HIGH	// highp may not be defined
		#define highp mediump
	#endif
	precision highp float; // default precision needs to be defined
#endif

// the only output variable
out vec4 fragColor;

void main()
{
    // gl_FragDepth = gl_FragCoord.z;
	fragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
