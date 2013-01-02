#version 120
uniform sampler2D tex;
uniform vec2 texel;
void main() {
	vec2 tc = gl_TexCoord[0].st;
	vec4 color = texture2D(tex, tc);
	
	//5x5 kernel, using sampler ability to mixing
	vec2 t = texel * 1.5;
	vec3 c = color.rgb * 8.0
	- 2 * (texture2D(tex, tc + vec2(-t.x, -t.y)).rgb
	+      texture2D(tex, tc + vec2(t.x, -t.y)).rgb
	+      texture2D(tex, tc + vec2(t.x, t.y)).rgb
	+      texture2D(tex, tc + vec2(-t.x, t.y)).rgb);
	gl_FragColor = vec4(c, color.a);
}