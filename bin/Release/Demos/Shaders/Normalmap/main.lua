require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
--set window width and height
local scr_w, scr_h = 1024, 1024
C.init('Lights', scr_w, scr_h, 32, '')
--print fps
C.printFPS = true
--landscape texture
local landImg = C.newImage('tex.jpg')
--lighting texture
local lightImg = C.newImage('tex_n.jpg')

local multi = C.newMultitexture(landImg, lightImg)

print(multi.w, multi.h, multi.images[0], multi.images[1], multi.images[2])

local shader = C.newShader([[#version 120
uniform sampler2D tex;
uniform sampler2D normal_tex;  
uniform vec3 light_pos;
//uniform float shininess;
//uniform float specular;
void main() {
	// Extract the normal from the normal map  
	vec3 normal = texture2D(normal_tex, gl_TexCoord[0].st).rgb * 2.0 - 1.0;  

	// Calculate the lighting diffuse value  
	float diffuse = dot(normal, normalize(vec3(light_pos.xy - vec2(gl_FragCoord.x, ]]..scr_h..[[-gl_FragCoord.y), 100.0)));  
	float spec = 0.;//specular * pow(diffuse, shininess);
	vec4 texc = texture2D(tex, gl_TexCoord[0].st);

	// Set the output color of our current pixel  
	gl_FragColor = vec4(diffuse*texc.rgb, texc.a);
}
]])

E:new(screen):draw(function(s)
	shader:bind()
	shader:set('tex', 0)
	shader:set('normal_tex', 1)
	shader:set('light_pos', lQuery.mX, lQuery.mY, 250)
	multi:drawxy(0,0,1024,1024)
end)

--do not forget about main loop!
C.mainLoop()