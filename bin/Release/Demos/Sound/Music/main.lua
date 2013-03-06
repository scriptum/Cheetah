require 'lib.cheetah'
require 'lib.mixer'
require 'lib.lquery.init'
local C = cheetah
C.init('Star dance', '800x600 vsync resizable')

local image = C.newImage('Stars.png')

--multi-layer starfield
Entity:new(screen):draw(function()
	C.blendMode(C.blendAdditive)
	local s = 2.3 + math.sin(time/16)*0.2
	image:draw(500, 350, 800*s, 600*s, time/15, 400*s, 300*s)
	s = 2.5 + math.sin(time/16)*0.5
	image:draw(350, 300, 800*s, 600*s, time/19 + 5, 400*s, 300*s)
	s = 2.5 + math.sin(time/16)*0.3
	image:draw(450, 200, 800*s, 600*s, time/25 + 1, 400*s, 300*s)
end)

--loops=-1 - infinite
C.playMusic('Stars.ogg', -1)

C.mainLoop()
