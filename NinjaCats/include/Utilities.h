#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include "gba.h"
#include "Intellisense.h"
#include <string.h>
#include "BG_Externs.h"
#include "smoke.h"
#include "ninjacatsprites.h"
#include "worm.h"
#include "Emitter.h"

#define GROUND 138
#define GRAVITY 2
#define JUMP_POWER -10

#define SCREEN_WIDTH 240
#define SCREEN_CENTER 112
#define SCREEN_COLUMNS 30
#define SCREEN_ROWS 20

#define MAP_COLUMNS 120
#define MAP_WIDTH 960

#define SKY_VELOCITY 3
#define ENEMY_VELOCITY 2
#define NINJA_CAT_VELOCITY 2

#define DOOR_SCREEN_POSITION 180
#define TILE_SIZE 8

#define PARTICLES_FRAMES 16

#define MIN(x,y) (x > y ? y : x)
#define MAX(x,y) (x < y ? y : x)

extern u8 g_backgroundOffset;
extern u8 g_skyOffset;
extern u8 g_collided;
extern u8 g_gameover;
extern u8 g_win;

typedef struct
{
	ObjectAttribute* spriteAttributes;
	s16 screenPositionX;
	s16 screenPositionY;
	s16 worldPositionX;
	s16 velocityX;
	s16 velocityY;
	s16 height;
	s16 width;
	s16 isFacingRight;
	s16 firstFrameOfAnimation;
	s16 animationFrame;
	s16 framesInAir;
}NinjaCat;

typedef struct
{
	ObjectAttribute* spriteAttributes;
	s16 screenPositionX;
	s16 screenPositionY;
	s16 worldPositionX;
	s16 velocityX;
	s16 velocityY;
	s16 height;
	s16 width;
	s16 isFacingRight;
	s16 firstFrameOfAnimation;
	s16 animationFrame;
	s16 framesInAir;
}Worm;


extern void initialize_sky();
extern void  initialize_background();

extern void update_sky();
extern void update_background(s32 a_x);

extern void initialize_sprite(NinjaCat* a_ninja);
extern void initialize_enemy_sprite(Worm* a_enemy);

extern void initialize_particles(ObjectAttribute* particleOAM, Emitter* emitter, Particle particles[]);
extern void update_particle(ObjectAttribute* particleOAM, Emitter* emitter, Particle particles[]);

extern u8 is_collided_with_background(NinjaCat* a_ninjaCat, s8 posYmodifier);
extern void is_collided_with_enemy(NinjaCat* a_hero, Worm* a_enemy);

extern void move_sprite_horizontally(NinjaCat* a_ninjaCat);
extern void move_sprite_vertically(NinjaCat* a_ninjaCat);
extern void move_sprite(NinjaCat* a_sprite);
extern void move_enemy_sprite(Worm* a_sprite);

extern void wrap_enemy_sprite(Worm* a_enemy);

extern void animate_sprite(NinjaCat* a_sprite);
extern void animate_enemy_sprite(Worm* a_sprite);

extern void load_game(NinjaCat* a_ninjaCat, Worm* a_enemy, ObjectAttribute* particleOAM, Emitter* emitter, Particle particles[]);
extern void handle_gameplay(NinjaCat* a_ninjaCat, Worm* a_enemy);

extern void handle_gameover(NinjaCat* a_ninjaCat, Worm* a_enemy, ObjectAttribute* particleOAM, Emitter* emitter, Particle particles[]);
extern void handle_win(ObjectAttribute* particleOAM, Emitter* emitter, Particle particles[]);

#endif