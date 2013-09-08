require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
--enable windows resizing and disable autoscale to avoid font corruption
C.init('Test', '1024x768')
C.printFPS = true
-- C.autoScale(false)
--load font
local f = C.newFont('DejaVuSans.fnt', true)
f.distanceField = true
f:setScale(0.4)
local tip = C.newBorderImage('tooltip.png', 2, 2, 2, 2)

local text = {
	"Lorem ipsum dolor sit amet, consectetur adipiscing elit.",
	"Morbi malesuada vulputate dui, ut ullamcorper sapien eleifend non. Vivamus ut ligula eu lectus tempus placerat. Suspendisse vitae leo leo. Aliquam dictum nisi velit, vitae blandit dui rhoncus id. In commodo bibendum varius. Nulla elementum turpis nec cursus semper. Donec quis eleifend dolor. Donec elementum vehicula vestibulum.",
	"Pellentesque elementum tortor faucibus, ultrices sem eget, hendrerit eros. Integer vitae sapien a massa suscipit egestas scelerisque et nisl. Mauris accumsan nisl ut mollis mattis. Nullam nec sem ultrices, luctus risus mattis, ullamcorper sem. Cras ornare sagittis feugiat. Etiam feugiat dignissim malesuada. Mauris commodo neque sed nisl molestie venenatis. Cras mi arcu, convallis eu tempus vel, semper semper mi. Integer suscipit dapibus augue at imperdiet.",
	"Donec consequat ornare felis, faucibus consectetur turpis feugiat in. Donec faucibus at nulla at iaculis. Etiam id tempus augue. Nullam dignissim dolor risus. Praesent luctus sem sapien, vitae elementum arcu auctor ut. Nunc non convallis ante, tempor rhoncus eros. Nulla accumsan faucibus eros ac sagittis. In hac habitasse platea dictumst. Etiam eu pharetra ligula. Donec non tempus sapien. Praesent congue sollicitudin arcu luctus porttitor. Phasellus quis ullamcorper est, ac congue quam. Fusce at metus nunc. Nulla facilisi.",
	"Vivamus dapibus elit sed neque ornare, quis consectetur sem lacinia. Interdum et malesuada fames ac ante ipsum primis in faucibus. Nunc pellentesque luctus velit, vel imperdiet lectus. Pellentesque orci felis, ullamcorper vitae mauris non, vestibulum ultrices enim. Curabitur eu lacinia sapien. Phasellus gravida auctor magna a convallis. Pellentesque ligula augue, interdum at velit sit amet, tempus euismod lacus. Fusce et iaculis nisi, ut euismod sapien. Ut laoreet volutpat rutrum. Vivamus varius quis quam et faucibus. Sed consequat quam vehicula ante molestie, nec porta elit fringilla. Vivamus dapibus elit sed neque ornare, quis consectetur sem lacinia. Interdum et malesuada fames ac ante ipsum primis in faucibus. Nunc pellentesque luctus velit, vel imperdiet lectus. Pellentesque orci felis, ullamcorper vitae mauris non, vestibulum ultrices enim. Curabitur eu lacinia sapien. Phasellus gravida auctor magna a convallis. Pellentesque ligula augue, interdum at velit sit amet, tempus euismod lacus. Fusce et iaculis nisi, ut euismod sapien. Ut laoreet volutpat rutrum. Vivamus varius quis quam et faucibus. Sed consequat quam vehicula ante molestie, nec porta elit fringilla. Vivamus dapibus elit sed neque ornare, quis consectetur sem lacinia. Interdum et malesuada fames ac ante ipsum primis in faucibus. Nunc pellentesque luctus velit, vel imperdiet lectus. Pellentesque orci felis, ullamcorper vitae mauris non, vestibulum ultrices enim. Curabitur eu lacinia sapien. Phasellus gravida auctor magna a convallis. Pellentesque ligula augue, interdum at velit sit amet, tempus euismod lacus. Fusce et iaculis nisi, ut euismod sapien. Ut laoreet volutpat rutrum. Vivamus varius quis quam et faucibus. Sed consequat quam vehicula ante molestie, nec porta elit fringilla. Vivamus dapibus elit sed neque ornare, quis consectetur sem lacinia. Interdum et malesuada fames ac ante ipsum primis in faucibus. Nunc pellentesque luctus velit, vel imperdiet lectus. Pellentesque orci felis, ullamcorper vitae mauris non, vestibulum ultrices enim. Curabitur eu lacinia sapien. Phasellus gravida auctor magna a convallis. Pellentesque ligula augue, interdum at velit sit amet, tempus euismod lacus. Fusce et iaculis nisi, ut euismod sapien. Ut laoreet volutpat rutrum. Vivamus varius quis quam et faucibus. Sed consequat quam vehicula ante molestie, nec porta elit fringilla."
}

E:new(screen)
:draw(function(s)
	local padding = 5
	local n = math.floor(time /2) % #text + 1
	local line_h = f:getLineHeight()
	s.w = math.sqrt(#text[n] * line_h * line_h)
	s.w = math.max(math.min(s.w, 1000), 300)
	local maxw = s.w - padding * 2
	tip:draw(s.x, s.y, s.w, f:getHeight(text[n], maxw))
	f:print(text[n], s.x + padding, s.y + padding, maxw, C.align_justify)
end)
C.mainLoop()
