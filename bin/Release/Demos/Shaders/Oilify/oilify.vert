//~ #version 120
//~ #define NUM_INTENSITIES 32
varying vec2 TexCoord;
//~ varying vec4 hist[32];
void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	TexCoord = gl_MultiTexCoord0.xy;
	//~ int i;
	//~ for (i = 0; i < 32; i++)
	//~ {
		//~ hist[i] = vec4(0.0);
	//~ }
}