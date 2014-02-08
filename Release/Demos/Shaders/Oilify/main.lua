require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Oilify shader')
local img = C.newImage('Cheetah.jpg', 'clamp')
C.printFPS = true
local shader = C.newShader('oilify_intensity.glsl')

local radius, exponent, inten = 4, 8, 16
E:new(screen):draw(function()
	local w, h = C.getWindowWidth(), C.getWindowHeight()
	shader:bind()
	shader:set('texel', 1/w, 1/h)
	shader:set('radius', radius)
	shader:set('exponent', exponent)
	shader:set('intensities', inten)
	img:draw(0, 0, w, h)
end)

C.mainLoop()