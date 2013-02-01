require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
local scr_w, scr_h = 640, 400
C.init('Generators', scr_w..'x'..scr_h)

local generators = {'dummy', 'noise', 'light', 'lightexp', 'circle'}
local sizes = {16, 64, 128}

E:new(screen):draw(function()
local a = (math.cos(time) + 1)*0.1	
C.clearColor(a,a,a,1)
end)
local offy  = 0
local offx  = 0
for _, s in ipairs(sizes) do
	offx = 0
	for _, v in ipairs(generators) do
		if offx  + s > scr_w then
			offx = 0
			offy = offy + s + 5
		end
		E:new(screen)
		:image(C.generate(v, s, s))
		:move(offx, offy)
		offx = offx + s
		if offx + s > scr_w then
			offx = 0
			offy = offy + s + 5
		end
		E:new(screen)
		:image(C.generate(v, s, s, 'a'))
		:move(offx, offy)
		offx = offx + s
	end
	offy = offy + s + 5
end

C.mainLoop()