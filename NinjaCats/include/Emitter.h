#ifndef _EMITTER_H_
#define _EMITTER_H_

#include "Particle.h"

typedef struct _Emitter
{
	fixed x, y;
}Emitter;

extern void EmitParticle(Emitter *a_e,  Particle *a_p);
extern void UpdateParticle(Particle *a_p, Emitter *a_e);
#endif