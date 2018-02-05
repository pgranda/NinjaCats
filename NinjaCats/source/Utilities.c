//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Project: Ninja Cats - GameBoy Advance game                                                     		//
//																										//
// Author:  Pawel Granda																				//
//																										// 
// Date Created:  11/10/2017																			//
//																										//
// Brief:   Ninja Cats is game about a cat who is also a ninja. In this short GameBoy Advance game,		//
//          you have to help our cat to reach his home. You will face many obstacles such as,			//
//          pits with skulls and snakes, or evil worms, which will try to eat your shiny fur.			// 
//																										//
//																										//
// Note:     I tried to write a code which is self documenting. Please note most of my functions,		//
//           variables are indicating their purpose.						                            //
//			 I will only comment code which contains logical statements, my approaches to solve 		//
//           the problem my own way.                                                                    //
//           Also please notice that for obvious reasons I am not commenting the gba files provided     //
//           by Jamie Stewart.                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Utilities.h"

//Variable used for scrolling the background
u8 g_backgroundOffset = 0;

//Variable used for scrolling the sky background
u8 g_skyOffset = 0;

//Logic variable to check if collision with enemy appeared
u8 g_collided = 0;

//Logic variable to check if the game is over - our Ninja Cat was killed
u8 g_gameover = 0;

//Logic variable to check if the game was won by Ninja Cat coming home
u8 g_win = 0;

//Function used to display the sky background on the screen
void initialize_sky()
{
	g_skyOffset = 0;
	REG_BG_OFS[1].x = g_skyOffset;

	//Copying background palette into memory
	memcpy(pal_bg_mem, bgPalette, bgPaletteLen);

	//Copying background tiles into memory
	u16* tileMemLoc = tile_block_address(1);
	memcpy(tileMemLoc, bgTiles, bgTilesLen);

	u16* tileMapMemLoc = tileMap_block_address(30);
	u16 offs = 0;

	//Loading first screen of background
	for (int i = 0; i < 32; ++i)
	{
		memcpy(&tileMapMemLoc[i * 32], &bgMap1[offs], sizeof(u16) * 32);

		//How big is the background map in columns
		offs += MAP_COLUMNS;
	}

	setBG_Control_Register(1, 1, 1, 0, 0, 30, 0, BG_REG_SIZE_32x32);
}

//Function used to display the background on the screen
void initialize_background()
{
	g_backgroundOffset = 0;
	REG_BG_OFS[0].x = g_backgroundOffset;

	//Copying background palette into memory
	memcpy(pal_bg_mem, bgPalette, bgPaletteLen);

	//Copying background tiles into memory
	u16* tileMemLoc = tile_block_address(0);
	memcpy(tileMemLoc, bgTiles, bgTilesLen);

	u16* tileMapMemLoc = tileMap_block_address(16);
	u16 offs = 0;

	//Loading first screen of background
	for (int i = 0; i < 32; ++i)
	{
		memcpy(&tileMapMemLoc[i * 32], &bgMap[offs], sizeof(u16) * 32);

		//How big is the background map in columns
		offs += MAP_COLUMNS;
	}

	setBG_Control_Register(0, 0, 0, 0, 0, 16, 0, BG_REG_SIZE_32x32);
}

//Function used to update the sky background - make it scrolling all the time
void update_sky()
{
	REG_BG_OFS[1].x = g_skyOffset;
	g_skyOffset += SKY_VELOCITY;
}

//Function used to update the background on which the actual gameplay happens
void update_background(s32 a_x)
{
	u16* tilemapMemLoc = tileMap_block_address(16);
	u32 columnValue = ((a_x >> 3) + SCREEN_COLUMNS) & 0x1F;			 // 0x1F = 31, where 30 is the number of columns visible on the screen
	u32 mapColValue = ((a_x >> 3) + SCREEN_COLUMNS) % MAP_COLUMNS;   // MAP_COLUMNS is the number of columns of the whole map

	for (int i = 0; i < 32; ++i)
	{
		tilemapMemLoc[columnValue] = bgMap[mapColValue];
		tilemapMemLoc[columnValue + 1] = bgMap[mapColValue + 1];
		columnValue += 32;
		mapColValue += MAP_COLUMNS;
	}
}

