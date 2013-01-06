require 'lib.cheetah'
--~ require 'lib.mixer'
require 'lib.lquery.init'
local C = cheetah

local scr = 512
C.init('Test', scr..'x'..scr)
C.printFPS = true

local size = 2

print("Drawing ".. (scr*scr/size/size) .. " quads, fixed fillrate")

local img1 = C.generate('noise', size, size)
local img2 = C.generate('noise', size, size)

e = E:new(screen):draw(function(s)
	for i = 0, scr/size, 2 do
		for j = 0, scr/size do
			img1:draw(i*size + time/10, j*size, size, size)
		end
	end
	for i = 1, scr/size, 2 do
		for j = 0, scr/size do
			img2:draw(i*size + time/10, j*size, size, size)
		end
	end
end)

C.mainLoop()

