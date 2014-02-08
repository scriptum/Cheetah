#version 120
#define NUM_INTENSITIES 256
uniform sampler2D tex;
uniform vec2 texel;
uniform int radius;
uniform float exponent;
varying vec2 TexCoord;
void main() {
	int i, j;
	vec4 temp_pixel;
	float radius_sq = radius * radius;
	float max_i = 0., intensity;
	vec4 best_color;
	for (i = -radius; i <= radius; i++)
	{
		for (j = -radius; j <= radius; j++)
			{
				if (i*i + j*j <= radius_sq)
					{
						temp_pixel = texture2D(tex, TexCoord + vec2(i, j) * texel);
						intensity = (0.299 * temp_pixel.r
												+0.587 * temp_pixel.g
												+0.114 * temp_pixel.b);
						if(intensity > max_i)
						{
							max_i = intensity;
							best_color = temp_pixel;
						}
					}
			}
	}
  gl_FragColor = best_color;
}