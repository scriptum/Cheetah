uniform sampler2D tex;
uniform float angle;
varying vec2 TexCoord;
void main() {
	vec4 color = texture2D(tex, TexCoord);
	vec3 normal = 1.0 - color.rgb * 2.0;
	normal *= mat3(
	cos(angle), -sin(angle), 0.0,
	sin(angle),  cos(angle), 0.0,
	0.0,         0.0,        1.0);
	gl_FragColor = vec4(0.5 - normal * 0.5, color.a);
}