require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Test', 1024, 768, 32, 'v')

local cp = require 'lib.chipmunk'
--set space into cp.space with gravity 300, 3 steps per frame
cp.defaultScape(300, 2)

cp.space:setIterations(10)
cp.space:setSleepTimeThreshold(1)
cp.space:setCollisionSlop(0.5);
--~ cp.space:useSpatialHash(10, 10000)
C.printFPS = true
cp.stepsPerFrame(1)


E:new(screen):rectangle()
:move(0,600):size(1000,400):offset(400,200)
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

cp.mouseForce(5000000)

local ball = C.newImage('data/ball.png')
local ball_small = C.newImage('data/ball_small.png')
local crate = C.newImage('data/crate.png')
local function getSleep(s)
	if (s.body.node_private.idleTime > cp.space.sleepTimeThreshold) then
		C.color(128,128,128,255)
	else
		C.color(255,255,255,255)
	end
end
E:new(screen):draw(function()C.rectangle()end)
:color('PeachPuff'):move(400,0):size(400,600):translate()

local SMALL = 20

for i=1, 30 do
	for j=10,30 do
		E:new(screen)
		:draw(getSleep)
		:radius(SMALL/2)
		:draw(function()ball_small:draw()end)
		:size(SMALL,SMALL)
		:move((j-1)*SMALL,(i-1)*SMALL)
		:offset(SMALL/2,SMALL/2)
		:physCircle(1)
		:physDraggable()
	end
end
E:new(screen)
		:draw(getSleep)
		:radius(SMALL/2)
		:draw(function()ball_small:draw()end)
		:size(SMALL,SMALL)
		:move(0,0)
		:offset(SMALL/2,SMALL/2)
		:physCircle(1)
		:physDraggable()
--~ for i=1,2 do
	--~ E:new(screen)
	--~ :draw(getSleep)
	--~ :radius(63)
	--~ :draw(function()ball:draw()end)
	--~ :size(128,128)
	--~ :move(300,i*64)
	--~ :offset(64,64)
	--~ :physCircle(10)
	--~ :physDraggable()
--~ end
--~ 
--~ for i=1,15 do
	--~ E:new(screen)
	--~ :draw(getSleep)
	--~ :draw(function()crate:draw()end)
	--~ :move(500,i*32)
	--~ :offset(32,32)
	--~ :size(64,64)
	--~ :physBox(1,0.9,0)
	--~ :physDraggable()
	--~ :size(64,64)
--~ end
--~ 
C.mainLoop()