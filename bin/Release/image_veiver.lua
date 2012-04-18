require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Test', 512, 512, 32, 'rv')

--E:new(screen):image('background.jpg'):draggable()

C.mainLoop()