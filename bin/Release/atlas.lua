require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Test', 512, 512, 32, 'v')

local data = C.resLoader('data/atlas')
local tiger = C.resLoader('data/1111')

E:new(screen):draw(function()
	data.images.anima.player.death[0][math.floor(time * 20 % 56)]:draw(0, 256, 256, 256)
	tiger.tiger:draw(0,0,nil,nil,time/10)
end)

C.mainLoop()