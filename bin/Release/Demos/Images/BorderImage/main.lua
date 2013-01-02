require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Bordered Image')
local drawCenter = true
Entity:new(screen):borderImage('button.png', 6, 6, 6, 6):size(150,32):move(400 - 75,300 - 16)
:animate({x = 16, y = 16}, 1):delay(0.5)
:animate({w = 800-32, h = 600-32}, 4):delay(0.5)
:animate({w = 32, h = 32, x = 400-16, y = 300-16}, 5)
E:new(screen):text('Border image example. Press T to toggle frame')
:keypressed(function(s, key)
	if key == 't' then
		drawCenter = not drawCenter
		C.imageDrawBorderCenter(drawCenter)
	end
end)
C.mainLoop()