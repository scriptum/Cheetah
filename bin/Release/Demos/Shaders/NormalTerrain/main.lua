require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
local scr_w, scr_h = 1024, 1024
C.init('Sunlight', scr_w, scr_h, 32, '')
C.printFPS = true
local landImg = C.newImage('1.png')
local normalImg = C.newImage('1n.png')
local lightFbo = C.newFramebuffer(scr_w, scr_h) --create new framebuffer - light layer
local normFbo = C.newFramebuffer(scr_w, scr_h) -- normalmap layer
local shader = C.newShader('sunlight.glsl')
--new entity just for rendering
E:new(screen)
:draw(function()
	landImg:drawq(0,0,scr_w*2,scr_h*2,0,0,scr_w,scr_h) --draw background
	--normalmap to framebuffer
	normFbo:bind()
	normalImg:drawq(0,0,scr_w,scr_h,0,0,scr_w,scr_h)
	normFbo:unbind()
	--draw to lighting layer now
	lightFbo:bind()
	C.clear() --fill with black
	C.blendMode(C.blendAdditive)
	shader:bind()
	shader:set('normal_tex', 0)
	shader:set('screen', scr_w, scr_h) --send screen size to shader
	shader:set('yavpitch', (lQuery.mX/scr_w-0.5)*16, (lQuery.mY/scr_h-0.5)*16) --height of light (z pos)
	shader:set('lightColor', 0.6, 0.5, 0.4) --100% light is 0.5 due to detial blending mode
	normFbo:draw(0,0,scr_w,scr_h)
	--shader:set('lightColor', 0.4, 0.5, 0.6) --let mouse light be blue
	--multi:drawxy(lQuery.mX-mouseLight/2, lQuery.mY-mouseLight/2, mouseLight, mouseLight)
	shader:unbind() --disable shader
	lightFbo:unbind() --stop rendering to framebuffer
	C.blendMode(C.blendDetail) --detail blenging is perfect for light effects
	lightFbo:draw(0, 0, scr_w, scr_h) --merge light layer (framebuffer)
	C.blendMode(0) --return blending mode to default
end)

C.mainLoop() --do not forget about main loop!