require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
local w, h = 1024, 768
C.init('Ajustable shader', w..'x'..h..' resizable')
C.printFPS = true

local shader = C.newShader("fire.glsl")
local dummy = C.generate('noise', 256, 256)

local fbo = C.newFramebuffer(w,h)
local s = 0
local targetFPS = 30
E:new(screen):draw(function()
	local delta = math.abs(C.getFps() - targetFPS) / 1000
	if C.getFps() < targetFPS then s = s + delta
	else s = s - delta end
	if s < 0 then s = 0 end
	local rx, ry = w - s*w, h - s*h
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
	fbo:drawq(0, 0, w, h, 0, 0, rx, ry)
	--print(C.FPS, rx)
end)
C.mainLoop()
