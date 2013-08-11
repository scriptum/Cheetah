require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Ajustable shader', '800x600 resizable')
C.printFPS = true

local shader = C.newShader("volcanic.glsl")
local dummy = C.generate('noise', 256, 256)
local fbo = C.newFramebuffer(800,600)
local s = 0
E:new(screen):draw(function()
	local delta = math.abs(C.getFps() - 60) / 1000
	if C.getFps() < 60 then s = s + delta
	else s = s - delta end
	if s < 0 then s = 0 end
	local rx, ry = 800 - s*800, 600 - s*600
	if rx < 32 then rx = 32 end
	if ry < 32 then ry = 32 end
	shader:bind()
	shader:set('iGlobalTime', time/5)
	shader:set('iResolution', rx, ry)
	shader:set('iChannel0', 0)

	fbo:bind()
	dummy:drawq(0, 0, rx, ry)
	fbo:unbind()
	shader:unbind()
	fbo:drawq(0, 0, 800, 600, 0, 0, rx, ry)
	--print(C.FPS, rx)
end)
C.mainLoop()
