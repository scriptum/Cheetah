require 'lib.cheetah'
--~ require 'lib.mixer'
require 'lib.lquery.init'
local C = cheetah
C.init('Test', 512, 512, 32, '')
C.printFPS = true
t = C.newTilemap("data/tilemap_test.map")

e = E:new(screen):draw(function(s)
	t:draw(math.floor(s.x), math.floor(s.y), s.angle, 1)
end)
:rotate(0)
:animate({x = 200, y = 200, angle = 180}, 1)
:size(192, 128)
:draggable()

C.mainLoop()

