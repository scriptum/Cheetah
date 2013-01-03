require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Waves')
local img = C.newImage('Cheetah.jpg')

local shader = C.newShader('waves.glsl')
shader:bind()
E:new(screen):draw(function(s)
	local w, h = C.getWindowWidth(), C.getWindowHeight()
	shader:set('texel', 1/w, 1/h)
	shader:set('amplitude', 10)
	shader:set('period', 1)
	shader:set('len', 50)
	shader:set('phase', -time*10)
	img:draw(0, 0, w, h)
end)

--do not forget about main loop!
C.mainLoop()