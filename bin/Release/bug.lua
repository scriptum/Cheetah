require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
--~ C.init('Test', 800, 600, 32, '')
local img = C.generate(32, 32, 'light')

--E:new(screen):image('data/ball.png'):size(32,32):draggable()
game = E:new(screen)
bgLayer = E:new(game)
player = E:new(bgLayer): set({x = C.screen.w / 2, y = C.screen.h / 2, w = 32, h = 32}): image(img): draggable()
C.mainLoop()