//Function used to initialize our Ninja Cat sprite
void initialize_sprite(NinjaCat* a_ninja)
{
	//Assigning the sprites attributes
	ObjectAttribute* spriteAttributes = &MEM_OAM[0];
	//Copying the sprites palette into the memory
	memcpy(palette_sp_block_address(0), ninjacatspritesPal, ninjacatspritesPalLen);
	//Copying the sprites tiles into the memory
	memcpy(sprite_tile_block_address(17), ninjacatspritesTiles, ninjacatspritesTilesLen);
	a_ninja->spriteAttributes = spriteAttributes;
	a_ninja->height = 32;
	a_ninja->width = 16;
	a_ninja->screenPositionX = 0;
	a_ninja->screenPositionY = GROUND - a_ninja->height;
	a_ninja->worldPositionX = 0;
	a_ninja->velocityX = 0;
	a_ninja->velocityY = 0;
	a_ninja->isFacingRight = 1;
	a_ninja->firstFrameOfAnimation = 17;
	a_ninja->animationFrame = 0;
	a_ninja->framesInAir = 0;
}

//Function used to initialize our Worm sprite
void initialize_enemy_sprite(Worm* a_enemy)
{
	//Assigning the sprites attributes
	ObjectAttribute* enemySpriteAttributes = &MEM_OAM[1];
	//Copying the sprites palette into the memory
	memcpy(palette_sp_block_address(1), wormPal, wormPalLen);
	//Copying the sprites tiles into the memory
	memcpy(sprite_tile_block_address(273), wormTiles, wormTilesLen);
	a_enemy->spriteAttributes = enemySpriteAttributes;
	a_enemy->height = 16;
	a_enemy->width = 32;
	a_enemy->screenPositionX = 170;
	a_enemy->screenPositionY = GROUND - a_enemy->height;
	a_enemy->worldPositionX = 170;
	a_enemy->velocityX = 0;
	a_enemy->velocityY = 0;
	a_enemy->isFacingRight = 1;
	a_enemy->firstFrameOfAnimation = 273;
	a_enemy->animationFrame = 0;
	a_enemy->framesInAir = 0;
}

//Function used to initialize particles for our chimney smoke
void initialize_particles(ObjectAttribute* particleOAM, Emitter* emitter, Particle particles[])
{
	//Copying the particles palette into the memory
	memcpy(palette_sp_block_address(3), smokePal, smokePalLen);
	//Copying the particles tiles into the memory
	memcpy(sprite_tile_block_address(0), smokeTiles, smokeTilesLen);

	//Setting the particles attributes
	ObjectAttribute particleAttrib;
	particleAttrib.attr0 = SetAttrib0(0, A0_MODE_REG, 0, 0, A0_4BPP, A0_SQUARE);
	particleAttrib.attr1 = SetAttrib1(0, 0, A1_SIZE_0);
	particleAttrib.attr2 = SetAttrib2(0, 0, 3);
	particleAttrib.pad = 0;

	//Setting the particles emitter position
	emitter->x = int2fix(222);
	emitter->y = int2fix(60);

	for (int i = 0; i < 32; ++i)
	{
		EmitParticle(emitter, &particles[i]);
		particleOAM[i] = particleAttrib;
	}

	obj_hide(particleOAM);
}

//Function used to update the particles position and animation
void update_particle(ObjectAttribute* particleOAM, Emitter* emitter, Particle particles[])
{
	for (int i = 0; i < 32; ++i)
	{
		UpdateParticle(&particles[i], emitter);

		SetObjectPosition(&particleOAM[i], fix2int(particles[i].x), fix2int(particles[i].y));
		u32 frameID = (1 << 9) - particles[i].life;
		frameID = frameID << 4 >> 9;
		//Cycling through the tiles to change the sprites animation
		particleOAM[i].attr2 = SetAttrib2(frameID % PARTICLES_FRAMES, 1, 3);
	}
}

