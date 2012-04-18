require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Cheetah engine', 800, 600, 32, '')
require 'data.tahoma'
lQuery.addhook(C.clear)

local image = C.newImage('test.png')

local noisetex = C.newFramebuffer(64, 64, 'n4') --no interpolation
local noise = C.newShader[[
#version 120
float noise(vec2 c) {
	return fract(sin(dot(c, vec2(12.9898, 78.233))) * 43758.5453);
}
void main (void)
{
  gl_FragColor = vec4(noise(gl_TexCoord[0].st), noise(vec2(1.0) - gl_TexCoord[0].st), 0.0, 1.0);
}
]]
noisetex:bind()
noise:bind()
C.scale(64,64)
C.rectangle(true)
noisetex:unbind()
noise:unbind()

local shader = C.newShader[[
#version 120
uniform sampler2D tex;
uniform sampler2D noise;
uniform float time;
void main (void)
{
	const float t = 1/4;
	float i = 0.0, j = 0.0;
	vec4 c;
	vec2 pos;
	while (i < 1.0) 
	{
		i = i + 0.1;
		//for (j = 0.0; j < 1.0;) 
		{
			j = j + t;
			pos = texture2D(noise, vec2(i,j)).rg * 600.0;
			
			if(gl_FragCoord.x > pos.x && gl_FragCoord.x < pos.x + 40 && 
				gl_FragCoord.y > pos.y && gl_FragCoord.y < pos.y + 52)
				{
					c = vec4(1,0,0,1);
				}
		}
	}
  gl_FragColor = c;
}
]]
E:new(screen):draw(function()
	noisetex:draw()
end):size(800,600)

local smallFont = Fonts["Tahoma"][8]
E:new(screen):draw(function()
	smallFont:print("fps: " .. math.floor(C.FPS) .. ", mem: " .. gcinfo(), 100, 0)
end):move(0,0)

C.mainLoop()

