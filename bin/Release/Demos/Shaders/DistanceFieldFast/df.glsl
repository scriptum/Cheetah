#version 120
uniform sampler2D texture;
uniform float sharpness;

void main() {
	vec4 color = texture2D(texture, gl_TexCoord[0].xy);
	gl_FragColor = vec4(color.rgb, (color.a - 0.5) * sharpness + 0.5);
}