require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('FXAA - Anti Aliasing')
--fast but less accurate shader
local shader = C.newShader('fxaa.glsl')
shader:bind()

local texOffset = 1.0
shader:set('size', 1/800 * texOffset, 1/600 * texOffset)
E:new(screen):image('1.png') --background
--~ E:new(screen):draw(function()
	--~ C.color(255,0,0,255)
	--~ texOffset = (time + 25) / 50
	--~ C.fonts.default:scale(3)
	--~ shader:unbind()
	--~ C.fonts.default:print(''..texOffset)
	--~ C.color(255,255,255,255)
	--~ shader:bind()
	--~ shader:set('size', 1/800 * texOffset, 1/600 * texOffset)
--~ end)
C.mainLoop()