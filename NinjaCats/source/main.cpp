extern "C"
{
#include "Utilities.h"
}

int main()
{
	REG_DISPLAYCONTROL = VIDEOMODE_0 | BGMODE_0 | BGMODE_1 | ENABLE_OBJECTS | MAPPINGMODE_1D;
	oam_init(obj_buffer, 128);

	NinjaCat ninjaCat;
	Worm worm;
	ObjectAttribute* particleOAM = &MEM_OAM[2];
	Emitter emitter;
	Particle particles[32];

	load_game(&ninjaCat, &worm, particleOAM, &emitter, particles);

	while (1)
	{
		is_collided_with_enemy(&ninjaCat, &worm);

		handle_gameover(&ninjaCat, &worm, particleOAM, &emitter, particles);

		for (int i = 0; i < 3; i++)
		{
			PollKeys();
			vSync();
		}

		handle_gameplay(&ninjaCat, &worm);
		handle_win(particleOAM, &emitter, particles);
	}

	return 0;
}