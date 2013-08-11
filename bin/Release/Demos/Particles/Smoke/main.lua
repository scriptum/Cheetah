require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Smoke Particle System')
local image = C.generate('lightexp', 32, 32)
local particleSystem = C.newParticleSystem(image)
particleSystem.particleLifeVariation = 0
particleSystem.colorVariation.r = 255
particleSystem.colorVariation.b = 255
particleSystem.colorVariation.g = 255
E:new(screen):draw(function()
	C.blendMode(C.blendAdditive)
	particleSystem:draw(400,300);
end)

C.mainLoop()
