require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
--enable windows resizing and disable autoscale to avoid font corruption
C.init('Test', '1024x700 resizable')
C.printFPS = true
C.autoScale(false)
--load font
local f = C.newFont('MonotypeCorsiva.fnt', true)
f.distanceField = true
-- local f = C.newFont('LiberationSerif.fnt', true)
-- local f = C.newFont('DejaVuSans.fnt', true)

--load DF shader
local dfshader = C.newShader('distance_field.glsl')
--load text as string
local str = C.getFile('bigtext.txt')
-------------------------------SCROLL TEXT ENTITY-------------------------------
local textarea = E:new(screen)
:set({
	fontScale = 0.5,  --50% font scale (21/2=10pt)
	gamma     = 0.48,  --gamma for Distance Field
	sharpness = 0.18, --sharpness of Distance Field anti-aliasing
	_offset   = 0,    --others are internal
	_velocity = 0,
	_lasttime = time
})
:draw(function(s)
	if time - s._lasttime > 0.01 then --100 fps
		if s.dragging then
			s._offset = s.dragY - lQuery.mY
			s._lastmY1 = s._lastmY2
			s._lastmY2 = lQuery.mY
		end
		s._offset = s._offset + s._velocity
		s._velocity = s._velocity * 0.97
		s._lasttime = time
	end
	-- dfshader:bind()
	-- dfshader:set('gamma', s.gamma)
	-- dfshader:set('sharpness', s.sharpness / s.fontScale)
	-- dfshader:set('size', f.image.w, f.image.h)
	-- C.enableAlphaTest()
	f:setScale(s.fontScale)
	f:print(str, s.x, s.y - s._offset, s.w, C.alignCenter)
	-- dfshader:unbind()
end):size(C.getWindowWidth(),C.getWindowHeight())
:mousepressed(function(s, x, y, b)
	s.dragY = lQuery.mY + s._offset
	s.dragging = true
	s._velocity = 0
	s._lastmY1, s._lastmY2 = nil, nil
end)
:mousereleased(function(s, x, y, b)
	s.dragging = false
	if s._lastmY1 and s._lastmY2 then
		s._velocity = s._lastmY1 - s._lastmY2
	end
end)
:mousewheel(function(s, x, y, b)
	if b == "u" then
		if s._velocity > 0 then s._velocity = 0 end
		s._velocity = s._velocity - 10
	elseif b == "d" then
		if s._velocity < 0 then s._velocity = 0 end
		s._velocity = s._velocity + 10
	end
end)
-----------------------------END SCROLL TEXT ENTITY-----------------------------
--when window resizes, resize texarea too
lQuery.onresize(function()
	textarea:size(C.getWindowWidth(),C.getWindowHeight())
end)

local fontScaleStep = 1.1
--some key bindings
screen:keypress(function(s, key)
	if key == "a" then
		textarea:stop():animate({fontScale = textarea.fontScale * fontScaleStep})
	elseif key =="s" then
		textarea:stop():animate({fontScale = textarea.fontScale / fontScaleStep})
	end
end)

C.mainLoop()
