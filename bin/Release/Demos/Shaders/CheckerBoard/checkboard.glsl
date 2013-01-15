#version 120
uniform vec2 size;
uniform vec2 scale;
uniform vec2 offset;
uniform vec3 color1;
uniform vec3 color2;
varying vec2 TexCoord;
void main() {
	vec2 tc = (TexCoord * scale - offset) / size;
	vec3 c = vec3(mix(color2, color1, (cos(tc.x-tc.y) - cos(tc.x+tc.y)) * 10e30));
	gl_FragColor = vec4(c, 1.0);
}