require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Clear Type')
local f = C.newFont('dejavuDF.fnt', true)
--load DF shader with ClearType
local dfshader = C.newShader('dfct.glsl')
local str = C.getFile('text.txt')
local fontScaleStep = 1.1
C.clearColor(1,1,1,1)
local textarea = E:new(screen)
:set({
	fontScale = 0.5, --50% font scale (21/2=10pt)
	gamma = 0.5,     --gamma for Distance Field
	sharpness = 0.15 --sharpness of DF antialiasing
}):size(C.getWindowWidth(),C.getWindowHeight())
:draw(function(s)
	dfshader:bind()
	dfshader:set('gamma', s.gamma)
	dfshader:set('sharpness', s.sharpness / s.fontScale)
	dfshader:set('pixel', 1 / 256 / s.fontScale)
	f:setScale(s.fontScale)
	f:print(str, (time), 0, s.w, C.alignJustify)
	dfshader:unbind()
end)
:mousewheel(function(s, x, y, b)
	if b == "u" then
		s.fontScale = s.fontScale * fontScaleStep
	elseif b == "d" then
		s.fontScale = s.fontScale / fontScaleStep
	end
end)

C.mainLoop()
