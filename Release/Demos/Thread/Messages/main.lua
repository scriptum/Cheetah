require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
--enable windows resizing and disable autoscale to avoid font corruption
C.init('Test', '320x240 vsync')

lQuery.addHook(function()
	local mess = C.threadRecv("worker")
	if mess and mess == "quit" then 
		print("Exit...")
		C.quit()
	end
	C.sleep(0.2)
end)

C.threadSend("message 1")
C.threadSend("message 2")
C.threadSend("message 3")
C.threadSend("Another queue", "main 2")

C.newThread("worker.lua")
C.mainLoop()
