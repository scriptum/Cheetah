require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
--set window width and height
local scr_w, scr_h = 800, 600
C.init('Lights', scr_w, scr_h, 32, '')
--print fps
C.printFPS = true
--landscape texture
local landImg = C.newImage('landscape.jpg')
--lighting texture
local lightImg = C.newImage('light.png')
--create new framebuffer - light layer
local lightFbo = C.newFramebuffer(scr_w, scr_h, '')
--generate different light sources
local lights = {}
local lightsCount = 3
for i = 1, lightsCount do
	lights[i] = {math.random(0,1000),math.random(0,1000),math.random(0.6,2),math.random(0.6,2),math.random(64,512)}
end

--new entity just for rendering
E:new(screen)
:draw(function()
	--draw background
	landImg:drawqxy(0,0,scr_w,scr_h,0,0,scr_w,scr_h)
	--draw to framebuffer now
	lightFbo:bind()
	--fill with black
	C.clear()
	--add one by one few light sources
	C.blendMode(C.blendAdditive)
	--scale light to gray (gray = 100% lightness in detail blending mode)
	C.color(128,128,128,255)
	for i = 1, lightsCount do
		lightImg:drawxy((math.sin(time*lights[i][3]+lights[i][1])+1)*(scr_w-lights[i][5])*0.5, (math.sin(time*lights[i][4]+lights[i][2])+1)*(scr_h-lights[i][5])*0.5, lights[i][5], lights[i][5])
	end
	--restore color
	C.color(255,255,255,255)
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