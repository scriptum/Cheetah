require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Test', '800x600')
C.printFPS = true
local img = C.generate('light', 4, 4)

C.blendMode(C.blendAdditive)
--~ C.gameSpeed(5)
for i=1,10000 do
	E:new(screen)
	:image(img)
	:move(400,300)
	--~ :move(math.random(0,800-img.w),math.random(0,600-img.h))
	:animate({x=math.random(0,800-img.w),y=math.random(0,600-img.h)}, 1)
	:mouseover(function(s)
		s:animate({x=math.random(0,800-img.w),y=math.random(0,600-img.h)})
	end)
end

C.mainLoop()