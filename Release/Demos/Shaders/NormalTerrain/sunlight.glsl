#version 120
uniform sampler2D normal_tex;
uniform vec3 lightColor;
uniform vec2 screen;
uniform vec2 yavpitch;
void main() {
	vec4 normal_c = texture2D(normal_tex, gl_FragCoord.xy/screen);
	vec3 normal = normal_c.rgb * 2.0 - 1.0;  
	float diffuse = dot(normal, normalize(vec3(yavpitch, 1.0)));
	gl_FragColor = vec4(diffuse*lightColor, 1.0);
}