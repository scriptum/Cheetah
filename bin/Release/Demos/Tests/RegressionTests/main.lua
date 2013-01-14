require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Regression tests', '1x1 noframe')
--test dds images
local ddsimg = C.newImage("01.dds")
--test npot png images
local npotimg = C.newImage("npot.png")
local fbo = C.newFramebuffer(2048,2048)
fbo:bind()
--testing scaling
ddsimg:draw(0,0,2048,2048)
--testing instant draw
npotimg:draw()
--testing instant draw
npotimg:draw(0,19)
--testing rotation
npotimg:draw(npotimg.w * 0.5, npotimg.h, nil, nil, math.pi/180, npotimg.w * 0.5, npotimg.h * 0.5)
npotimg:draw(0, npotimg.h*2, npotimg.w, npotimg.h, -math.pi/180)
npotimg:draw(npotimg.w, npotimg.h*4, npotimg.w, npotimg.h, math.pi/180, npotimg.w, npotimg.h)
--save result
fbo:save("result.bmp")
