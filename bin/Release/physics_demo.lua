require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Test', 800, 600, 32, 'v')
lQuery.addhook(C.clear)

local cp = require 'lib.chipmunk'
--set space into cp.space with gravity 300, 3 steps per frame
cp.defaultScape(300, 3)

cp.space:setIterations(5)
cp.space:setSleepTimeThreshold(1)
cp.space:useSpatialHash(5, 10000)
C.printFPS = true
cp.stepsPerFrame(1)
--borders around the screen
--~ cp.addFrame(0,0,800,600, 0.5, 1)
--~ cp.addBorder(0,500,800,600, 0.5, 1)


E:new(screen):rectangle()
:move(0,600):size(800,400):offset(400,200)
:physBox(math.huge)

E:new(screen):rectangle()
:move(0,-400):size(800,400):offset(400,200)
:physBox(math.huge)

E:new(screen):rectangle()
:move(-400,-400):size(400,1400):offset(200,700)
:physBox(math.huge)

E:new(screen):rectangle()
:move(800,-400):size(400,1400):offset(200,700)
:physBox(math.huge)

cp.mouseForce(500000)

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
for i=1,40 do
	for j=1,40 do
		E:new(screen)
		:draw(getSleep)
		:radius(2)
		:image('data/ball_small.png')
		:size(4,4)
		:move(1+j*4,1+i*4)
		:offset(2,2)
		:physCircle(1, 1)
		:physDraggable()
	end
end

for i=1,2 do
	E:new(screen)
	:draw(getSleep)
	:radius(63)
	:image('data/ball.png')
	:move(300,i*64)
	:offset(64,64)
	:physCircle(10)
	:physDraggable()
end

for i=1,9 do
	E:new(screen)
	:draw(getSleep)
	:image('data/crate.png')
	:move(500,i*64)
	:offset(32,32)
	:size(62,62)
	:physBox(10,0.9,0)
	:physDraggable()
	:size(64,64)
end
--~ 
C.mainLoop()