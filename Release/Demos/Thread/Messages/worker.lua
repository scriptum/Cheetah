require 'lib.cheetah'

print("Hello from worker!")
cheetah.sleep(0.3)
print("Another hello from worker!")
done = false
while not done do
mess = cheetah.threadRecv("main")
if mess then print(mess)
else done = true
end
cheetah.sleep(0.2)
end

mess = cheetah.threadRecv("main 2")
print(mess)

--send message to the main thread
cheetah.threadSend("quit", "worker")
--wait main thread to exit
cheetah.sleep(1)
