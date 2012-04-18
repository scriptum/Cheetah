require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Test', 512, 512, 32, '')
C.printFPS = true
local img = C.newImage('data/h.png')
local img1 = C.newImage('data/13.png')
local img2 = C.newImage('data/28.png')
--~ local img3 = C.newImage('data/38.png')
lQuery.addhook(C.clear)

--~ local polar = C.newShader([[#version 120
--~ uniform sampler2D tex;
--~ void main() {
	--~ float a = -(gl_TexCoord[0].x+0.25)*3.1415926*2;
	--~ float r = sqrt(gl_TexCoord[0].y)*0.5;
	--~ float c = texture2D(tex, vec2(r*sin(a)+0.5, r*cos(a)+0.5)).g;
	--~ gl_FragColor = vec4(c,c,c,1.0);
--~ }
--~ ]])
--~ 
--~ local polar_rev = C.newShader([[#version 120
--~ uniform sampler2D tex;
--~ const vec2 half = vec2(0.5, 0.5);
--~ void main() {
	--~ float c;
	--~ float p = distance(gl_TexCoord[0].xy, half)*2;
	--~ p = p*p;
	--~ float f = 0.25-atan((gl_TexCoord[0].x-0.5)/(gl_TexCoord[0].y-0.5))/3.1415926/2;
	--~ if(gl_TexCoord[0].y < 0.5)
		--~ c = texture2D(tex, vec2(f,p)).g;
	--~ else
		--~ c =  texture2D(tex, vec2(f+0.5,p)).g;
	--~ gl_FragColor = vec4(c,c,c,1.0);
--~ }
--~ ]])


local fbo = {}
fbo[1] = C.newFramebuffer(512, 512, '')
fbo[2] = C.newFramebuffer(512, 512, '')
E:new(screen):draw(function()
	C.push()
	C.blendMode(0)
	fbo[2]:bind()
	--~ C.scale(512, 512)
	--~ img:draw()
	C.clear()
	C.scale(128, 128)
	C.move(0,math.sin(time*1.2)+1)
	img1:draw()
	C.move(1-math.sin(time)+1,-math.sin(time*1.2))
	img2:draw()
	C.move(-1,1)
	img1:draw()
	fbo[1]:bind()
	C.scale(512, 512)
	fbo[2]:draw()
	--~ polar:bind()
	--~ fbo[2]:draw()
	
	C.blendMode(C.blendAdditive)
	--~ C.move(0, 1.5/512)
	--~ fbo[2]:draw()
	--~ C.move(0, 1/512)
	--~ fbo[2]:draw()
	--~ polar:unbind()
	local of = {0, 1}
	
	--~ fbo[2]:bind()
	--~ C.scale(512, 512)
	--~ C.setBlendMode(0)
	--~ fbo[1]:draw()
	--~ C.setBlendMode(C.blendAdditive)
	--~ C.move(0, 2/512)
	--~ fbo[1]:draw()
	local i = 0
	while true do
		fbo[i%2+1]:bind()
		C.imageDrawqxy(fbo[(i+1)%2+1].image, 0, 0, 512, 512, of[i%2+1], of[i%2+1], 512 - of[i%2+1]*2, 512 - of[i%2+1]*2)
		of[i%2+1] = of[i%2+1] + of[(i+1)%2+1]*0.8
		if of[i%2+1] > 256 then break end
		i = i + 1
	end
	fbo[2]:unbind()
	C.pop()
	C.scale(512,512)
	--~ polar_rev:bind()
	C.blendMode(0)
	fbo[i%2+1]:draw()
	--~ polar_rev:unbind()
	--~ C.scale(1/4, 1/4)
	--~ C.move(0,math.sin(time*1.2)+1)
	--~ img1:draw()
	--~ C.move(1-math.sin(time)+1,-math.sin(time*1.2))
	--~ img2:draw()
	--~ C.move(-1,1)
	--~ img1:draw()
end)
:translate()
--~ :hide()



--~ E:new(screen):draw(function()
	--~ fbo[2]:bind()
	--~ C.scale(512,512)
	--~ polar:bind()
	--~ img:draw()
	--~ polar:unbind()
	--~ fbo[2]:unbind()
	--~ C.scale(512,512)
	--~ polar_rev:bind()
	--~ fbo[2]:draw()
	--~ polar_rev:unbind()
--~ end):size(512,512)

C.mainLoop()