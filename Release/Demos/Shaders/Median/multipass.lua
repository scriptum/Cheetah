require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Median filter' ,'800x600 resizable')
local img = C.newImage('Cheetah.jpg', 'clamp')
C.printFPS = true
local shader = C.newShader('median3x3.glsl')
local fbo1 = C.newFramebuffer(800, 600, 'clamp')
local fbo2 = C.newFramebuffer(800, 600, 'clamp')
shader:bind()
E:new(screen):draw(function()
	shader:set('T', 0)
	shader:set('texel', 1/800 * 1.5, 1/600 * 1.5)
	fbo1:bind()
	img:draw(0, 0, 800, 600)
	for i = 1, 1 do
		fbo2:bind()
		fbo1:draw(0, 0, 800, 600)
		fbo2:unbind()
		fbo1:bind()
		fbo2:draw(0, 0, 800, 600)
		fbo1:unbind()
	end
	fbo1:draw(0, 0, 800, 600)
end)

C.mainLoop()