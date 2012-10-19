require 'lib.cheetah'
--~ require 'lib.mixer'
require 'lib.lquery.init'
local C = cheetah
C.init('Test', 1024, 1024, 32, '')
C.printFPS = true

local img1 = C.generate('noise', 8, 8, 'n')
local img2 = C.generate('noise', 8, 8, 'n')

e = E:new(screen):draw(function(s)
	for i = 0, 1024/8, 2 do
		for j = 0, 1024/8 do
			img1:draw(i*8, j*8, 8, 8)
		end
	end
	for i = 1, 1024/8, 2 do
		for j = 0, 1024/8 do
			img2:draw(i*8, j*8, 8, 8)
		end
	end
end)


C.mainLoop()

