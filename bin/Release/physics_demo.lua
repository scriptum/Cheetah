require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Test', 800, 600, 32, '')
lQuery.addhook(C.clear)

local cp = require 'lib.chipmunk'
--set space into cp.space with gravity 300
cp.defaultScape(3)
--~ cp.space:setIterations(10)
cp.space:setSleepTimeThreshold(1)
--~ print(cp.space:getIterations())
C.printFPS = true
--borders around the screen
cp.addFrame(0,0,800,600, 0.5, 1)
cp.addBorder(0,500,800,600, 0.5, 1)
local img = C.newImage('data/ball.png')
C.setBlendMode(0)
local function getSleep(s)
	if (s.body.node_private.idleTime > cp.space.sleepTimeThreshold) then
		C.Color(128,128,128,255)
	else
		C.Color(255,255,255,255)
	end
end
E:new(screen):draw(function()C.rectangle(true)end):color('PeachPuff'):move(400,0):size(400,600)
for i=1,1000 do
	E:new(screen)
	:draw(getSleep)
	:image('data/ball_small.png')
	:move(150+i/2,100+i/3)
	:offset(5,5)
	:physCircle(1, 5)
	:physDraggable()
	:size(10,10)
end
C.mainLoop()