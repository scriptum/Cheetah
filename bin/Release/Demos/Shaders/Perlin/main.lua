require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Perlin noise', '1024x1024')
C.printFPS = true
local noiseSize = 512
local noise = C.generate('noise', noiseSize, noiseSize, 'nearest')
--~ local noisePrepare = C.newShader([[#version 120
--~ uniform sampler2D noiseTex;
--~ uniform vec2 texel;
--~ varying vec2 TexCoord;
--~ void main() {
	--~ float n00 = perm(noise2d( x11/rep),                   pixelPosition);
	--~ float n10 = perm(noise2d((x11 + vec2(1.0, 0.0))/rep), pixelPosition - vec2(1.0, 0.0));
	--~ float n01 = perm(noise2d((x11 + vec2(0.0, 1.0))/rep), pixelPosition - vec2(0.0, 1.0));
	--~ float n11 = perm(noise2d((x11 + vec2(1.0, 1.0))/rep), pixelPosition - vec2(1.0, 1.0));
--~ }
--~ ]])
local fbo = C.newFramebuffer(1024,1024)
local shader = C.newShader('perlin.glsl')
local scale, persist, steps, contrast, brightness = 16, 0.5, 16, 1, 0
E:new(screen):draw(function()
	shader:bind()
	shader:set('scale', scale, scale)
	shader:set('persist', persist)
	shader:set('contrast', contrast)
	shader:set('brightness', brightness)
	shader:set('steps', steps)
	shader:set('texel', 1/noiseSize, 1/noiseSize)
	shader:set('noiseSize', noiseSize, noiseSize)
	shader:set('noiseTex', 0)
	--~ fbo:bind()
	noise:drawq(0, 0, 1024, 1024)
	--~ fbo:unbind()
	--~ shader:unbind()
	--~ fbo:drawq(0, 0, 1024, 1024, 512, 512)
end)

C.mainLoop()
