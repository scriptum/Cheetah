#version 120
uniform sampler2D texture;
uniform float sharpness;
uniform float gamma;
uniform float pixel;
varying vec2 TexCoord;

const vec3 colorScale = vec3(0.0, 0.0, 0.0);
const float pixelShift = 0.15;
void main() {
	vec4 color = texture2D(texture, TexCoord);
	float alpha_left  = texture2D(texture, TexCoord - vec2(pixel * pixelShift, 0.)).a;
	float alpha_right = texture2D(texture, TexCoord + vec2(pixel * pixelShift, 0.)).a;
	gl_FragColor = vec4(vec3(
	smoothstep(gamma + sharpness + colorScale.r, gamma - sharpness + colorScale.r, alpha_left),
	smoothstep(gamma + sharpness + colorScale.g, gamma - sharpness + colorScale.g, color.a),
	smoothstep(gamma + sharpness + colorScale.b, gamma - sharpness + colorScale.b, alpha_right)
	), 1.);
}
