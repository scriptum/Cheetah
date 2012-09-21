require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Gems game', 800, 600, 32, 'v')
local img = C.generate('light', 32, 32)
C.blendMode(C.blendAdditive)
Entity:new(screen)
:image(img):move(30,30)
:animate({x = 150}, {speed = 1, queue = "x"})
:animate({x = 300}, {speed = 2, queue = "x"})
:animate({y = 300}, {speed = 3, queue = "y"})

Entity:new(screen)
:image(img):move(30,30)
:animate({x = 150}, 1)
:animate({x = 300}, 2)
:animate({y = 300}, 3)
C.mainLoop()