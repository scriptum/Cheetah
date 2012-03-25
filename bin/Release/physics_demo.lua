require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Test', 800, 600, 32, 'v')
lQuery.addhook(C.clear)

local cp = require 'lib.chipmunk'
--set space into cp.space with gravity 300
cp.defaultScape(300)

--borders around the screen
cp.addFrame(0,0,800,600, 0.8)
cp.addBorder(0,500,800,600, 0.8)
local img = C.newImage('data/ball.png')
for i=1,10 do
	E:new(screen)
	:image(img)
	:move(i*64,i*64)
	:offset(64,64)
	:physCircle(10, 0.5, 63)
	:physDraggable()
end
C.mainLoop()