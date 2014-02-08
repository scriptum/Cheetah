require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
local sin = math.sin
local scr_w, scr_h = 1024, 768
C.init('Lights', scr_w..'x'..scr_h)
C.printFPS = true
local landImg = C.newImage('tex1.jpg')
local normalImg = C.newImage('tex1_n.jpg')
local mouseLight = 150
local lightImg = C.generate('light', 64, 64)
local lightFbo = C.newFramebuffer(scr_w, scr_h)
local normFbo = C.newFramebuffer(scr_w, scr_h)
local playerImg = C.newImage('player.png')
local playerNorm = C.newImage('player_n.png')
--generate different light sources
local lights = {}
local lightsCount = 10
for i = 1, lightsCount do
	lights[i] = {math.random(0,1000),math.random(0,1000),math.random(0.06,0.02),math.random(0.06,0.02),math.random(200,300)}
end
--generate objects
local players = {}
local playersCount = 50
for i = 1, playersCount do
	players[i] = {math.random(0,scr_w),math.random(0,scr_h),math.random(0,6)}
end
--normalmapping shader in separate file
local shader = C.newShader('lightshader.glsl')
--special shader to rotate normals
local rotnorm = C.newShader('rotatenormal.glsl')
 --generate multitexture for normal mapping shader
local multi = C.newMultitexture(normFbo.image, lightImg)
--new entity just for rendering
E:new(screen)
:draw(function()
	landImg:drawq(0,0,scr_w,scr_h,0,0,scr_w,scr_h) --draw background
	for i = 1, playersCount do
		playerImg:draw(players[i][1], players[i][2], playerImg.w, playerImg.h, sin(time/10 + players[i][3]) * 12, playerImg.w/2, playerImg.h/2)
	end
	--normalmap to framebuffer
	normFbo:bind()
	normalImg:drawq(0,0,scr_w,scr_h,0,0,scr_w,scr_h)
	rotnorm:bind() --rotate normals - this is nessesary if object is rotated
	for i = 1, playersCount do
		local angle = sin(time/10 + players[i][3]) * 12
		rotnorm:set('angle', angle)
		playerNorm:draw(players[i][1], players[i][2], playerNorm.w, playerNorm.h, angle, playerImg.w/2, playerImg.h/2)
	end
	rotnorm:unbind()
	normFbo:unbind()
	--draw to lighting layer now
	lightFbo:bind()
	C.clearColor(0.05,0.05,0.05,1)
	C.clear() --fill with gray to emulate ambient
	C.clearColor(0,0,0,1)
	--add one by one few light sources
	C.blendMode(C.blendAdditive)
	shader:bind() --use shader for light binding
	shader:set('normal_tex', 0) --first texture is normalmap
	shader:set('light', 1) --second - light
	shader:set('screen', scr_w, scr_h) --send screen size to shader
	shader:set('height', 0.1) --height of light (z pos)
	shader:set('lightColor', 0.4, 0.3, 0.2) --100% light is 0.5 due to detial blending mode
	for i = 1, lightsCount do
		local l = lights[i]
		multi:draw((sin(time*l[3]+l[1])+1)*(scr_w-l[5])*0.5, (sin(time*l[4]+l[2])+1)*(scr_h-l[5])*0.5, l[5], l[5])
	end
	shader:set('lightColor', 0.4, 0.5, 0.6) --let mouse light be blue
	multi:draw(lQuery.mX-mouseLight/2, lQuery.mY-mouseLight/2, mouseLight, mouseLight)
	shader:unbind() --disable shader
	lightFbo:unbind() --stop rendering to framebuffer
	C.blendMode(C.blendDetail) --detail blenging is perfect for light effects
	lightFbo:draw(0, 0, scr_w, scr_h) --merge light layer (framebuffer)
	C.blendMode(0) --return blending mode to default
end)

screen:wheel(function(s, x,y,b)
	if b == "u" then
		mouseLight = mouseLight * 1.1
	elseif b == "d" then
		mouseLight = mouseLight * 0.9
	end
end)

C.gameSpeed(1) --slow down lights

C.mainLoop() --do not forget about main loop!