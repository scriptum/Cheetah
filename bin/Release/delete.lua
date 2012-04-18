require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
local parent = E:new(screen)
local ents = {}
for i = 1, 100000 do
	ents[i] = E:new(parent)
end
print(gcinfo())
parent = parent:delete()
ents = nil
collectgarbage()
print(gcinfo())