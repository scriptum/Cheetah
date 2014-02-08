require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Julia fractal', 'resizable')
C.printFPS = true
local shader = C.newShader([[#version 100
#ifdef GL_ES
precision highp float;
#endif
uniform float time;
uniform sampler2D texture;
uniform vec2 cc;
varying vec2 TexCoord;
void main(void)
{
	vec2 p = vec2(-2.0, -1.5) + vec2(4.0, 3.0) * TexCoord;

	float dmin = 1000.0;
	vec2 z  = p;
	#pragma unroll
	for( int i=0; i<128; i++ )
	{
		z = cc + vec2( z.x*z.x - z.y*z.y, 2.0*z.x*z.y );
		float m2 = dot(z,z);
		if( m2>100.0 ) break;
		dmin=min(dmin,m2);
	}
	dmin = clamp(sqrt(dmin), 0., 1.);
	vec4 color = texture2D(texture,vec2((dmin-time*0.03645)*0.1, (dmin+time*0.02154)*0.1));
	gl_FragColor = vec4((color - .5) * 3. + .5);
}

]])
local c1, c2 = math.random(1.012847, 1.9234), math.random(0.985123, 2.2381678)
shader:bind()
img = C.generate("noise", 16, 16)
E:new(screen):set({time = 0})
:draw(function(s)
	shader:set('cc', math.pow(math.cos(.2*time*c1), 3), math.pow(math.cos(.2*time*c2), 3))
	--~ shader:set('cc', (lQuery.mX-400)/400, (lQuery.mY-300)/300)
	shader:set('time', time*2)
	img:draw(0,0,800,600)
end)
C.mainLoop()