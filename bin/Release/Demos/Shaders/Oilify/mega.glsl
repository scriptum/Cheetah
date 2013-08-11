#version 120
#define NUM_INTENSITIES 10
uniform sampler2D tex;
uniform vec2 texel;
uniform int radius;
uniform float exponent;
uniform float intensities;
varying vec2 TexCoord;
void main() {
	vec4 cumulative_rgb1 = vec4(0.0);
	int hist_inten1 = 1;
	vec4 cumulative_rgb2 = vec4(0.0);
	int hist_inten2 = 1;
	vec4 cumulative_rgb3 = vec4(0.0);
	int hist_inten3 = 1;
	vec4 cumulative_rgb4 = vec4(0.0);
	int hist_inten4 = 1;
	vec4 cumulative_rgb5 = vec4(0.0);
	int hist_inten5 = 1;
	vec4 cumulative_rgb6 = vec4(0.0);
	int hist_inten6 = 1;
	vec4 cumulative_rgb7 = vec4(0.0);
	int hist_inten7 = 1;
	vec4 cumulative_rgb8 = vec4(0.0);
	int hist_inten8 = 1;
	vec4 cumulative_rgb9 = vec4(0.0);
	int hist_inten9 = 1;
	vec4 cumulative_rgb10 = vec4(0.0);
	int hist_inten10 = 1;
	int inten_max;
	int i, j, intensity;
	vec4 temp_pixel;
	float radius_sq = radius * radius;
	for (i = -radius; i <= radius; i++)
	{
		for (j = -radius; j <= radius; j++)
			{
				if (i*i + j*j <= radius_sq)
					{
						temp_pixel = texture2D(tex, TexCoord + vec2(i, j) * texel);
						intensity = int((0.299 * temp_pixel.x
												+0.587 * temp_pixel.y
												+0.114 * temp_pixel.z) * (NUM_INTENSITIES - 1));
						if(intensity == (1 - 1)){hist_inten1+=1;cumulative_rgb1+=temp_pixel;}
						if(intensity == (2 - 1)){hist_inten2+=1;cumulative_rgb2+=temp_pixel;}
						if(intensity == (3 - 1)){hist_inten3+=1;cumulative_rgb3+=temp_pixel;}
						if(intensity == (4 - 1)){hist_inten4+=1;cumulative_rgb4+=temp_pixel;}
						if(intensity == (5 - 1)){hist_inten5+=1;cumulative_rgb5+=temp_pixel;}
						if(intensity == (6 - 1)){hist_inten6+=1;cumulative_rgb6+=temp_pixel;}
						if(intensity == (7 - 1)){hist_inten7+=1;cumulative_rgb7+=temp_pixel;}
						if(intensity == (8 - 1)){hist_inten8+=1;cumulative_rgb8+=temp_pixel;}
						if(intensity == (9 - 1)){hist_inten9+=1;cumulative_rgb9+=temp_pixel;}
						if(intensity == (10 - 1)){hist_inten10+=1;cumulative_rgb10+=temp_pixel;}
					}
			}
	}
	inten_max = 0;
	if(hist_inten1 > inten_max) inten_max = hist_inten1;
	if(hist_inten2 > inten_max) inten_max = hist_inten2;
	if(hist_inten3 > inten_max) inten_max = hist_inten3;
	if(hist_inten4 > inten_max) inten_max = hist_inten4;
	if(hist_inten5 > inten_max) inten_max = hist_inten5;
	if(hist_inten6 > inten_max) inten_max = hist_inten6;
	if(hist_inten7 > inten_max) inten_max = hist_inten7;
	if(hist_inten8 > inten_max) inten_max = hist_inten8;
	if(hist_inten9 > inten_max) inten_max = hist_inten9;
	if(hist_inten10 > inten_max) inten_max = hist_inten10;
	float div = 0.0, ratio, weight, mult_inten;
	vec4 color = vec4(0.0);
	ratio = float(hist_inten1) / float(inten_max);
	weight = pow(ratio, exponent);
	mult_inten = weight / float(hist_inten1);
	div += weight;
	color += mult_inten * cumulative_rgb1;
	ratio = float(hist_inten2) / float(inten_max);
	weight = pow(ratio, exponent);
	mult_inten = weight / float(hist_inten2);
	div += weight;
	color += mult_inten * cumulative_rgb2;
	ratio = float(hist_inten3) / float(inten_max);
	weight = pow(ratio, exponent);
	mult_inten = weight / float(hist_inten3);
	div += weight;
	color += mult_inten * cumulative_rgb3;
	ratio = float(hist_inten4) / float(inten_max);
	weight = pow(ratio, exponent);
	mult_inten = weight / float(hist_inten4);
	div += weight;
	color += mult_inten * cumulative_rgb4;
	ratio = float(hist_inten5) / float(inten_max);
	weight = pow(ratio, exponent);
	mult_inten = weight / float(hist_inten5);
	div += weight;
	color += mult_inten * cumulative_rgb5;
	ratio = float(hist_inten6) / float(inten_max);
	weight = pow(ratio, exponent);
	mult_inten = weight / float(hist_inten6);
	div += weight;
	color += mult_inten * cumulative_rgb6;
	ratio = float(hist_inten7) / float(inten_max);
	weight = pow(ratio, exponent);
	mult_inten = weight / float(hist_inten7);
	div += weight;
	color += mult_inten * cumulative_rgb7;
	ratio = float(hist_inten8) / float(inten_max);
	weight = pow(ratio, exponent);
	mult_inten = weight / float(hist_inten8);
	div += weight;
	color += mult_inten * cumulative_rgb8;
	ratio = float(hist_inten9) / float(inten_max);
	weight = pow(ratio, exponent);
	mult_inten = weight / float(hist_inten9);
	div += weight;
	color += mult_inten * cumulative_rgb9;
	ratio = float(hist_inten10) / float(inten_max);
	weight = pow(ratio, exponent);
	mult_inten = weight / float(hist_inten10);
	div += weight;
	color += mult_inten * cumulative_rgb10;
	gl_FragColor = color/div;
}
