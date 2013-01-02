require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Oilify shader')
local img = C.newImage('Cheetah.jpg')
C.printFPS = true
local shader = C.newShader('oilify_intensity.glsl')
shader:bind()
local radius = 4
E:new(screen):draw(function(s)
	local w, h = C.getWindowWidth(), C.getWindowHeight()
	shader:set('texel', 1/w, 1/h)
	shader:set('radius', radius)
	shader:set('radius_sq', radius * radius)
	shader:set('exponent', 8)
	img:draw(0, 0, 800, 600)
end)

--do not forget about main loop!
C.mainLoop()