require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Huge math load test', '1x1 noframe')
C.printFPS = true
local N = 256
local img = C.generate('dummy', 1, 1)
local start = os.clock()
for k = 1, 300 do
	for j = 1, N do
		for i = 1, N do
			img:draw(i, j, 1.5,1.5, 0.123, 0.111, 0.111)
			img:draw(i, j, 1.5,1.5)
		end
	end
end
print(os.clock() - start)