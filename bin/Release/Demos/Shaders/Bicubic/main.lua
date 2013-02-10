require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
local scr_w, scr_h = 800, 600
C.init('Bicubic Filter', scr_w..'x'..scr_h)
C.printFPS = true
local img1 = C.newImage('1.png')
local img2 = C.newImage('2.png')
local shader = C.newShader('bicubic.glsl')
local bic = false
local image = true
local bg = false
local zoom = 1
local ent = E:new(screen):set({scale = 1.0})
:draw(function(s)
	if bic then
		shader:bind()
		shader:set('size', s.img.w, s.img.h)
	end
	local w, h = s.img.w * s.scale, s.img.h * s.scale
	s.img:draw(scr_w * 0.5 , scr_h * 0.5, w, h, time * 0.01, w * 0.5, h * 0.5)
	if bic then shader:unbind() end
	C.fonts.default:setScale(2)
	if bg then C.color(0,0,0,255)
	else C.color(255,255,255,255) end
	C.fonts.default:print("Toggle bicubic filter: f\nToggle zoom: z\nToggle image: t\nToggle background: g")
	if bg then C.color(255,255,255,255) end
end)
ent.img = img1

screen:keypress(function(s, key)
	if key == "f" then
		bic = not bic
	elseif key == "z" then
		if zoom == 1 then ent:stop():animate({scale = 5})
		elseif zoom == 2 then ent:stop():animate({scale = 0.35})
		else ent:stop():animate({scale = 1}) end
		zoom = (zoom + 1) % 3
	elseif key == "t" then
		if image then
			ent.img = img2
		else
			ent.img = img1
		end
		image = not image
	elseif key == "g" then
		if bg then
			C.clearColor(0,0,0,0)
		else
			C.clearColor(1,2,3,0)
		end
		bg = not bg
	end
end)

C.mainLoop() --do not forget about main loop!
