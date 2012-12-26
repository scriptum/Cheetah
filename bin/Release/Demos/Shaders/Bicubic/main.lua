require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
local scr_w, scr_h = 800, 600
C.init('RotoZoom', scr_w, scr_h, 32, '')
C.printFPS = true
local img = C.newImage('1.png')
local shader = C.newShader('bicubic.glsl')
local bic = false
local zoom = 1
local ent = E:new(screen):set({scale = 1.0})
:draw(function(s)
	if bic then shader:set('size', img.w, img.h) end
	local w, h = img.w * s.scale, img.h * s.scale
	img:draw(scr_w * 0.5 , scr_h * 0.5, w, h, time * 0.01, w * 0.5, h * 0.5)
	C.fonts.default:scale(2)
	C.fonts.default:print("Toggle bicubic filter: f\nToggle zoom: z")
end)

screen:keypress(function(s, key)
	if key == "f" then 
		if bic then
			shader:unbind()
		else
			shader:bind()
		end
		bic = not bic
	elseif key == "z" then
		if zoom == 1 then ent:stop():animate({scale = 5})
		elseif zoom == 2 then ent:stop():animate({scale = 0.25})
		else ent:stop():animate({scale = 1}) end
		zoom = (zoom + 1) % 3
	end
end)

C.mainLoop() --do not forget about main loop!