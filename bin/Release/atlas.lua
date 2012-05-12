require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Test', 512, 512, 32, 'v')

local data = C.resLoader('data/cheetah')
local tiger = C.resLoader('data/1111')

E:new(screen):draw(function()
	data[math.floor(time * 24 % 32)]:draw(0, 256)
	tiger.tiger:draw(0,0,nil,nil,time/10)
end)

C.mainLoop()