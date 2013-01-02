require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Laplas edge detaction')
local img = C.newImage('Cheetah.jpg')

local shader = C.newShader('laplas.glsl')
shader:bind()
E:new(screen):draw(function(s)
	local w, h = C.getWindowWidth(), C.getWindowHeight()
	shader:set('texel', 1/w, 1/h)
	img:draw(0, 0, w, h)
end)

--do not forget about main loop!
C.mainLoop()