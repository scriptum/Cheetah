require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Test', 512, 512, 32, '')
E:new(screen):image('data/brick.png'):color(255,255,255,255)
local lightFBO = C.newFramebuffer(128,128,'')
E:new(screen):draw(function()
	lightFBO:bind()
end)
E:new(screen):image('data/light.png'):draggable():set({blendMode = C.blendAdditive}):size(50,50)
E:new(screen):image('data/light.png'):draggable():set({blendMode = C.blendAdditive}):size(80,80)
E:new(screen):draw(function()
	lightFBO:unbind()
	lightFBO:draw()
end):size(512,512)
C.mainLoop()