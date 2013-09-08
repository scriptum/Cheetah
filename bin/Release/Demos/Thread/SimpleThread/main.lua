require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
--enable windows resizing and disable autoscale to avoid font corruption
C.init('Test', '1024x700')

cheetah.newThread("worker.lua")
cheetah.mainLoop()
