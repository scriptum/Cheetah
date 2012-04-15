require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Test', 800, 600, 32, '')

local img = C.generate(32,32,'light_alpha')

--~ C.blendMode(1)

for i=1,4000 do
	E:new(screen)
	:image(img)
	:move(400,300)
	:animate({x=math.random(0,800-img.w),y=math.random(0,600-img.h)})
	:mouseover(function(s)
		s:animate({x=math.random(0,800-img.w),y=math.random(0,600-img.h)})
	end)
end

E:new(screen):fps()

C.mainLoop()