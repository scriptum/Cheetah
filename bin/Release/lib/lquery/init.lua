--получение переменной по ссылке и задание ей значения невозможны в луа, так что костыли
var_by_ref = function(var, value)
	if value then loadstring(var .. '='..value)()
	else return loadstring('return '..var)()end
end

require("lib.lquery.entity")
require("lib.lquery.objects")

getMouseXY = cheetah.getMousePos

cheetah.render = function()
	mX, mY = getMouseXY()
	time = cheetah.getTime()
	--events
	local e, a, b, c = cheetah.poll()
	if e then lQuery.event(e,a,b,c) end
	lQuery.process()
end
 
lQuery.MousePressed = false
