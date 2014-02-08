#version 120
#define NUM_INTENSITIES 256
uniform sampler2D tex;
uniform vec2 texel;
uniform int radius;
uniform float exponent;
uniform float intensities;
varying vec2 TexCoord;
void main() {
	ivec4 hist[NUM_INTENSITIES];
	ivec4 hist_max = ivec4(1);
	int i, j, intensity;
	float radius_sq = radius * radius;
	vec4 temp_pixel;
	for (i = 0; i < intensities; i++)
	{
		hist[i] = ivec4(0);
	}
	
	for (i = -radius; i <= radius; i++)
	{
		for (j = -radius; j <= radius; j++)
			{
				if (i*i + j*j <= radius_sq)
					{
						temp_pixel = texture2D(tex, TexCoord + vec2(i, j) * texel);
						hist[int(temp_pixel.x * (intensities - 1))].x+=1;
						hist[int(temp_pixel.y * (intensities - 1))].y+=1;
						hist[int(temp_pixel.z * (intensities - 1))].z+=1;
						hist[int(temp_pixel.w * (intensities - 1))].w+=1;
					}
			}
	}

	for (i = 0; i < intensities; i++) {
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
  for (i = 0; i < intensities; i++)
	{
		ratio = vec4(hist[i]) / vec4(hist_max);
		weight = pow(ratio, vec4(exponent));
		sum += weight * vec4(i);
		div += weight;
	}
  gl_FragColor = sum / div / (intensities - 1);
}