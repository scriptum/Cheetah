require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Masked Image')
local img = C.newImage('checkers.jpg', 'm')

Entity:new(screen):image(img):draggable()
Entity:new(screen):image(img):draggable():move(100,100)
Entity:new(screen):image(img):draggable():move(150,150)

C.mainLoop()