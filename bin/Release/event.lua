require 'lib.cheetah'
cheetah.init('Test', 512, 512, 32, '')
cheetah.render = function()
	local e, a, b, c = cheetah.poll()
	if e then print(e, a, b, c) end
end

cheetah.mainLoop()