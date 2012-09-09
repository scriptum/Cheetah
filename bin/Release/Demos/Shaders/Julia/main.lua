require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Test', 800, 600, 32, 'v')

local shader = C.newShader([[#version 120
uniform float time;
uniform sampler2D texture;
void main(void)
{
	vec2 p = vec2(-2.0, -1.5) + vec2(4.0, 3.0) * gl_TexCoord[0].xy;
	vec2 cc = vec2( cos(.25*time), sin(.25*time*1.423) );

	float dmin = 1000.0;
	vec2 z  = p;
	#pragma unroll
	for( int i=0; i<16; i++ )
	{
		z = cc + vec2( z.x*z.x - z.y*z.y, 2.0*z.x*z.y );
		float m2 = dot(z,z);
		if( m2>100.0 ) break;
		dmin=min(dmin,m2);
		z = cc + vec2( z.x*z.x - z.y*z.y, 2.0*z.x*z.y );
		m2 = dot(z,z);
		if( m2>100.0 ) break;
		dmin=min(dmin,m2);
		z = cc + vec2( z.x*z.x - z.y*z.y, 2.0*z.x*z.y );
		m2 = dot(z,z);
		if( m2>100.0 ) break;
		dmin=min(dmin,m2);
	}
	dmin = sqrt(dmin);
	gl_FragColor = vec4(texture2D(texture,vec2((dmin-time*0.2)*0.02, (dmin+time*0.1)*0.02)));
}

]])
shader:bind()
img = C.generate("noise", 64, 64)
E:new(screen):set({time = 0})
:draw(function(s)
	shader:set('time', time*2)
	img:drawxy(0,0,800,600)
end)
C.mainLoop()