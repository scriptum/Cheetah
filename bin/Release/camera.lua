require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Test', 800, 600, 32, 'r')

local cam = E:new(screen):camera():move(800/2, 600/2)

screen:wheel(function(s,x,y,b)
	if b == 'u' then
		cam.w = cam.w * 1.1
		cam.h = cam.h * 1.1
	else
		cam.w = cam.w * 0.9
		cam.h = cam.h * 0.9
	end
	cam:stop():animate({sx = cam.w, sy = cam.h, x = cam.x -400 +  x, y = cam.y - 300 + y}, {speed = 0.5, easing = 'outQuad'})
end)

E:new(cam):image('data/brick.png'):move(-256,-256)

E:new(screen):fps()

C.mainLoop()