require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Test', 1024, 768, 32, '')

local generators = {'dummy', 'noise', 'light', 'lightexp', 'circle'}
local sizes = {16, 64, 256}

C.clearColor(0.1,0.1,0.1,1)
--~ 
--~ 
local offy  = 30
local offx  = 0
for _, s in ipairs(sizes) do
	offx = 0
	for _, v in ipairs(generators) do
		print(s, v)
		if offx  + s > 1024 then
			offx = 0
			offy = offy + s + 5
		end
		E:new(screen)
		:image(C.generate(s,s,v))
		:move(offx, offy)
		offx = offx + s
		if offx + s > 1024 then
			offx = 0
			offy = offy + s + 5
		end
		print(s, v..'_alpha')
		E:new(screen)
		:image(C.generate(s,s,v..'_alpha'))
		:move(offx, offy)
		offx = offx + s
	end
	offy = offy + s + 5
end


E:new(screen):fps()
local ffi = require 'ffi'
local millis = C.getTicks()
local ptr = ffi.new('ImageData')
C.generateImageData(ptr, 5000, 5000, 'circle')
print(C.getTicks() - millis)

C.mainLoop()