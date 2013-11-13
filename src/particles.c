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
#include "cmacros.h"
#include "render.h"
#include "cvertex.h"
#include "crandom.h"
#include "test.h"

void imageBind(Image * image);
void color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);


/* RNG push/pop functions to avoid internal state corruption */
static xor_state rng_state;
static inline void rng_push(void)
{
	memcpy(&rng_state, random_get_state(), sizeof(xor_state));
}
static inline void rng_pop(void)
{
	random_set_state(&rng_state);
}

static void particleSystemApplyForces(ParticleSystem *ptr, Particle *particle)
{

}

void newParticleSystem(ParticleSystem *ptr, Image *image, unsigned maxParticles, const char *options) {
	NEEDED_INIT_VOID;
	new0(ptr->particles, Particle, maxParticles);
	ptr->maxParticles = maxParticles;
	ptr->image = image;
	ptr->_lasttime = globalTimers.timed;
	ptr->startSpeed = 50.0f;
	ptr->directionVariation = (float)M_PI;
	ptr->emissionRate = 100.0f;
	ptr->particleLife = 3.0f;
	ptr->scale = 1.0f;
	memset(&ptr->color, 255, sizeof(Color));
	//~ memset(&ptr->colorVariation, 255, sizeof(Color));
}

static void particleSystemUpdate(ParticleSystem *pSystem) {
	unsigned	i;
	Particle	*particle = pSystem->particles;
	if(globalTimers.timed - pSystem->_lasttime < 1.0/100.0)
		return;
	float deltaTime = (float)(globalTimers.gameTimed - (double)pSystem->_lasttime);
	pSystem->_lasttime = globalTimers.gameTimed;
	pSystem->_particlesNeeded += pSystem->emissionRate * deltaTime;
	for(i = 0; i < pSystem->_aliveParticles; i++)
	{
		particle->age -= deltaTime;
		if(particle->age < 0.0f)
		{
			pSystem->_aliveParticles--;
			memcpy(particle, &pSystem->particles[pSystem->_aliveParticles], sizeof(Particle));
			i--;
			continue;
		}
		particleSystemApplyForces(pSystem, particle);
		particle->position.x += particle->speed.x * deltaTime;
		particle->position.y += particle->speed.y * deltaTime;
		particle++;
	}

	bool alive = globalTimers.gameTimed - pSystem->_startTime < pSystem->lifeTime;
	if((alive || pSystem->lifeTime <= 0) && pSystem->_particlesNeeded >= 1.0f)
	{
		unsigned particlesNeeded = (unsigned)pSystem->_particlesNeeded;
		pSystem->_particlesNeeded -= (float)particlesNeeded;
		particle = &(pSystem->particles[pSystem->_aliveParticles]);
		for(i = 0; i < particlesNeeded; i++)
		{
			if(pSystem->_aliveParticles >= pSystem->maxParticles) break;
			memset(particle, 0, sizeof(Particle));
			float angle = pSystem->direction + randf2(rand128()) * pSystem->directionVariation;
			particle->speed.x = cosf(angle);
			particle->speed.y = sinf(angle);
			float startSpeed = pSystem->startSpeed + randf2(rand128()) * pSystem->startSpeedVariation;
			particle->speed.x *= startSpeed;
			particle->speed.y *= startSpeed;
			#define RNDC(T) particle->color.T = (unsigned char)((int)pSystem->color.T + (((int)(rand128() & 255) - 128) * (int)pSystem->colorVariation.T) / 256);
			RNDC(r) RNDC(g) RNDC(b) RNDC(a)
			#undef RNDC
			particle->age = pSystem->particleLife + randf2(rand128()) * pSystem->particleLifeVariation;
			particle->scale = pSystem->scale + randf2(rand128()) * pSystem->scaleVariation;
			particle++;
			pSystem->_aliveParticles++;
		}
	}
}

void particleSystemDraw(ParticleSystem *ptr, float x, float y) {
	unsigned	i;
	Particle	*particle = ptr->particles;
	particleSystemUpdate(ptr);
	imageBind(ptr->image);
	for(i = 0; i < ptr->_aliveParticles; i++)
	{
		color(particle->color.r, particle->color.g, particle->color.b, particle->color.a);
		PUSH_QUAD(particle->position.x + x, particle->position.y + y, ptr->image->w, ptr->image->h, 0, ptr->image->w * 0.5f, ptr->image->h * 0.5f);
		particle++;
	}
	// unsigned	i;
	// Particle	*particle = ptr->particles;
	// Color particleColor;
	// union {
		// Color particleColor;
		// uint32_t colorUint;
	// } colorUnion;
	// particleSystemUpdate(ptr);
	// rng_push();
	// imageBind(ptr->image);
	// particleColor = ptr->color;
	// colorUnion.particleColor = ptr->colorVariation;
	// for(i = 0; i < ptr->_aliveParticles; i++)
	// {
		// if(colorUnion.colorUint) /* if have randomized color */
		// {
			// crandom.hash_seed128((uint32_t)particle, 362436069, 521288629, 88675123);
			// #define RNDC(T) int T = (int)particleColor.T + (((int)(rand128() & 255) - 128) * (int)colorUnion.particleColor.T) / 256;
			// RNDC(r) RNDC(g) RNDC(b) RNDC(a)
			// #undef RNDC
			// color(r, g, b, a);
		// }
		// else
		// {
			// color(particleColor.r, particleColor.g, particleColor.b, particleColor.a);
		// }
		// PUSH_QUAD(particle->position.x + x, particle->position.y + y, ptr->image->w, ptr->image->h, 0, ptr->image->w * 0.5f, ptr->image->h * 0.5f);
		// particle++;
	// }
	// rng_pop();
}


void deleteParticleSystem(ParticleSystem *ptr) {
	if(ptr)
		delete(ptr->particles);
}