//Function used to check the collision of our Ninja Cat with the background
u8 is_collided_with_background(NinjaCat* a_ninjaCat, s8 posYmodifier)
{
	//Calculating X and Y position of Ninja Cat on the whole map
	u32 ninjaCatTilePositionX = (a_ninjaCat->worldPositionX + (a_ninjaCat->width >> 1)) >> 3;
	u32 ninjaCatTilePositionY = (a_ninjaCat->screenPositionY + posYmodifier + a_ninjaCat->height) >> 3;

	//Choosing the X position from which we are going to fill the collision matrix for the screen
	u32 startX = MAX(0, ninjaCatTilePositionX - (SCREEN_COLUMNS >> 1));

	//Number of the tiles visible on the screen
	u16 collisions[SCREEN_ROWS][SCREEN_COLUMNS];

	//Fill the collision matrix with the values from the BG_Collision file
	for (int i = 0; i < SCREEN_ROWS; i++)
	{
		for (int j = 0; j < SCREEN_COLUMNS; j++)
		{
			collisions[i][j] = bgCollision[i * MAP_COLUMNS + j + startX];
		}
	}

	//If the position of the ninja cat is under the map, we don't want to proceed and return 0 which means no collision
	if (ninjaCatTilePositionY >= SCREEN_ROWS)
	{
		return 0;
	}

	u16 tile = collisions[ninjaCatTilePositionY][ninjaCatTilePositionX - startX];

	return tile == 0x0000 ? 0 : 1;
}

//Function used to calculate the collision with the enemy
void is_collided_with_enemy(NinjaCat* a_hero, Worm* a_enemy)
{
	//If we collided with the enemy before
	if (g_collided)
	{
		return;
	}
	//If horizontal position of ninja cat and enemy sprites are overlapping
	if (a_hero->worldPositionX + a_hero->width >= a_enemy->worldPositionX && a_hero->worldPositionX <= a_enemy->worldPositionX + a_enemy->width)
	{
		u8 isJumping = !is_collided_with_background(a_hero, 0);

		//If ninja cat is in the air and vertical position of ninja cat and enemy sprites are overlapping
		if (isJumping && a_hero->screenPositionY + a_hero->height >= a_enemy->screenPositionY + (a_enemy->height >> 1))
		{
			obj_hide(a_enemy->spriteAttributes);
			g_collided = 1;
		}
		//If enemy sprite hits ninja cat
		else if (!isJumping && a_hero->screenPositionY <= a_enemy->screenPositionY + a_enemy->height
			&& a_hero->screenPositionY + a_hero->height >= a_enemy->screenPositionY)
		{
			g_gameover = 1;
		}
	}
}

//Function used to move enemy sprite
void move_enemy_sprite(Worm* a_enemy)
{
	//If the worm position in the world reached the far right checkpoint
	if (a_enemy->worldPositionX == 250)
	{
		//We want him to face the left
		a_enemy->isFacingRight = 0;
		//And go backward
		a_enemy->velocityX = -ENEMY_VELOCITY;
	}
	//If the worm position in the world reached the far left checkpoint
	else if (a_enemy->worldPositionX == 170)
	{
		//We want him to face the right
		a_enemy->isFacingRight = 1;
		//And go forward
		a_enemy->velocityX = ENEMY_VELOCITY;
	}

	//Update enemy sprites position in the world according to its velocity
	a_enemy->worldPositionX += a_enemy->velocityX;
	//Update enemy sprites position in the screen according to its world position and background offset
	a_enemy->screenPositionX = a_enemy->worldPositionX - g_backgroundOffset;

	//We are setting the enemy sprite attributes if he is visible on the screen
	a_enemy->spriteAttributes->attr0 = SetAttrib0(a_enemy->screenPositionY, A0_MODE_REG, 0, 0, A0_8BPP, A0_WIDE);
	a_enemy->spriteAttributes->attr1 = SetAttrib1(a_enemy->screenPositionX, a_enemy->isFacingRight ? 1 : 0, A1_SIZE_2);
	a_enemy->spriteAttributes->attr2 = SetAttrib2(273, 1, 1);

}

//Function used to hide and unhide enemy sprite accordingly to his position on the screen 
void wrap_enemy_sprite(Worm* a_enemy)
{
	//If the enemy is dead we go out of the function
	if (g_collided)
	{
		return;
	}

	if (g_backgroundOffset >= 250)
	{
		g_collided = 1;
		obj_hide(a_enemy->spriteAttributes);
		return;
	}

	//If the enemy sprites position on the screen is less than 0 meaning he is out of bounds
	if (a_enemy->screenPositionX <= 0)
	{
		//We want to hide the sprite
		obj_hide(a_enemy->spriteAttributes);
		//But we still want to move him, as he comes back
		move_enemy_sprite(a_enemy);
	}
	else
	{
		//We want to make him visible again
		obj_unhide(a_enemy->spriteAttributes, A0_MODE_REG);
		//We also want to move him
		move_enemy_sprite(a_enemy);
		//And animate him
		animate_enemy_sprite(a_enemy);
	}
}

