#version 120
#define NUM_INTENSITIES 16
uniform sampler2D tex;
uniform vec2 texel;
uniform int radius;
uniform int radius_sq;
uniform float exponent;
void main() {
	vec4 cumulative_rgb[NUM_INTENSITIES];
	float hist_inten[NUM_INTENSITIES], inten_max;
	int i, j, intensity;
	vec2 tc = gl_TexCoord[0].st;
	vec4 temp_pixel;
	for (i = 0; i < NUM_INTENSITIES; i++)
	{
		hist_inten[i] = 1;
		cumulative_rgb[i] = vec4(0.0);
	}
	for (i = -radius; i <= radius; i++)
	{
		for (j = -radius; j <= radius; j++)
			{
				if (i*i + j*j <= radius_sq)
					{
						temp_pixel = texture2D(tex, tc + vec2(i, j) * texel);
						intensity = int((0.299 * temp_pixel.x
												+0.587 * temp_pixel.y
												+0.114 * temp_pixel.z) * (NUM_INTENSITIES - 1));
						hist_inten[intensity] += 1;
						cumulative_rgb[intensity] += temp_pixel;
					}
			}
	}
	inten_max = 1.;

  /* calculated maximums */
	for (i = 0; i < NUM_INTENSITIES; i++) {
		if(hist_inten[i] > inten_max)
			inten_max = hist_inten[i];
	}
	float div = 0.0;
	float ratio, weight, mult_inten;

	vec4 color = vec4(0.0);
  for (i = 0; i < NUM_INTENSITIES; i++)
	{
		ratio = hist_inten[i] / inten_max;
		weight = pow(ratio, exponent);
		mult_inten = weight / hist_inten[i];
		
		div += weight;
		color += mult_inten * cumulative_rgb[i];
	}
  gl_FragColor = color/div;
}