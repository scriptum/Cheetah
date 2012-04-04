require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Test', 800, 600, 32, '')
--~ lQuery.addhook(C.clear)
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
local img = C.newImage('data/light.png')

C.smooth(false)
--~ for i=1,2000 do
	--~ E:new(screen)
	--~ :point()
	--~ :move(i,i)
--~ end

E:new(screen):draw(function()
	for i=1,50000 do
		--~ C.reset()
		--~ C.move(i%800, math.floor(i/800))
		img:draw()
		--~ img:drawxy(i%800, math.floor(i/800),1,1)
	end
end)

C.mainLoop()