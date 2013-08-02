/*******************************************************************************

Copyright (c) 2012-2013 Pavel Roschin (aka RPG) <rpg89@post.ru>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to
deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:  The above
copyright notice and this permission notice shall be included in all copies
or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.

*******************************************************************************/

#include <math.h>
#include <string.h>
#include <stdio.h>

#include "cheetah.h"
#include "macros.h"
#include "render.h"
#include "vertex.h"
#include "random.h"
#include "test.h"

void imageBind(Image * image);
void color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);


/* RNG push/pop functions to avoid internal state corruption */
static xor_state rng_state;
static inline void rng_push()
{
	memcpy(&rng_state, random_get_state(), sizeof(xor_state));
}
static inline void rng_pop()
{
	random_set_state(&rng_state);
}

static void particleSystemApplyForces(ParticleSystem *ptr, Particle *particle)
{

}

void newParticleSystem(ParticleSystem *ptr, Image *image, int maxParticles, const char *options) {
	NEEDED_INIT_VOID;
	new0(ptr->particles, Particle, maxParticles);
	ptr->maxParticles = maxParticles;
	ptr->image = image;
	ptr->_lasttime = globalTimers.timed;
	ptr->startSpeed = 50.0;
	ptr->directionVariation = M_PI;
	ptr->emissionRate = 100;
	ptr->particleLife = 3.0;
	memset(&ptr->color, 255, sizeof(Color));
	//~ memset(&ptr->colorVariation, 255, sizeof(Color));
}

static void particleSystemUpdate(ParticleSystem *ptr) {
	unsigned	i;
	Particle	*particle = ptr->particles;
	if(globalTimers.timed - ptr->_lasttime < 1.0/100.0)
		return;
	rng_push();
	float deltaTime = globalTimers.gameTimed - ptr->_lasttime;
	ptr->_lasttime = globalTimers.gameTimed;
	ptr->_particlesNeeded += ptr->emissionRate * deltaTime;
	for(i = 0; i < ptr->_aliveParticles; i++)
	{
		particle->age += deltaTime;
		random_seed128(particle->seed, 362436069, 521288629, 88675123);
		/* handle particles life cycle */
		if(particle->age >= ptr->particleLife + randf2(rand128()) * ptr->particleLifeVariation)
		{
			ptr->_aliveParticles--;
			memcpy(particle, &ptr->particles[ptr->_aliveParticles], sizeof(Particle));
			i--;
			continue;
		}
		particleSystemApplyForces(ptr, particle);
		particle->position.x += particle->speed.x * deltaTime;
		particle->position.y += particle->speed.y * deltaTime;
		particle++;
	}
	rng_pop();

	bool alive = globalTimers.gameTimed - ptr->_startTime < ptr->lifeTime;
	if((alive || ptr->lifeTime <= 0) && ptr->_particlesNeeded >= 1.0f)
	{
		unsigned particlesNeeded = ptr->_particlesNeeded;
		ptr->_particlesNeeded -= particlesNeeded;
		particle = &(ptr->particles[ptr->_aliveParticles]);
		for(i = 0; i < particlesNeeded; i++)
		{
			if(ptr->_aliveParticles >= ptr->maxParticles) break;
			memset(particle, 0, sizeof(Particle));
			float angle = ptr->direction + randf2(rand128()) * ptr->directionVariation;
			particle->speed.x = cosf(angle);
			particle->speed.y = sinf(angle);
			float startSpeed = ptr->startSpeed + randf2(rand128()) * ptr->startSpeedVariation;
			particle->speed.x *= startSpeed;
			particle->speed.y *= startSpeed;
			particle->seed = rand128();
			particle++;
			ptr->_aliveParticles++;
		}
	}
}

void particleSystemDraw(ParticleSystem *ptr, float x, float y) {
	unsigned	i;
	Particle	*particle = ptr->particles;
	Color c;
	union {
		Color cv;
		uint32_t cvi;
	} u;
	particleSystemUpdate(ptr);
	rng_push();
	imageBind(ptr->image);
	c = ptr->color;
	u.cv = ptr->colorVariation;
	for(i = 0; i < ptr->_aliveParticles; i++)
	{
		if(u.cvi) /* if have randomized color */
		{
			random_seed128(particle->seed, 362436069, 521288629, 88675123);
			#define RNDC(T) int T = (int)c.T + (((int)(rand128() & 255) - 128) * (int)u.cv.T) / 256;
			RNDC(r) RNDC(g) RNDC(b) RNDC(a)
			#undef RNDC
			color(r, g, b, a);
		}
		else
		{
			color(c.r, c.g, c.b, c.a);
		}
		PUSH_QUAD(particle->position.x + x, particle->position.y + y, ptr->image->w, ptr->image->h, 0, ptr->image->w * 0.5f, ptr->image->h * 0.5f);
		particle++;
	}
	rng_pop();
}


void deleteParticleSystem(ParticleSystem *ptr) {
	if(ptr)
		delete(ptr->particles);
}
