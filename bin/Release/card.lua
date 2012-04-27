require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Test', 800, 600, 32, '')

local img = C.newImage('data/card_cover.png')

E:new(screen):image(img)
:offset(img.w/2, img.h/2)
:rotate(0)
:move(100,100)
:animate({angle = 7,x = 300, y = 300}, 4)

C.mainLoop()