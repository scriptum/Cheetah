require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
local scr_w, scr_h = 1920, 1080
C.init('Test', scr_w..'x'..scr_h..' resizable')
C.printFPS = true
local data = C.resLoader('data')
local N = 100
local pos = {}
local img = 390
local img_2 = img*0.5
for i = 1, N do
	pos[i] = {math.random(img_2, scr_w-img_2), math.random(img_2, scr_h-img_2), img, img, math.random(0, math.pi*2), img_2, img_2}
end
C.clearScreen(false)
E:new(screen):imageq('bg.dds'):size(scr_w, scr_h)
E:new(screen):draw(function()
	local i = math.floor(time * 30 % 62)
	if i == 1 then
		for k = 1, N do
			pos[k] = {math.random(img_2, scr_w-img_2), math.random(img_2, scr_h-img_2), img, img, math.random(0, math.pi*2), img_2, img_2}
		end
	end
	for k = 1, N do
		data[i]:draw(unpack(pos[k]))
	end
end)

C.mainLoop()