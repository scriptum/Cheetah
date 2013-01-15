require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Median filter' ,'800x600 resizable')
local img = C.newImage('Cheetah.jpg')
C.printFPS = true
local shader = C.newShader('median3x3.glsl')
shader:bind()
E:new(screen):draw(function()
	shader:set('T', 0)
	shader:set('texel', 1/800, 1/600)
	img:draw(0, 0, 800, 600)
end)

C.mainLoop()