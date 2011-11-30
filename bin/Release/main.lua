require 'libcheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Test', 512, 512, 32, '')

require 'data.tahoma'

--~ C.enableDepthTest()
lQuery.addhook(C.clear)

local img = C.newImage('background.dds')

local shader = cheetah.newShader[[
#version 120
#pragma optionNV(fastmath off)
#pragma optionNV(fastprecision off)
uniform sampler2D tex;
uniform float radius;
uniform float size;
float Gauss(float y)
{
	return exp(-y*y*3.058)/radius;
}

void main(void)
{
	vec4 sum = texture2D(tex, vec2(gl_TexCoord[0].x, gl_TexCoord[0].y)) * Gauss(0);
	int max = int(radius);
	float y1, y2;
	vec2 uv;
	for (int y = 1; y < max; y++)
	{
		y1 = y/size;
		y2 = y/radius;
		sum += texture2D(tex, vec2(gl_TexCoord[0].x , gl_TexCoord[0].y + y1)) * Gauss(y2);
		sum += texture2D(tex, vec2(gl_TexCoord[0].x , gl_TexCoord[0].y - y1)) * Gauss(y2);
	}
	gl_FragColor = sum;
}
]]

shader:bind()
E:new(screen):draw(function()
	shader:set('tex', 0)
	shader:set('radius', 10)
	shader:set('size', 512)
	img:draw()
end):size(512,512)





C.mainLoop()