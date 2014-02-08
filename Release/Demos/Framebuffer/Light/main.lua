require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah --assign cheetah to C (short alias)
local scr_w, scr_h = 1024, 768 --set window width and height
C.init('Lights', scr_w..'x'..scr_h..' vsync') --init screen
C.printFPS = true --print fps
local landImg = C.newImage('landscape.jpg') --background texture
local lightImg = C.newImage('light.png') --lighting texture
local lightDiv = 8 -- light layer will be 8 times smaller than screen buffer (for best speed)
--create new framebuffer - light layer
local lightFbo = C.newFramebuffer(scr_w/lightDiv, scr_h/lightDiv, '')
--generate different light sources
local lights = {}
local lightsCount = 15
for i = 1, lightsCount do
	lights[i] = {math.random(0,1000),math.random(0,1000),math.random(0.6,2),math.random(0.6,2),math.random(64/lightDiv,512/lightDiv)}
end

--new entity just for rendering
E:new(screen)
:draw(function()
	--draw background
	landImg:drawq(0,0,scr_w,scr_h,0,0,scr_w,scr_h)
	--draw to framebuffer now
	lightFbo:bind()
	--fill with black
	C.clear()
	--add one by one few light sources
	C.blendMode(C.blendAdditive)
	--scale light to gray (gray = 100% lightness in detail blending mode)
	C.color(65,60,55,255)
	for i = 1, lightsCount do
		lightImg:draw((math.sin(time*lights[i][3]+lights[i][1])+1)*(scr_w/lightDiv-lights[i][5])*0.5, 
		(math.sin(time*lights[i][4]+lights[i][2])+1)*(scr_h/lightDiv-lights[i][5])*0.5, lights[i][5], lights[i][5])
	end
	--restore color
	C.color(255,255,255,255)
	--disable drawing to framebuffer
	lightFbo:unbind()
	--new blend mode - detail -- is perfect for light effects
	C.blendMode(C.blendDetail)
	--draw framebuffer
	lightFbo:draw(0, 0, scr_w, scr_h)
	--return blending mode to default
	C.blendMode(0)
	--show framebuffer
	lightFbo:draw(0, 0)
end)
--slow down lights
C.gameSpeed(0.3)
--do not forget about main loop!
C.mainLoop()