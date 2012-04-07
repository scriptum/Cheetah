require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Test', 800, 600, 32, '')
lQuery.addhook(C.clear)
C.printFPS = true

local shader = cheetah.newShader(

[[#version 120

uniform vec2 position;

void main(void)
{
  gl_Position = vec4(position.x, 0.2, 0.0, 1.0);
}]],
[[#version 120
void main(void)
{
    gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}]])
--~ local img = C.newImage('data/light.png')

--~ C.smooth(false)
--~ for i=1,50 do
	--~ E:new(screen)
	--~ :image(img)
	--~ :move(i%800, math.floor(i/800))
--~ end
--~ C.blendMode(1)
E:new(screen):draw(function()
	for i=0,1874 do
		--~ C.reset()
		--~ C.move(i%800, math.floor(i/800))
		--~ img:draw()
		img:drawt((16*i)%800, math.floor(i/800*16)*16,16,16,time*math.sin(i),math.cos(time),math.sin(time))
	end
end)
:hide()

E:new(screen):draw(function()
	--~ C.move(1,1)
	C.rotate(time)
	C.rectanglexy(100,100,150,100, true)
end)
:translate()
--~ :rotate(2)
C.mainLoop()