//Function used to animate our Ninja Cat sprite
void animate_sprite(NinjaCat* a_sprite)
{
	ObjectAttribute* spriteAttributes = a_sprite->spriteAttributes;

	u8 isJumping = !is_collided_with_background(a_sprite, 0);

	//Animate sprite while jumping
	if (isJumping)
	{
		a_sprite->firstFrameOfAnimation = 81;
		a_sprite->animationFrame = a_sprite->velocityY > 0 ? 1 : 0;
	}
	else
	{
		//If the sprite is moving then play Walk Animation
		if (a_sprite->velocityX != 0)
		{
			a_sprite->firstFrameOfAnimation = 145;
			a_sprite->animationFrame = (++a_sprite->animationFrame) % 8;
		}
		//If the sprite is standing still play Idle Animation
		else
		{
			a_sprite->firstFrameOfAnimation = 17;
			a_sprite->animationFrame = (++a_sprite->animationFrame) % 4;
		}
	}

	//Shift the sprite in the spritesheet to actually play the desired animation
	spriteAttributes->attr2 = a_sprite->firstFrameOfAnimation + (a_sprite->animationFrame * 16);
}

//Function used to animate enemy sprite
void animate_enemy_sprite(Worm* a_enemy)
{
	ObjectAttribute* spriteAttributes = a_enemy->spriteAttributes;

	//If the sprite is moving then play Walk Animation
	if (a_enemy->velocityX != 0)
	{
		a_enemy->firstFrameOfAnimation = 273;
		a_enemy->animationFrame = (++a_enemy->animationFrame) % 2;
	}

	//Shift the sprite in the spritesheet to actually play the desired animation
	a_enemy->spriteAttributes->attr2 = a_enemy->firstFrameOfAnimation + (a_enemy->animationFrame * 16);
}

//Function used to initalize everything needed for gameplay
void load_game(NinjaCat* a_ninjaCat, Worm* a_enemy, ObjectAttribute* particleOAM, Emitter* emitter, Particle particles[])
{
	initialize_sprite(a_ninjaCat);
	initialize_enemy_sprite(a_enemy);
	initialize_background();
	initialize_sky();
	initialize_particles(particleOAM, emitter, particles);
}

//Function used to handle gameplay
void handle_gameplay(NinjaCat* a_ninjaCat, Worm* a_enemy)
{
	update_sky();
	move_sprite(a_ninjaCat);
	animate_sprite(a_ninjaCat);
	wrap_enemy_sprite(a_enemy);
}

