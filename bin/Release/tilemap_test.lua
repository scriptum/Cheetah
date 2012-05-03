require 'lib.cheetah'
--~ require 'lib.mixer'
require 'lib.lquery.init'
local C = cheetah
C.init('Test', 512, 512, 32, '')
C.printFPS = true
t = C.newTilemap("data/tilemap_test.map")

e = E:new(screen):draw(function(s)
	t:draw(math.floor(s.x), math.floor(s.y), s.angle, 1, s.ox, s.oy)
end)
:rotate(0)
:sizeoffset(6*32, 4*32)
:size(192, 128)
:animate({x = 200, y = 200}, 1)
:animate({angle = 180*4}, 2)
:draggable()

C.mainLoop()

