#version 120
uniform sampler2D normal_tex;
uniform sampler2D light;  
uniform vec3 lightColor;
uniform vec2 screen;
uniform float height; 
//uniform float shininess;
//uniform float specular;
varying vec2 TexCoord;
void main() {
	vec3 texc = texture2D(light, TexCoord).rgb; 
	// Extract the normal from the normal map
	vec4 normal_c = texture2D(normal_tex, gl_FragCoord.xy/screen);
	vec3 normal = normal_c.rgb * 2.0 - 1.0;  
	float diffuse = dot(normal, normalize(vec3(0.5 - TexCoord, height)));

	// Set the output color of our current pixel
	gl_FragColor = vec4(diffuse*texc*lightColor, 1.0);
}