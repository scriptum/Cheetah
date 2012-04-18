require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Test', 1024, 768, 32, 'r')

lQuery.addhook(C.clear)
local str

str = C.getFile('data/japan.txt')

C.newFont('data/japan.fnt')
local f = C.fonts["Liberation Sans"][10]
local f1 = C.fonts.default
f1:scale(2)
--~ f:interval(0.85)
C.autoScale(false)
E:new(screen):draw(function(s)
	f:print(str,0,15, C.getWindowWidth())
	f1:scale(2)
	f1:print('FPS: '..C.FPS..'\n123123\n12312313123\n')
	--~ f:scale(1)
end)

C.mainLoop()