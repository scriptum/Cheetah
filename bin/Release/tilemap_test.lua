require 'lib.cheetah'
--~ require 'lib.mixer'
require 'lib.lquery.init'
local C = cheetah
C.init('Test', 512, 512, 32, '')

t = C.newTilemap("data/tilemap_test.map")

e = E:new(screen):draw(function()
	t:draw(0, 0, 0, 1)
end)

C.mainLoop()

