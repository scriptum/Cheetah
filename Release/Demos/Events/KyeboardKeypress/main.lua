require 'lib.cheetah'
require 'lib.lquery.init'

--alias for all cheetah's functions
local C = cheetah

--800x600 with vertical synch by default
C.init('Keyboard events')

--show FPS in console and window title
C.printFPS = true

--generate simple image
player_img = C.generate("circle", 32, 32)

local player = E:new(screen):image(player_img)
:move(400-16,300-16)
:keypress(function(self, key)
	if key == 'w' then
		self:move(self.x, self.y - 16)
	elseif key == 's' then
		self:move(self.x, self.y + 16)
	elseif key == 'a' then
		self:move(self.x - 16, self.y)
	elseif key == 'd' then
		self:move(self.x + 16, self.y)
	end
end)
--do not forget about main loop cycle!
C.mainLoop()