--[[****************************************************************************

Copyright (c) 2012 Pavel Roschin (aka RPG) <rpg89@post.ru>

Permission is hereby granted, free of charge, to any person obtaining a copy 
of this software and associated documentation files (the "Software"), to 
deal in the Software without restriction, including without limitation the 
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
sell copies of the Software, and to permit persons to whom the Software is 
furnished to do so, subject to the following conditions:  The above 
copyright notice and this permission notice shall be included in all copies 
or substantial portions of the Software.
 
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
IN THE SOFTWARE.

******************************************************************************]]

--получение переменной по ссылке и задание ей значения невозможны в луа, так что костыли
var_by_ref = function(var, value)
	if value then loadstring(var .. '='..value)()
	else return loadstring('return '..var)()end
end

require("lib.lquery.entity")
require("lib.lquery.objects")

lQuery.getMouseXY = cheetah.getMousePos
time = cheetah.getTime()

cheetah.render = function()
	mX, mY = lQuery.getMouseXY()
	time = cheetah.getTime()
	--events
	local e, a, b, c = cheetah.poll()
	if e then lQuery.event(e,a,b,c) end
	lQuery.process()
end
 
lQuery.MousePressed = false
