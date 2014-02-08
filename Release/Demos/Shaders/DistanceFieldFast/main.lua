require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Fast Distance Field')
C.newFont('dejavuDF.fnt', true)
local f = C.fonts["DejaVu Sans"][21]
--fast but less accurate shader
local dfshader = C.newShader('df.glsl')
local str = C.getFile('text.txt')
local fontScaleStep = 1.1
E:new(screen):image('paper.jpg') --background
E:new(screen)
:set({
	fontScale = 0.5, --50% font scale (21/2=10pt)
	sharpness = 5 --sharpness of DF antialiasing
}):size(C.getWindowWidth(),C.getWindowHeight())
:draw(function(s)
	dfshader:bind()
	dfshader:set('sharpness', s.sharpness * s.fontScale)
	f:setScale(s.fontScale)
	f:print(str, 0, 0, s.w, C.alignJustify)
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