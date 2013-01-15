#version 120
uniform sampler2D texture;
uniform float sharpness;
uniform float gamma;
uniform float pixel;
varying vec2 TexCoord;

void main() {
	vec4 color = texture2D(texture, gl_TexCoord[0].xy);
	float alpha_left  = texture2D(texture, TexCoord - vec2(pixel * 0.5, 0.)).a;
	float alpha_right = texture2D(texture, TexCoord + vec2(pixel * 0.5, 0.)).a;
	gl_FragColor = vec4(vec3(
	smoothstep(gamma + sharpness, gamma - sharpness, alpha_left),
	smoothstep(gamma + sharpness, gamma - sharpness, color.a),
	smoothstep(gamma + sharpness, gamma - sharpness, alpha_right)
	), 1.);
}
