require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
--enable windows resizing and disable autoscale to avoid font corruption
C.init('Test', '1024x700')
C.printFPS = true
local f = C.newFont('myfont.fnt', true)
local str = "The quick brown fox jumps over the lazy dog.\nСъешь ещё этих мягких французских булок да выпей йаду.\nKerning test: AVA АVА"
E:new(screen):draw(function(s)
	f:enableKerning()
	f:print(str, 10, 10, 1024, C.alignLeft)
	f:disableKerning()
	f:print(str, 10, 10, 1024, C.alignLeft)
end)
C.mainLoop()
