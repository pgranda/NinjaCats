#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "gba_math.h"

typedef struct _Particle
{
	fixed x, y;
	fixed vx, vy;
	u32 life;
}Particle;

extern void InitParticle(Particle *a_p);

#endif