require 'lib.cheetah'
--~ require 'lib.mixer'
require 'lib.lquery.init'
local C = cheetah
C.init('Test', 512, 512, 32, '')

t = C.newTilemap("data/tilemap_test.map")

e = E:new(screen):draw(function(s)
	t:draw(s.x, s.y, s.angle, 1)
end)
:animate({x = 600}, 50)

C.mainLoop()

