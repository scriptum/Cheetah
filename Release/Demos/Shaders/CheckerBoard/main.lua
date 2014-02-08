require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Checkers Board')
local dummy = C.generate('dummy', 1, 1)
C.printFPS = true
local shader = C.newShader('checkboard.glsl')
shader:bind()
E:new(screen):draw(function(s)
	local w, h = C.getWindowWidth(), C.getWindowHeight()
	shader:set('scale', w, h)
	shader:set('size', 16, 16)
	shader:set('offset', math.floor(time), 0)
	shader:set('color1', 0, 1, 0)
	shader:set('color2', 1, 0, 0)
	dummy:draw(0, 0, w, h)
end)

--do not forget about main loop!
C.mainLoop()