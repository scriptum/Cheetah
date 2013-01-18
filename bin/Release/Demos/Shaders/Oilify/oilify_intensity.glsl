#version 120
#define NUM_INTENSITIES 16
uniform sampler2D tex;
uniform vec2 texel;
uniform int radius;
uniform float exponent;
uniform float intensities;
varying vec2 TexCoord;
void main() {
	//~ vec4 cumulative_rgb[NUM_INTENSITIES];
	float hist_inten[NUM_INTENSITIES], inten_max;
	int i, j, intensity;
	vec4 temp_pixel;
	float radius_sq = radius * radius;
	for (i = 0; i < intensities; i++)
	{
		hist_inten[i] = 0.001;
		//~ cumulative_rgb[i] = vec4(0.0);
	}
	for (i = -radius; i <= radius; i++)
	{
		for (j = -radius; j <= radius; j++)
			{
				if (i*i + j*j <= radius_sq)
					{
						temp_pixel = texture2D(tex, TexCoord + vec2(i, j) * texel);
						intensity = int((0.299 * temp_pixel.x
												+0.587 * temp_pixel.y
												+0.114 * temp_pixel.z) * (intensities - 1));
						hist_inten[intensity] += 1;
						//~ cumulative_rgb[intensity] += temp_pixel;
					}
			}
	}
	inten_max = 0.;

  /* calculated maximums */
	for (i = 0; i < intensities; i++) {
		if(hist_inten[i] > inten_max)
			inten_max = hist_inten[i];
	}
	float div = 0.0;
	float ratio, weight, mult_inten;

	float color = (0.0);
  for (i = 0; i < intensities; i++)
	{
		ratio = hist_inten[i] / inten_max;
		weight = pow(ratio, exponent);
		mult_inten = weight / hist_inten[i];
		
		div += weight;
		//~ color += mult_inten * cumulative_rgb[i];
		color += (mult_inten * (intensities - i));
	}
  gl_FragColor = temp_pixel * color/div;
}