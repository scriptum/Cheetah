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

void imageBind(Image * image);

void newParticleSystem(ParticleSystem *ptr, Image *image, int maxParticles, const char *options) {
	NEEDED_INIT_VOID;
	new0(ptr->particles, Particle, maxParticles);
	ptr->maxParticles = maxParticles;
	ptr->image = image;
	ptr->_lasttime = globalTimers.timed;
	ptr->startSpeed = 100.0;
	ptr->directionVariation = 1;
	ptr->emissionRate = 100;
	ptr->particleLife = 5.0;
	ptr->particleLifeVariation = 1.0;
}

static void particleSystemUpdate(ParticleSystem *ptr) {
	unsigned	i;
	unsigned	seed = random_get_seed();
	Particle	*particle = ptr->particles;
	float		deltaTime;
	if(globalTimers.timed - ptr->_lasttime < 1.0/100.0)
		return;
	deltaTime = globalTimers.gameTimed - ptr->_lasttime;
	ptr->_lasttime = globalTimers.gameTimed;
	ptr->_particlesNeeded += ptr->emissionRate * deltaTime;
	for(i = 0; i < ptr->_aliveParticles; i++)
	{
		particle->age += deltaTime;
		random_seed(particle->seed);
		if(particle->age >= ptr->particleLife + randf2(rand128()) * ptr->particleLifeVariation)
		{
			ptr->_aliveParticles--;
			memcpy(particle, &ptr->particles[ptr->_aliveParticles], sizeof(Particle));
			i--;
			continue;
		}
		particle->position.x += particle->speed.x * deltaTime;
		particle->position.y += particle->speed.y * deltaTime;
		particle++;
	}
	random_seed(seed);

	bool alive = globalTimers.gameTimed - ptr->_startTime < ptr->lifeTime;
	if((alive || ptr->lifeTime <= 0) && ptr->_particlesNeeded >= 1.0)
	{
		unsigned particlesNeeded = ptr->_particlesNeeded;
		//~ vard(ptr->_aliveParticles);
		ptr->_particlesNeeded -= particlesNeeded;
		particle = &(ptr->particles[ptr->_aliveParticles]);
		//~ vard(ptr->particles);
		for(i = 0; i < particlesNeeded; i++)
		{
			//~ printf("%d %d\n", ptr->_aliveParticles, ptr->maxParticles);
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
	particleSystemUpdate(ptr);
	imageBind(ptr->image);
	for(i = 0; i < ptr->_aliveParticles; i++)
	{
		PUSH_QUAD(particle->position.x + x, particle->position.y + y, ptr->image->w, ptr->image->h, 0, ptr->image->w * 0.5f, ptr->image->h * 0.5f);
		particle++;
	}
}


void deleteParticleSystem(ParticleSystem *ptr) {
	if(ptr)
		delete(ptr->particles);
}
