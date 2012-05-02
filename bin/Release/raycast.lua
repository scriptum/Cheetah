require 'lib.cheetah'
require 'lib.lquery.init'
local C = cheetah
C.init('Test', 512, 512, 32, '')
C.printFPS = true
local img = C.newImage('data/crate.png')

local polar = C.newShader([[#version 120
uniform float time;
const vec3 eye = vec3(.0,.0,-1.);
const vec3 eps = vec3(.00001,.0,.0);
uniform sampler2D texture;

float s,c;

// scene definition
#define sphere(p, r) (length(p) - r)
#define h(p) \
	 min(sphere(p - vec3(1. + c*.5, .2 + s*.5, 5.), 1.7), \
	     sphere(p - vec3(-2.,.5, 6.+c), 1.5))

// sorry...
vec4 raycast(vec3 p, vec3 ray, float delta)
{
	float d;
#define STEP() d = h(p); if (d <= delta) return vec4(p,d); p+= ray * d
	STEP(); STEP(); STEP(); STEP(); STEP(); STEP();
	STEP(); STEP(); STEP(); STEP(); STEP(); STEP();
	STEP(); STEP(); STEP(); STEP(); STEP(); STEP();
	return vec4(p,d);
}

#define grad(f, p) normalize(vec3( f(p+eps) - f(p-eps), f(p+eps.yxz) - f(p-eps.yxz), f(p+eps.zyx) - f(p-eps.zyx) ));

void main(void)
{
	s = sin(time); c = cos(time);
	vec2 tc = gl_TexCoord[0].xy;
	tc = 2.0 * tc - vec2(1.0);
	vec3 ray = normalize(vec3(tc.x, tc.y, .0) - eye);

	vec4 info = raycast(eye, ray, .01);
	float d = info.w;
	if (d > .01) discard;

	vec3 p = info.xyz;
	vec3 n = grad(h, p);

	// fast fake ambient occlusion
	float delta = 1.;
	float ao = .1;
	ao += (1. * delta - h(p + n * 1. * delta)) / pow(2.0, 1.);
	ao += (2. * delta - h(p + n * 2. * delta)) / pow(2.0, 2.);
	ao += (3. * delta - h(p + n * 3. * delta)) / pow(2.0, 3.);
	ao = 1.0 - .3 * ao;

	// use normal for texture coordinates
	tc = n.zy * .5 + vec2(.5);
	gl_FragColor = texture2D(texture, tc) * vec4(ao,ao,ao, 1.0);
}
]])
E:new(screen):draw(function()
	polar:bind()
	polar:set('tex', 0)
	polar:set('time', time)
	img:drawxy(0,0,512,512)
	polar:unbind()
end)

C.mainLoop()