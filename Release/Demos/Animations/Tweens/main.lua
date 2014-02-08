require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Tweens')
local img = C.newImage('card.png')

--only one time
Entity:new(screen):image(img):move(30,30)
:animate({x = 500}, 2)

--loop animations

--horizontal scroll
Entity:new(screen):image(img):move(30,130)
:animate({x = 500}, {speed = 2, loop = true}):delay({loop = true})
:animate({x = 30}, {speed = 2, loop = true}):delay({loop = true})
--horizontal scroll with rotation and origin in center
Entity:new(screen):image(img):move(30 + img.w/2,230 + img.h/2):origin(img.w/2, img.h/2)
:animate({x = 500 + img.w/2, angle = math.pi}, {speed = 2, loop = true}):delay({loop = true})
:animate({x = 30 + img.w/2, angle = 0}, {speed = 2, loop = true}):delay({loop = true})
--easing function
Entity:new(screen):image(img):move(30,330)
:animate({x = 500}, {speed = 2, loop = true, easing = "outBounce"}):delay({loop = true})
:animate({x = 30}, {speed = 2, loop = true, easing = "outBounce"}):delay({loop = true})
C.mainLoop()