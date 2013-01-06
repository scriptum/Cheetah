#version 120
uniform sampler2D texture;
uniform float sharpness;
uniform float gamma;
varying vec2 TexCoord;
void main() {
	vec4 color = texture2D(texture, TexCoord);
	gl_FragColor = vec4(color.rgb, smoothstep(gamma - sharpness, gamma + sharpness, color.a));
}