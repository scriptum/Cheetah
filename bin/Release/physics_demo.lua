require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Test', 800, 600, 32, 'v')
lQuery.addhook(C.clear)

local cp = require 'lib.chipmunk'
--set space into cp.space with gravity 300
cp.defaultScape(300)

print(cp.space:getIterations())

--borders around the screen
cp.addFrame(0,0,800,600, 0.5, 1)
cp.addBorder(0,500,800,600, 0.5, 1)
local img = C.newImage('data/ball.png')
C.setBlendMode(0)
E:new(screen):draw(function()C.rectangle(true)end):color('PeachPuff'):move(400,0):size(400,600)
for i=1,10 do
	E:new(screen)
	:image(img)
	:move(i*64,i*64)
	:offset(64,64)
	:physCircle(10, 63, 0.5, 1)
	:physDraggable()
end
C.mainLoop()