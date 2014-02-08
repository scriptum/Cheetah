#version 120

uniform sampler2D texture;
uniform vec2 size;
varying vec2 TexCoord;
float BSpline( float x )
{
	float f = abs(x);

	if(f <= 1.0 )
		return (( 2.0 / 3.0 ) + ( 0.5 ) * ( f * f * f ) - (f * f));
	else
	{
		f = 2.0 - f;
		return (1.0 / 6.0 * f * f * f);
	}
}
const vec4 offset = vec4(-1.0, 1.0, 1.0 ,-1.0);

vec4 filter(sampler2D tex, vec2 texcoord)
{
	float fx = fract(texcoord.x);
	float fy = fract(texcoord.y);
	texcoord.x -= fx;
	texcoord.y -= fy;

	vec4 xcubic = vec4(BSpline(- 1 - fx), BSpline(-fx), BSpline(1 - fx), BSpline(2 - fx));
	vec4 ycubic = vec4(BSpline(- 1 - fy), BSpline(-fy), BSpline(1 - fy), BSpline(2 - fy));

	vec4 c = vec4(texcoord.x - 0.5, texcoord.x + 1.5, texcoord.y - 0.5, texcoord.y + 1.5);
	vec4 s = vec4(xcubic.x + xcubic.y, xcubic.z + xcubic.w, ycubic.x + ycubic.y, ycubic.z + ycubic.w);
	vec4 offset = c + vec4(xcubic.y, xcubic.w, ycubic.y, ycubic.w) / s;

	vec4 sample0 = texture2D(tex, vec2(offset.x, offset.z) / size);
	vec4 sample1 = texture2D(tex, vec2(offset.y, offset.z) / size);
	vec4 sample2 = texture2D(tex, vec2(offset.x, offset.w) / size);
	vec4 sample3 = texture2D(tex, vec2(offset.y, offset.w) / size);

	float sx = s.x / (s.x + s.y);
	float sy = s.z / (s.z + s.w);

	return mix(mix(sample3, sample2, sx), mix(sample1, sample0, sx), sy);
}

void main() {
   gl_FragColor = filter(texture, TexCoord * size);
}
