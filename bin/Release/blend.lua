require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Test', 1024, 512+64, 32, '')
C.printFPS = true
local img = C.newImage('data/light.png')
local img1 = C.newImage('data/28.png')
lQuery.addhook(C.clear)
E:new(screen):image('data/brick.png')
:size(1024, 1024)
local blend1 = {
	cheetah.GL_ZERO,
	cheetah.GL_ONE,
	cheetah.GL_DST_COLOR,
	cheetah.GL_ONE_MINUS_DST_COLOR,
	cheetah.GL_SRC_ALPHA,
	cheetah.GL_ONE_MINUS_SRC_ALPHA,
	cheetah.GL_DST_ALPHA,
	cheetah.GL_ONE_MINUS_DST_ALPHA,
	cheetah.GL_SRC_ALPHA_SATURATE
}
local blend2 = {
	cheetah.GL_ZERO,
	cheetah.GL_ONE,
	cheetah.GL_SRC_COLOR,
	cheetah.GL_ONE_MINUS_SRC_COLOR,
	cheetah.GL_SRC_ALPHA,
	cheetah.GL_ONE_MINUS_SRC_ALPHA,
	cheetah.GL_DST_ALPHA,
	cheetah.GL_ONE_MINUS_DST_ALPHA
}
E:new(screen):draw(function()
	C.move(0, 0)
	C.color(255,255,255,(math.sin(time)+1)*128)
	--~ C.setBlendEquation(cheetah.GL_MAX)
	
	for i, u in ipairs(blend1) do
		C.push()
		for j, v in ipairs(blend2) do
			C.setBlendFunc(u, v)
			--~ C.setBlendMode(C.blendMask)
			
			img1:draw()
			C.move(1, 0)
		end
		C.pop()
		C.move(0, 1)
	end
	C.move(8, -9)
	for i, u in ipairs(blend1) do
		C.push()
		for j, v in ipairs(blend2) do
			C.setBlendFunc(u, v)
			img:draw()
			C.move(1, 0)
		end
		C.pop()
		C.move(0, 1)
	end
	C.setBlendEquation(cheetah.GL_FUNC_ADD)
	C.setBlendMode(0)
end):size(64, 64)

C.mainLoop()