//Function used move sprite on X axis
void move_sprite_horizontally(NinjaCat* a_ninjaCat)
{
	//If we fall down to the pit full of skull and snakes
	if (a_ninjaCat->screenPositionY >= 160)
	{
		//Well we die, don't we? Although I think that Cat would probably win with the snake
		g_gameover = 1;
		return;
	}

	//If we press the LEFT button, face the sprite to the left side and change its velocity so it moves backward
	if (keyDown(LEFT))
	{
		a_ninjaCat->isFacingRight = 0;
		a_ninjaCat->velocityX = -NINJA_CAT_VELOCITY;
	}
	//If we press the RIGHT button, face the sprite to the right side and change its velocity so it moves forward
	else if (keyDown(RIGHT))
	{
		a_ninjaCat->isFacingRight = 1;
		a_ninjaCat->velocityX = NINJA_CAT_VELOCITY;
	}
	//If we don't press any key the sprite stands still
	else
	{
		a_ninjaCat->velocityX = 0;
	}

	//Update sprites position on the screen according to its velocity
	a_ninjaCat->screenPositionX += a_ninjaCat->velocityX;

	//If our Ninja Cat didn't reach the center of the screen, we want him to be able to change position
	//But when he does that we want to keep him on the center of the screen, I guess some old games did that
	if (a_ninjaCat->worldPositionX < MAP_WIDTH - (SCREEN_CENTER + a_ninjaCat->width))
	{
		a_ninjaCat->screenPositionX = MIN(SCREEN_CENTER, a_ninjaCat->screenPositionX);
	}
	//If we are at the end of the level we want to be able to move our Ninja Cat to the house
	else
	{
		a_ninjaCat->screenPositionX = MIN(DOOR_SCREEN_POSITION, a_ninjaCat->screenPositionX);

		//Our Ninja Cat reaches his house doors
		if (a_ninjaCat->screenPositionX == DOOR_SCREEN_POSITION)
		{
			//We hide the sprite
			obj_hide(a_ninjaCat->spriteAttributes);
			g_win = 1;
			return;
		}
	}

	a_ninjaCat->screenPositionX = MAX(0, a_ninjaCat->screenPositionX);

	//We update the Ninja Cats position in the world according to his velocity
	if (a_ninjaCat->screenPositionX != 0 && a_ninjaCat->worldPositionX != MAP_WIDTH)
	{
		a_ninjaCat->worldPositionX += a_ninjaCat->velocityX;
	}

	//If the position of the sprite is bigger than the center of the screen and the sprite moves right
	if (a_ninjaCat->worldPositionX > SCREEN_CENTER && a_ninjaCat->screenPositionX == SCREEN_CENTER && a_ninjaCat->velocityX > 0
		&& a_ninjaCat->worldPositionX < MAP_WIDTH - (SCREEN_CENTER + a_ninjaCat->width))
	{
		//Update the background in memory
		update_background(a_ninjaCat->worldPositionX - SCREEN_CENTER);
		//Move the background
		g_backgroundOffset = a_ninjaCat->worldPositionX - SCREEN_CENTER;
		REG_BG_OFS[0].x = g_backgroundOffset;
	}
}

//Function used to move our sprite in Y axis
void move_sprite_vertically(NinjaCat* a_ninjaCat)
{
	//Value which is going to indicate if the sprite is jumping
	u8 isJumping = !is_collided_with_background(a_ninjaCat, 0);

	//If we press the key to Jump and we are not jumping already
	if (keyDown(A) && !isJumping)
	{
		a_ninjaCat->velocityY = JUMP_POWER;
		a_ninjaCat->framesInAir = 0;
	}
	else if (isJumping && is_collided_with_background(a_ninjaCat, -TILE_SIZE))
	{
		a_ninjaCat->velocityY = -(TILE_SIZE >> 1);
	}
	//If we are jumping
	else if (isJumping)
	{
		a_ninjaCat->velocityY = JUMP_POWER + (GRAVITY * a_ninjaCat->framesInAir);
		a_ninjaCat->velocityY = MIN(-JUMP_POWER >> 1, a_ninjaCat->velocityY);
		a_ninjaCat->framesInAir++;
	}
	else
	{
		a_ninjaCat->velocityY = 0;
	}

	a_ninjaCat->screenPositionY += a_ninjaCat->velocityY;
}

//Function used to finally combine our two move functions into one and set sprites attributes
void move_sprite(NinjaCat* a_ninjaCat)
{
	move_sprite_horizontally(a_ninjaCat);

	if (g_win)
	{
		return;
	}

	move_sprite_vertically(a_ninjaCat);

	a_ninjaCat->spriteAttributes->attr0 = SetAttrib0(a_ninjaCat->screenPositionY, A0_MODE_REG, 0, 0, A0_8BPP, A0_TALL);
	a_ninjaCat->spriteAttributes->attr1 = SetAttrib1(a_ninjaCat->screenPositionX, a_ninjaCat->isFacingRight ? 0 : 1, A1_SIZE_2);
}

//Function used to handle gameover state and loading new game, while resetting some of the global variables
void handle_gameover(NinjaCat* a_ninjaCat, Worm* a_enemy, ObjectAttribute* particleOAM, Emitter* emitter, Particle particles[])
{
	if (!g_gameover)
	{
		return;
	}

	load_game(a_ninjaCat, a_enemy, particleOAM, &emitter, particles);
	g_collided = 0;
	g_gameover = 0;
}

//Function used to handle the win game state, and show the smoke coming out of the chimney
void handle_win(ObjectAttribute* particleOAM, Emitter* emitter, Particle particles[])
{
	if (!g_win)
	{
		return;
	}

	update_particle(particleOAM, emitter, particles);
}