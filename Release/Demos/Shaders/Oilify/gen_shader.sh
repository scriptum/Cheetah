NUM_INTENSITIES=$1
LOOP=`expr $NUM_INTENSITIES - 1`
echo "#version 120"
echo "#define NUM_INTENSITIES $NUM_INTENSITIES"
echo "uniform sampler2D tex;"
echo "uniform vec2 texel;"
echo "uniform int radius;"
echo "uniform float exponent;"
echo "uniform float intensities;"
echo "varying vec2 TexCoord;"
echo "void main() {"
for i in `seq $NUM_INTENSITIES`
do
	echo "	vec4 cumulative_rgb$i = vec4(0.0);"
	echo "	int hist_inten$i = 1;"
done
echo "	int inten_max;"
echo "	int i, j, intensity;"
echo "	vec4 temp_pixel;"
echo "	float radius_sq = radius * radius;"
echo "	for (i = -radius; i <= radius; i++)"
echo "	{"
echo "		for (j = -radius; j <= radius; j++)"
echo "			{"
echo "				if (i*i + j*j <= radius_sq)"
echo "					{"
echo "						temp_pixel = texture2D(tex, TexCoord + vec2(i, j) * texel);"
echo "						intensity = int((0.299 * temp_pixel.x"
echo "												+0.587 * temp_pixel.y"
echo "												+0.114 * temp_pixel.z) * (NUM_INTENSITIES - 1));"
for i in `seq $NUM_INTENSITIES`
do
	echo "						if(intensity == ($i - 1)){hist_inten$i+=1;cumulative_rgb$i+=temp_pixel;}"
done
echo "					}"
echo "			}"
echo "	}"
echo "	inten_max = 0;"
for i in `seq $NUM_INTENSITIES`
do
	echo "	if(hist_inten$i > inten_max) inten_max = hist_inten$i;"
done
echo "	float div = 0.0, ratio, weight, mult_inten;"
echo "	vec4 color = vec4(0.0);"
for i in `seq $NUM_INTENSITIES`
do
	echo "	ratio = float(hist_inten$i) / float(inten_max);"
	echo "	weight = pow(ratio, exponent);"
	echo "	mult_inten = weight / float(hist_inten$i);"
	echo "	div += weight;"
	echo "	color += mult_inten * cumulative_rgb$i;"
done
echo "	gl_FragColor = color/div;"
echo "}"