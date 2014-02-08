require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Bordered Image')
local drawCenter = true
local img = C.newBorderImage('button.png', 6, 6, 6, 6)
Entity:new(screen):draw(function(s)
	img:draw(s.x, s.y, s.w, s.h, time/5, s.w/2, s.h/2)
end)
:size(img:getWidth(), img:getHeight())
:move(400, 300)
:animate({w = 200, h = 46}, 5):delay(3)
:animate({w = 46, h = 200}, 5)
C.mainLoop()