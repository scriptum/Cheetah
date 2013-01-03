#version 120
uniform sampler2D tex;
uniform vec2 texel;
uniform float amplitude;
uniform float phase;
uniform float len;
varying vec2 TexCoord;
void main() {
	vec2 tc = TexCoord;
	vec2 v = tc - 0.5;
	tc = tc + sin(length(v / texel) / len + phase) * normalize(v) * texel * amplitude;
	vec4 color = texture2D(tex, tc);
	gl_FragColor = color;
}