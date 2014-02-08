require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Road of ribbon raytrace example', '800x600 resizable')
C.printFPS = true

local shader = C.newShader("shader.glsl")
shader:bind()
E:new(screen):draw(function()
	shader:set('time', time)
	shader:set('resolution', 800,600)
	C.rectanglexy(0,0,800,600)
end)
C.mainLoop()