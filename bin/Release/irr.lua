require 'libcheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Test', 512, 512, 32, '')

require 'data.tahoma'

--~ C.enableDepthTest()
--~ lQuery.addhook(C.clear)

local img = C.newImage('background.jpg')
local gauss = cheetah.newShader[[
#version 120
#pragma optionNV(fastmath off)
#pragma optionNV(fastprecision off)
uniform sampler2D tex;
void main(void)
{
	vec4 sum = 0.8*texture2D(tex, vec2(gl_TexCoord[0].x, gl_TexCoord[0].y))+0.1*texture2D(tex, vec2(gl_TexCoord[0].x - 1.0/512.0, gl_TexCoord[0].y));
	gl_FragColor = sum;
}
]]
local fbo = C.newFramebuffer(512,512,8,false,false,false)
fbo:bind()
C.push()
C.scale(512,512)
img:draw()
gauss:bind()
gauss:set('tex', 0)
fbo:draw()
fbo:draw()
fbo:draw()
fbo:draw()fbo:draw()
fbo:draw()
fbo:draw()
fbo:draw()fbo:draw()
fbo:draw()
fbo:draw()
fbo:draw()fbo:draw()
fbo:draw()
fbo:draw()
fbo:draw()fbo:draw()
fbo:draw()
fbo:draw()
fbo:draw()fbo:draw()
fbo:draw()
fbo:draw()
fbo:draw()fbo:draw()
fbo:draw()
fbo:draw()
fbo:draw()fbo:draw()
fbo:draw()
fbo:draw()
fbo:draw()fbo:draw()
fbo:draw()
fbo:draw()
fbo:draw()fbo:draw()
fbo:draw()
fbo:draw()
fbo:draw()fbo:draw()
fbo:draw()
fbo:draw()
fbo:draw()fbo:draw()
fbo:draw()
fbo:draw()
fbo:draw()
fbo:unbind()
gauss:unbind()
C.pop()
E:new(screen):draw(function(s)
	fbo:draw()
end):size(512,512)





C.mainLoop()