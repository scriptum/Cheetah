#version 120
uniform sampler2D tex;
uniform vec2 texel;
uniform float amplitude;
uniform float phase;
uniform float period;
varying vec2 TexCoord;
void main() {
	vec2 tc = TexCoord;
	vec2 v = tc - 0.5;
	float l = length(v / texel);
	float theta = sin(l / period) * amplitude;
	float s = sin(theta);
	float c = cos(theta);
	v = vec2(dot(v, vec2(c, -s)), dot(v, vec2(s, c)));
	//~ tc = tc + sin(l / len + phase) * normalize(v) * texel * amplitude;
	tc = v + 0.5;
	vec4 color = texture2D(tex, tc);
	gl_FragColor = color;
}