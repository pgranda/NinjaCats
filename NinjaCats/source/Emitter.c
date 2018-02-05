#include "Emitter.h"
#include "gba_drawing.h"

extern void EmitParticle(Emitter *a_e, Particle *a_p)
{
	a_p->x = a_e->x;
	a_p->y = a_e->y;
	a_p->vx = qran_range(-55, 55);
	a_p->vy = -300 + qran_range(-50, 0);
	a_p->life = 0;
}

extern void UpdateParticle(Particle *a_p, Emitter *a_e)
{
	a_p->x = fixAdd(a_p->x, a_p->vx);
	a_p->y = fixAdd(a_p->y, a_p->vy);
	a_p->vy = fixSub(a_p->vy, 2);
	a_p->life += 16;

	if (a_p->life  > 500)
	{
		EmitParticle(a_e, a_p);
	}
}