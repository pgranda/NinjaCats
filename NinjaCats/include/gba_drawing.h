#ifndef __GBA_DRAWING_H__
#define __GBA_DRAWING_H__
#include "gba_types.h"
#include "gba_reg.h"
#include "gba_mathUtil.h"

#define SCREENBUFFER ((vu16*)(VRAM))
#define SCREEN_W 240
#define SCREEN_H 160

u16 SetColor(u8 a_red, u8 a_green, u8 a_blue);
void DrawRect(s32 a_x, s32 a_y, s32 a_w, s32 a_h, u16 a_color);
void DrawRectOutline(s32 a_x, s32 a_y, s32 a_w, s32 a_h, u16 a_color);
void DrawHorizontalLine(s32 a_x, s32 a_y, s32 a_w, u16 a_color);
void DrawVetricalLine(s32 a_x, s32 a_y, s32 a_h, u16 a_color);
void DrawLine(s32 a_x, s32 a_y, s32 a_x2, s32 a_y2, u16 a_color);
void DrawSlashLine(s32 a_x, s32 a_y, s32 a_l, u16 a_color);
void DrawBackSlashLine(s32 a_x, s32 a_y, s32 a_l, u16 a_color);
void DrawRectTriangleLeftBottom(s32 a_x, s32 a_y, s32 a_l, u16 a_color);
void DrawRectTriangleRightTop(s32 a_x, s32 a_y, s32 a_l, u16 a_color);
void DrawTriangle(s32 a_x, s32 a_y, s32 a_h, u16 a_color);
void DrawUpsideDownTriangle(s32 a_x, s32 a_y, s32 a_h, u16 a_color);
//Bresenham's Algorithm for circle drawing routine
void DrawCircle(s32 a_x, s32 a_y, s32 a_r, u16 a_color);
void DrawFullCircle(s32 a_x, s32 a_y, s32 a_r, u16 a_color);
void PlayDemo();

#endif //__GBA_DRAWING_H__