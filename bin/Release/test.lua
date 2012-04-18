require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Cheetah engine', 800, 600, 32, 's')
require 'data.tahoma'
local ffi = require 'ffi'
local MAX = 120701
--~ C.enableStencilTest()
local arr = ffi.new('Point[?]', MAX)
local x, y
for i=0, MAX-1 do 
x, y = math.random(0,800), math.random(0,600)
arr[i].x = x 
arr[i].y = y
end
local vbo = C.newVboPoints(arr, MAX)

E:new(screen):draw(function()
	C.rectangle(true)
end)
:color(0,0,0,255)
:size(800,600)
:animate({g = 100}, {loop = true, speed = 3})
:animate({g = 0}, {loop = true, speed = 3})


local image = C.newImage('test.png')
E:new(screen):draw(function()
	--~ C.push()
	--~ C.move(400,0)
	--~ C.scale(400,600)
	--~ C.drawToStencil()
	--~ C.rectangle(true)
	--~ C.pop()
	
	C.move(math.sin(time),0)
	--~ C.rotate(time)
	--~ C.move(-400,-300)
	--~ C.drawUsingStencil()
	C.vboDrawSprites(vbo, image, 10)
end)

local smallFont = Fonts["Tahoma"][8]
E:new(screen):draw(function()
	smallFont:print("fps: " .. math.floor(C.FPS) .. ", mem: " .. gcinfo(), 100, 0)
end):move(0,0)

C.mainLoop()
