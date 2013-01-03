require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Bordered Image')
local borderOnly = false
Entity:new(screen):borderImage('button.png', 6, 6, 6, 6):size(150,32):move(400 - 75,300 - 16)
:animate({x = 16, y = 16}, 1):delay(0.5)
:animate({w = 800-32, h = 600-32}, 4):delay(0.5)
:animate({w = 64, h = 64, x = 400, y = 300, ox = 32, oy = 32}, 5):delay(0.5)
:animate({angle = math.pi * 6}, 10):delay(0.5)
:animate({w = 800-32, h = 600-32, ox = 0, oy = 0, x = 16, y = 16})
:keypressed(function(self, key)
	if key == 't' then
		borderOnly = not borderOnly
		self._image:drawBorderOnly(borderOnly)
	end
end)
E:new(screen):text('Border image example. Press T to toggle frame')

C.mainLoop()