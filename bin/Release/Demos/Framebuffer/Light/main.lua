require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
local scr_w, scr_h = 512, 512
C.init('Test', scr_w, scr_h, 32, 'v')
--landscape texture
local landImg = C.newImage('landscape.jpg')
--lighting texture
local lightImg = C.newImage('light.png')
--draw landscape first
E:new(screen):image(landImg)
--create new framebuffer - light layer
local lightFbo = C.newFramebuffer(scr_w, scr_h, '')
--new entity for lighting
E:new(screen)
:draw(function()
	--draw to framebuffer now
	lightFbo:bind()
	--fill with black
	C.clear()
	--add one by one few light sources
	C.blendMode(C.blendAdditive)
	lightImg:drawxy((math.sin(time)+1)*(512-128)*0.5, (math.sin(time*1.2)+1)*(512-128)*0.5, 128, 128)
	lightImg:drawxy((math.sin(time*1.5+10)+1)*(512-128)*0.5, (math.sin(time*1.2-5)+1)*(512-128)*0.5, 128, 128)
	lightImg:drawxy((math.sin(-time*0.9+6)+1)*(512-128)*0.5, (math.sin(time*0.7+9)+1)*(512-128)*0.5, 128, 128)
	lightImg:drawxy((math.sin(-time*0.5+2)+1)*(512-256)*0.5, (math.sin(time*0.2-1)+1)*(512-256)*0.5, 256, 256)
	--disable drawing to framebuffer
	lightFbo:unbind()
	--new blend mode - detail -- is perfect for light effects
	C.blendMode(C.blendDetail)
	--draw framebuffer
	lightFbo:drawxy(0, 0, scr_w, scr_h)
	--return blending mode to default
	C.blendMode(0)
end)
--slow down lights
C.gameSpeed(0.3)
--do not forget about main loop!
C.mainLoop()