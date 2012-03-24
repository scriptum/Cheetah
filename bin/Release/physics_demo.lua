require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Test', 800, 600, 32, 'v')
lQuery.addhook(C.clear)

local cp = require 'lib.chipmunk'
space = cp.SpaceNew()
space:setGravity(cp.v(0, 100))

lQuery.addhook(function()
		space:step(1/C.FPS)
end)

ground = cp.SegmentShapeNew(space.staticBody, cp.v(0, 500), cp.v(800, 600), 0)
ground:setFriction(1)
space:addShape(ground)
ground = cp.SegmentShapeNew(space.staticBody, cp.v(800, 0), cp.v(800, 600), 0)
ground:setFriction(1)
space:addShape(ground)
ground = cp.SegmentShapeNew(space.staticBody, cp.v(0, 0), cp.v(0, 600), 0)
ground:setFriction(1)
space:addShape(ground)
ground = cp.SegmentShapeNew(space.staticBody, cp.v(0, 0), cp.v(800, 0), 0)
ground:setFriction(1)
space:addShape(ground)

moment = cp.MomentForCircle(1, 0, 64, cp.vzero)
ballBody = space:addBody(cp.BodyNew(1, moment))
ballBody:setPos(cp.v(64, 64))
ballShape = space:addShape(cp.CircleShapeNew(ballBody, 64, cp.vzero))
ballShape:setFriction(0.7)

ball = E:new(screen):draw(function(s)
	C.pop()
	C.push()
	C.translateObject(ballBody.p.x-64, ballBody.p.y-64, ballBody.a*180/math.pi, 128, 128, 64, 64)
end):image('data/ball.png')

C.mainLoop()