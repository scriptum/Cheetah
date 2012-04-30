require 'lib.cheetah'
--~ require 'lib.mixer'
require 'lib.lquery.init'
local C = cheetah
C.init('Test', 512, 512, 32, '')

C.newTilemap("data/telemap_test.map")

C.mainLoop()

