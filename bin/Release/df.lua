require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Test', 1920, 1080, 32, 'v')

C.printFPS = true

local df = C.newShader([[#version 120
uniform sampler2D texture;
uniform float sharpness;
uniform float gamma;

void main() {
	vec4 color = texture2D(texture, gl_TexCoord[0].xy);
	gl_FragColor = vec4(color.rgb, smoothstep(gamma - sharpness, gamma + sharpness, color.a));
}
]])
local img = C.newImage('df.png')
--~ E:new(screen):image('data/brick.png')
E:new(screen):draw(function(s)
	df:bind()
	df:set('texture', 0)
	local sharp = img.w/s.w/3
	if sharp > 0.5 then sharp = 0.5 end
	df:set('sharpness', sharp)
	df:set('gamma', 0.3)
	df:set('pixel', 1/s.w)
	img:drawt(s.x, s.y, s.w, s.h, 0, s.ox, s.oy)
	df:unbind()
	img:drawxy(0, 0, img.w, img.h)
end)
:wheel(function(s, x, y, b)
	if b == 'u' then
		s:set({tw = s.tw*1.2, th = s.th*1.2})
	else
		s:set({tw = s.tw/1.2, th = s.th/1.2})
	end
	s:stop():animate({ox = s.tw/2, oy = s.th/2, w = s.tw, h = s.th}, {easing = "linear"})
end)
:set({tw = img.w, th = img.h})
:origin(img.w/2, img.h/2)
:draggable()
:size(img.w, img.h)
:animate({x = 100,y=100})
--~ :animate({w=1024, h=380},25)

C.mainLoop()