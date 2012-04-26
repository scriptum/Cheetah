require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Test', 800, 600, 32, '')
C.printFPS = true
local img = C.generate(8,8,'light_alpha')

C.blendMode(1)

for i=1,4000 do
	E:new(screen)
	:image(img)
	:move(400,300)
	--~ :move(math.random(0,800-img.w),math.random(0,600-img.h))
	:animate({x=math.random(0,800-img.w),y=math.random(0,600-img.h)})
	:mouseover(function(s)
		s:animate({x=math.random(0,800-img.w),y=math.random(0,600-img.h)})
	end)
end

E:new(screen):fps():delay(function()collectgarbage()end)

C.mainLoop()