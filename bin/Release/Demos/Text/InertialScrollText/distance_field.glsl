#version 120
uniform sampler2D texture;
uniform float sharpness;
uniform float gamma;

void main() {
	vec4 color = texture2D(texture, gl_TexCoord[0].xy);
	gl_FragColor = vec4(color.rgb, smoothstep(gamma - sharpness, gamma + sharpness, color.a));
}