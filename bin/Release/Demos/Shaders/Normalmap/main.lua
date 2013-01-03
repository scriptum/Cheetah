require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
--set window width and height
local scr_w, scr_h = 1024, 1024
C.init('Lights', scr_w..'x'..scr_h)
--print fps
C.printFPS = true
--landscape texture
local landImg = C.newImage('tex.jpg')
--lighting texture
local lightImg = C.newImage('tex_n.jpg')

local multi = C.newMultitexture(landImg, lightImg)


local shader = C.newShader([[#version 120
uniform sampler2D tex;
uniform sampler2D normal_tex;  
uniform vec3 light_pos;
varying vec2 TexCoord;
void main() {
	// Extract the normal from the normal map  
	vec3 normal = texture2D(normal_tex, TexCoord).rgb * 2.0 - 1.0;  

	// Calculate the lighting diffuse value  
	float diffuse = dot(normal, normalize(vec3(light_pos.xy - vec2(gl_FragCoord.x, ]]..scr_h..[[-gl_FragCoord.y), 100.0)));  
	float spec = 0.;//specular * pow(diffuse, shininess);
	vec4 texc = texture2D(tex, TexCoord);

	// Set the output color of our current pixel  
	gl_FragColor = vec4(diffuse*texc.rgb, texc.a);
}
]])

E:new(screen):draw(function(s)
	shader:bind()
	shader:set('tex', 0)
	shader:set('normal_tex', 1)
	shader:set('light_pos', lQuery.mX, lQuery.mY, 250)
	multi:draw(0,0,scr_w, scr_h)
end)

--do not forget about main loop!
C.mainLoop()