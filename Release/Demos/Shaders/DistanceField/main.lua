require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Test')

C.printFPS = true

--simple distance field shader
local df = C.newShader([[#version 120
uniform sampler2D texture;
uniform float sharpness;
uniform float gamma;
varying vec2 TexCoord;
void main() {
	vec4 color = texture2D(texture, TexCoord);
	gl_FragColor = vec4(color.rgb, smoothstep(gamma - sharpness, gamma + sharpness, color.a));
}
]])

local img = C.newImage('df.png')

E:new(screen):draw(function(s)
	df:bind()
	df:set('texture', 0)
	local sharp = img.w/s.w/3
	if sharp > 0.5 then sharp = 0.5 end
	df:set('sharpness', sharp)
	df:set('gamma', 0.3)
	df:set('pixel', 1/s.w)
	img:draw(s.x, s.y, s.w, s.h, 0, s.ox, s.oy)
	df:unbind()
	img:draw(0, 0, img.w, img.h)
end)
:wheel(function(s, x, y, b)
	if b == 'u' then
		s:set({tw = s.tw*1.2, th = s.th*1.2})
	else
		s:set({tw = s.tw/1.2, th = s.th/1.2})
	end
end)
:set({tw = img.w, th = img.h})
:origin(img.w/2, img.h/2)
:draggable()
:size(img.w, img.h)
:animate({x = 100,y=100})
--~ :animate({w=1024, h=380},25)

C.mainLoop()