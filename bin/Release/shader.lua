require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Test', 512, 512, 32, '')
C.printFPS = true
local img = C.newImage('data/h.png')

local polar = C.newShader([[#version 120
uniform sampler2D tex;
void main() {
	float a = -(gl_TexCoord[0].x+0.25)*3.141592653589793*2;
	float r = sqrt(gl_TexCoord[0].y)*0.5;
	float c = texture2D(tex, vec2(r*sin(a)+0.5, r*cos(a)+0.5)).g;
	gl_FragColor = vec4(c,c,c,1.0);
}
]])
E:new(screen):draw(function()
	polar:bind()
	polar:set('tex', 0)
	img:drawxy(0,0,512,512)
	polar:unbind()
end)

C.mainLoop()