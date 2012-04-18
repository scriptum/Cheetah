require 'libcheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Test', 512, 512, 32, '')

local gauss = cheetah.newShader[[
#version 120
#pragma optionNV(fastmath off)
#pragma optionNV(fastprecision off)
float Gauss(float x)
{
	//return 1.200066800267201*exp(-4.5*x*x);
	return exp(-4.5*x*x);
}

void main(void)
{

	gl_FragColor = vec4(vec3(Gauss(gl_TexCoord[0].x)), 1.0);
}
]]
--~ local fbo = C.newFramebuffer(128,2,8,false,false,false)
--~ fbo:bind()
--~ C.scale(128,2)


--~ local gauss = cheetah.newShader(generateGaussSHader(512, 50))
--~ print(gauss)
gauss:bind()
E:new(screen):draw(function(s)
	C.rectangle(true)
end):size(128,512)





C.mainLoop()