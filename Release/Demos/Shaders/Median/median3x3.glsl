/*
3x3 Median
Morgan McGuire and Kyle Whitson
http://graphics.cs.williams.edu


Copyright (c) Morgan McGuire and Williams College, 2006
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Copyright (c) 2013 adapted by Pavel Roschin aka RPG (rpg89@post.ru)

*/
#version 120

#define INTENSITY_MODE

// Input texture
uniform sampler2D T;

varying vec2 TexCoord;

// vec2(1/width, 1/height) of the texture
    uniform vec2 texel;
#define s2(a, b)				temp = a; a = min(a, b); b = max(temp, b);
#define mn3(a, b, c)			s2(a, b); s2(a, c);
#define mx3(a, b, c)			s2(b, c); s2(a, c);

#define mnmx3(a, b, c)			mx3(a, b, c); s2(a, b);                                   // 3 exchanges
#define mnmx4(a, b, c, d)		s2(a, b); s2(c, d); s2(a, c); s2(b, d);                   // 4 exchanges
#define mnmx5(a, b, c, d, e)	s2(a, b); s2(c, d); mn3(a, c, e); mx3(b, d, e);           // 6 exchanges
#define mnmx6(a, b, c, d, e, f) s2(a, d); s2(b, e); s2(c, f); mn3(a, b, c); mx3(d, e, f); // 7 exchanges

#ifdef INTENSITY_MODE
#define vec float
const vec3 luma = vec3(0.299, 0.587, 0.114);
#define getvec(v) dot(luma, v.rgb)
#else
#define vec vec4
#define getvec(v) v
#endif

#define get(x, y) getvec(texture2D(T, TexCoord + vec2(x, y) * texel)) 

void main() {
	vec v[6];
	vec4 central = texture2D(T, TexCoord);
	v[0] = get(-1, -1);
	v[1] = get( 0, -1);
	v[2] = get( 1, -1);
	v[3] = get(-1,  0);
	v[4] = getvec(central);
	v[5] = get( 1,  0);
	vec temp;
	mnmx6(v[0], v[1], v[2], v[3], v[4], v[5]);
	v[5] = get(-1, 1);
	mnmx5(v[1], v[2], v[3], v[4], v[5]);
	v[5] = get(0, 1);
	mnmx4(v[2], v[3], v[4], v[5]);
	v[5] = get(1, 1);
	mnmx3(v[3], v[4], v[5]);
	#ifdef INTENSITY_MODE
	float Y = dot(luma, central.rgb);
	//~ gl_FragColor = vec4(v[4] + central.r - Y, v[4] - (central.b - Y)*0.194 - (central.r - Y)*0.509, v[4] + central.b - Y, central.a);
	gl_FragColor = vec4(v[4] + central.r - Y, v[4] + central.g - Y, v[4] + central.b - Y, central.a);
	//~ gl_FragColor = vec4(vec3(v[4]), central.a);
	#else
	gl_FragColor = v[4];
	#endif
}