require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Huge math load test', '1x1 noframe')

local start = os.clock()
for i = 1, 200 do
	C.generate('noise', 1024, 1024)
end
print(os.clock() - start)
