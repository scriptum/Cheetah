require 'lib.cheetah'
--~ require 'lib.mixer'
require 'lib.lquery.init'
local C = cheetah
C.init('Test', 512, 512, 32, '')
lQuery.addhook(C.clear)
cheetah.printFPS = true
E:new(screen):image('line.png'):size(40, 300):move(68,100):rotate(1):animate({x=110}, 6)

C.mainLoop()