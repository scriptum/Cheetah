#version 120
uniform sampler2D texture;
uniform float sharpness;
varying vec2 TexCoord;
void main() {
	vec4 color = texture2D(texture, TexCoord);
	gl_FragColor = vec4(color.rgb, (color.a - 0.5) * sharpness + 0.5);
}