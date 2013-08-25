ffi = require 'ffi'
ffi.cdef[[
void sleep(unsigned int sec);
]]


-- require 'lib.cheetah'

print("Hello from worker!")
ffi.C.sleep(1)
print("Another hello from worker!")
