require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Animation queue')
local img = C.generate('light', 32, 32)
C.blendMode(C.blendAdditive)
--run two x animaions in queue and y in parallel
Entity:new(screen)
:image(img):move(30,30)
:animate({x = 500}, {speed = 2, queue = "x"})
:animate({x = 300}, {speed = 1, queue = "x"})
:animate({y = 300}, {speed = 3, queue = "y"})

--run all animations in queue
Entity:new(screen)
:image(img):move(30,30)
:animate({x = 500}, 2)
:animate({x = 300}, 1)
:animate({y = 300}, 3)
C.mainLoop()