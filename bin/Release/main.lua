require 'lib.cheetah'
require 'lib.mixer'
require 'lib.lquery.init'
local C = cheetah
C.init('Test', 512, 512, 32, '')

require 'data.tahoma'

--~ C.enableDepthTest()
lQuery.addhook(C.clear)

local img = C.newImage('background.jpg')
local gauss = cheetah.newShader[[
#version 120
#pragma optionNV(fastmath off)
#pragma optionNV(fastprecision off)
uniform sampler2D tex;
uniform sampler2D gauss;
uniform float radius;
uniform float size;
float Gauss(float x)
{
	//return texture2D(gauss, vec2(y, 0.0)).r/radius;
	return 1.200066800267201*exp(-4.5*x*x)/radius;
}

void main(void)
{
	vec4 sum = texture2D(tex, vec2(gl_TexCoord[0].x, gl_TexCoord[0].y)) * Gauss(0);
	int max = int(radius);
	float y1, y2;
	vec2 uv;
	#pragma unroll
	for (int y = 1; y < max; y++)
	{
		y1 = y/size;
		y2 = y/radius;
		sum += texture2D(tex, vec2(gl_TexCoord[0].x , gl_TexCoord[0].y + y1)) * Gauss(y2);
		sum += texture2D(tex, vec2(gl_TexCoord[0].x , gl_TexCoord[0].y - y1)) * Gauss(y2);
		y++;
		y1 = y/size;
		y2 = y/radius;
		sum += texture2D(tex, vec2(gl_TexCoord[0].x , gl_TexCoord[0].y + y1)) * Gauss(y2);
		sum += texture2D(tex, vec2(gl_TexCoord[0].x , gl_TexCoord[0].y - y1)) * Gauss(y2);
	}
	gl_FragColor = sum;
}
]]
local gauss3 = cheetah.newShader[[
#version 120
#pragma optionNV(fastmath off)
#pragma optionNV(fastprecision off)
uniform sampler2D tex;
uniform float step;

void main()
{
	vec2 uv = gl_TexCoord[0].xy;
	float delta = step/512.0;
	vec4 color = texture2D(tex, uv) * 0.25;
	color += texture2D(tex, uv + delta * vec2(1.0, 0.0)) * 0.125;
	color += texture2D(tex, uv - delta * vec2(1.0, 0.0)) * 0.125;
	color += texture2D(tex, uv + delta * vec2(0.0, 1.0)) * 0.125;
	color += texture2D(tex, uv - delta * vec2(0.0, 1.0)) * 0.125;
	color += texture2D(tex, uv + delta * vec2(1.0, 1.0)) * 0.0625;
	color += texture2D(tex, uv - delta * vec2(1.0, 1.0)) * 0.0625;
	color += texture2D(tex, uv + delta * vec2(-1.0, 1.0)) * 0.0625;
	color += texture2D(tex, uv + delta * vec2(1.0, -1.0)) * 0.0625;
	gl_FragColor = color;
}
]]
local gauss1 = cheetah.newShader[[
#version 120
#pragma optionNV(fastmath off)
#pragma optionNV(fastprecision off)
uniform sampler2D tex;
uniform float step;

void main()
{
	vec2 uv = gl_TexCoord[0].xy;
	float delta = step/512.0;
	vec4 color = texture2D(tex, uv) * 0.5;
	color += texture2D(tex, uv + delta * vec2(1.0, 0.0)) * 0.25;
	color += texture2D(tex, uv - delta * vec2(1.0, 0.0)) * 0.25;
	gl_FragColor = color;
}
]]
local gauss2 = cheetah.newShader[[
#version 120
#pragma optionNV(fastmath off)
#pragma optionNV(fastprecision off)
uniform sampler2D tex;
uniform float step;

void main()
{
	vec2 uv = gl_TexCoord[0].xy;
	float delta = step/512.0;
	vec4 color = texture2D(tex, uv) * 0.5;
	color += texture2D(tex, uv + delta * vec2(0.0, 1.0)) * 0.25;
	color += texture2D(tex, uv - delta * vec2(0.0, 1.0)) * 0.25;
	gl_FragColor = color;
}
]]
--runtime shader
local function Gauss(t, radius)
	return 1.200066800267201*math.exp(-t*t*4.5)/radius;
end
local function generateGaussSHader(size, radius)
	radius = math.floor(radius)
	local shader = [[
	#version 120
	#pragma optionNV(fastmath off)
	#pragma optionNV(fastprecision off)
	uniform sampler2D tex;
	void main(void)
	{
		vec2 uv = gl_TexCoord[0].xy;
		vec4 sum = texture2D(tex, vec2(gl_TexCoord[0].x, gl_TexCoord[0].y)) * ]].. Gauss(0, radius) ..';'
	for i = 1, radius - 1 do
		shader = shader .. 'sum += texture2D(tex, vec2(uv.x, uv.y + '.. (i/size) ..')) * '.. Gauss(i/radius, radius) ..';'
		shader = shader .. 'sum += texture2D(tex, vec2(uv.x, uv.y - '.. (i/size) ..')) * '.. Gauss(i/radius, radius) ..';'
	end
	shader = shader .. 'gl_FragColor = sum;}'
	return shader
end

--~ local gauss = cheetah.newShader(generateGaussSHader(512, 50))
--~ print(gauss)
--~ gauss:bind()
--~ E:new(screen):draw(function(s)
	--~ gauss:set('tex', 0)
	--~ gauss:set('size', 512)
	--~ gauss:set('radius', 50)
	--~ img:draw()
--~ end):size(512,512)

--ещё хитрое размывание
fbo1 = C.newFramebuffer(512,512,'')
local scale = 1
fbo2 = C.newFramebuffer(512,512,'')
local function step(step, gauss)
	if step % 2 == 1 then fbo1:bind() else fbo2:bind() end
	C.scale(512,512)
	gauss:set('tex', 0)
	gauss:set('step', math.pow(2,step))
	--~ gauss:set('step', step)
	if step == 1 then img:draw()
	else
		if step % 2 == 1 then fbo2:draw() else fbo1:draw() end
	end
end
E:new(screen):draw(function(s)
	C.blend(false)
	gauss3:bind()
	C.push()
	for i = 1, 2 do
		step(i, gauss3)
	end
	gauss3:unbind()
	fbo2:unbind()
	C.pop()
	fbo2:draw()
	C.line(0,0,0.5,0.5)
end):size(512,512):draggable()

C.playMusic('data/03 - Cather Rhythm.ogg')
C.playMusic('data/11 - Impera.ogg')

local dir = C.opendir('.')
local de = C.readdir(dir)
print(de.name)

--~ require 'lib.table'
--~ table.print(jit)
C.mainLoop()