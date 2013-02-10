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

#include "cheetah.h"
#include "macros.h"
#include "render.h"
#include "vertex.h"
#include "random.h"

void imageBind(Image * image);

void newParticleSystem(ParticleSystem *ptr, int maxParticles, const char *options) {
	NEEDED_INIT_VOID;
	Particle *particles = NULL;
	new0(particles, Particle, maxParticles);
	ptr->_lasttime = globalTimers.timed;
	ptr->startSpeed = 1.0;
	ptr->directionVariation = 0.5;
}

void particleSystemUpdate(ParticleSystem *ptr) {
	unsigned	i;
	unsigned	seed = random_get_seed();
	Particle	*particle = ptr->particles;
	float		deltaTime;
	deltaTime = globalTimers.gameTimed - ptr->_lasttime;
	ptr->_lasttime = globalTimers.gameTimed;
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
	if(globalTimers.gameTimed - ptr->_startTime < ptr->lifeTime || ptr->lifeTime < 0)
	{
		unsigned particlesNeeded = ptr->emissionRate * deltaTime;
		particle = &ptr->particles[ptr->_aliveParticles];
		for(i = 0; i < particlesNeeded; i++)
		{
			if(ptr->_aliveParticles >= ptr->maxParticles) break;
			particle->age = 0.0f;
			float angle = ptr->direction + randf2(rand128()) * ptr->directionVariation;
			particle->speed.x = cosf(angle);
			particle->speed.y = sinf(angle);
			float startSpeed = ptr->startSpeed + randf2(rand128()) * ptr->startSpeedVariation;
			particle->speed.x *= startSpeed;
			particle->speed.y *= startSpeed;
			particle++;
			ptr->_aliveParticles++;
		}
	}
}
void particleSystemDraw(ParticleSystem *ptr, float x, float y) {
	unsigned	i;
	Particle	*particle = ptr->particles;
	if(globalTimers.timed - ptr->_lasttime > 1.0/60.0)
	{
		particleSystemUpdate(ptr);
	}
	imageBind(ptr->image);
	glPushMatrix();
	glTranslatef(x, y, 0);
	for(i = 0; i < ptr->_aliveParticles; i++)
	{
		PUSH_QUAD(particle->position.x, particle->position.y, ptr->image->w, ptr->image->h, 0, ptr->image->w * 0.5f, ptr->image->h * 0.5f);
		particle++;
	}
	glPopMatrix();
}


void deleteParticleSystem(ParticleSystem *ptr) {
	if(ptr)
		delete(ptr->particles);
}
