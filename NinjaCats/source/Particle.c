#include "Particle.h"

extern void InitParticle(Particle *a_p)
{
	a_p->x = 0;
	a_p->y = 0;
	a_p->vx = 0;
	a_p->vy = 0;
	a_p->life = 0;
}