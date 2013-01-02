#version 120
#define NUM_INTENSITIES 16
uniform sampler2D tex;
uniform vec2 texel;
uniform int radius;
uniform int radius_sq;
uniform float exponent;
void main() {
	vec4 hist[NUM_INTENSITIES];
	vec4 hist_max = vec4(1.0);
	int i, j, intensity;
	vec2 tc = gl_TexCoord[0].st;
	vec4 temp_pixel;
	for (i = 0; i < NUM_INTENSITIES; i++)
	{
		hist[i] = vec4(0.0);
	}
	
	for (i = -radius; i <= radius; i++)
	{
		for (j = -radius; j <= radius; j++)
			{
				if (i*i + j*j <= radius_sq)
					{
						temp_pixel = texture2D(tex, tc + vec2(i, j) * texel);
						hist[int(temp_pixel.x * (NUM_INTENSITIES - 1))].x+=1;
						hist[int(temp_pixel.y * (NUM_INTENSITIES - 1))].y+=1;
						hist[int(temp_pixel.z * (NUM_INTENSITIES - 1))].z+=1;
						hist[int(temp_pixel.w * (NUM_INTENSITIES - 1))].w+=1;
					}
			}
	}

	for (i = 0; i < NUM_INTENSITIES; i++) {
		if(hist_max.x < hist[i].x)
			hist_max.x = hist[i].x;
		if(hist_max.y < hist[i].y)
			hist_max.y = hist[i].y;
		if(hist_max.z < hist[i].z)
			hist_max.z = hist[i].z;
		if(hist_max.w < hist[i].w)
			hist_max.w = hist[i].w;
	}
	vec4 div = vec4(0.0);
	vec4 sum = vec4(0.0);
	vec4 ratio, weight;

	vec4 color = vec4(0.0);
  for (i = 0; i < NUM_INTENSITIES; i++)
	{
		ratio = hist[i] / hist_max;
		weight = pow(ratio, vec4(exponent));
		sum += weight * vec4(i);
		div += weight;
	}
  gl_FragColor = sum / div / float(NUM_INTENSITIES - 1);
}