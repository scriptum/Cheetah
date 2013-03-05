------------ THIS IS THE SAMPLE APPLICATION FOR CHEETAH 2D ENGINE --------------

require 'lib.cheetah'
--~ require 'lib.mixer'
require 'lib.lquery.init'
local C = cheetah
C.init('Test application', '800x600 vsync')


C.mainLoop()
