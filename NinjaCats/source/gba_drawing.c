#include "gba_drawing.h"

u16 SetColor(u8 a_red, u8 a_green, u8 a_blue)
{
	return (a_red & 0x1F) | (a_green & 0x1F) << 5 | (a_blue & 0x1F) << 10;
}

void DrawRect(s32 a_x, s32 a_y, s32 a_w, s32 a_h, u16 a_color)
{
	for (int y = 0; y < a_h; ++y)
	{
		for (int x = 0; x < a_w; x++)
		{
			SCREENBUFFER[(a_y + y)*SCREEN_W + a_x + x] = a_color;
		}
	}
}

void DrawRectOutline(s32 a_x, s32 a_y, s32 a_w, s32 a_h, u16 a_color)
{
	for (int y = 0; y < a_h; y++)
	{
		for (int x = 0; x < a_w; x++)
		{
			if (y == 0 || y == a_h - 1 || x == 0 || x == a_w - 1)
			{
				SCREENBUFFER[(a_y + y)*SCREEN_W + a_x + x] = a_color;
			}
		}
	}
}

void DrawHorizontalLine(s32 a_x, s32 a_y, s32 a_w, u16 a_color)
{
	for (int x = 0; x < a_w; ++x)
	{
		SCREENBUFFER[a_y*SCREEN_W + a_x + x] = a_color;
	}
}

void DrawVetricalLine(s32 a_x, s32 a_y, s32 a_h, u16 a_color)
{
	for (int y = 0; y <= a_h; ++y)
	{
		SCREENBUFFER[(a_y + y)*SCREEN_W + a_x] = a_color;
	}
}

void DrawLine(s32 a_x, s32 a_y, s32 a_x2, s32 a_y2, u16 a_color)
{
	s32 w = a_x2 - a_x;
	s32 h = a_y2 - a_y;
	s32 dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0;
	if (w < 0) dx1 = -1; else if (w > 0) dx1 = 1;
	if (h < 0) dy1 = -1; else if (h > 0) dy1 = 1;
	if (w < 0) dx2 = -1; else if (w > 0) dx2 = 1;
	s32 longest = abs(w);
	s32 shortest = abs(h);
	if (!(longest > shortest)) {
		longest = abs(h);
		shortest = abs(w);
		if (h < 0) dy2 = -1; else if (h > 0) dy2 = 1;
		dx2 = 0;
	}
	s32 numerator = longest >> 1;
	for (s32 i = 0; i <= longest; i++) {
		SCREENBUFFER[a_y * SCREEN_W + a_x] = a_color;
		numerator += shortest;
		if (!(numerator < longest)) {
			numerator -= longest;
			a_x += dx1;
			a_y += dy1;
		}
		else {
			a_x += dx2;
			a_y += dy2;
		}
	}
}

void DrawSlashLine(s32 a_x, s32 a_y, s32 a_l, u16 a_color)
{
	for (int y = 0; y < a_l; ++y)
	{
		for (int x = 0; x < a_l; ++x)
		{
			if (x == 0 && y == a_l)
			{
				SCREENBUFFER[(a_y + y)*SCREEN_W + a_x + x] = a_color;
			}
			else if (y + x == a_l)
			{
				SCREENBUFFER[(a_y + y)*SCREEN_W + a_x + x] = a_color;
			}
		}
	}
}

void DrawBackSlashLine(s32 a_x, s32 a_y, s32 a_l, u16 a_color)
{
	for (int y = 0; y < a_l; ++y)
	{
		for (int x = 0; x < a_l; ++x)
		{
			if (x == 0 && y == 0)
			{
				SCREENBUFFER[(a_y + y)*SCREEN_W + a_x + x] = a_color;
			}
			else if (x == y)
			{
				SCREENBUFFER[(a_y + y)*SCREEN_W + a_x + x] = a_color;
			}
		}
	}
}

void DrawRectTriangleLeftBottom(s32 a_x, s32 a_y, s32 a_l, u16 a_color)
{
	for (int y = 0; y < a_l; ++y)
	{
		for (int x = 0; x < a_l; ++x)
		{
			if (x < y)
			{
				SCREENBUFFER[(a_y + y)*SCREEN_W + a_x + x] = a_color;
			}
		}
	}
}

void DrawRectTriangleRightTop(s32 a_x, s32 a_y, s32 a_l, u16 a_color)
{
	for (int y = 0; y < a_l; ++y)
	{
		for (int x = 0; x < a_l; ++x)
		{
			if (x > y)
			{
				SCREENBUFFER[(a_y + y)*SCREEN_W + a_x + x] = a_color;
			}
		}
	}
}

void DrawTriangle(s32 a_x, s32 a_y, s32 a_h, u16 a_color)
{
	bool finished = false;
	int center = a_h / 2;
	if (a_h % 2 == 0)
	{
		center--;
	}

	for (int y = 0; y < a_h; y++)
	{
		for (int x = 0; x < a_h; x++)
		{

			if (x >= center - y && x <= center + y)
			{
				SCREENBUFFER[(a_y + y)*SCREEN_W + a_x + x] = a_color;
				if (x == 0)
				{
					finished = true;
				}
			}
		}
		if (finished)
		{
			return;
		}
	}
}

void DrawUpsideDownTriangle(s32 a_x, s32 a_y, s32 a_h, u16 a_color)
{
	int center = a_h / 2;
	if (a_h % 2 == 0)
	{
		center--;
	}

	for (int y = a_h - 1; y >= 0; y--)
	{
		for (int x = 0; x < a_h; x++)
		{
			if (x >= y && x <= center * 2 - y)
			{
				SCREENBUFFER[(a_y + y)*SCREEN_W + a_x + x] = a_color;
			}
		}
	}
}

//Bresenham's Algorithm for circle drawing routine
void DrawCircle(s32 a_x, s32 a_y, s32 a_r, u16 a_color)
{
	int x = a_r - 1;
	int y = 0;
	int dx = 1;
	int dy = 1;
	int radiusError = dx - (a_r << 1);

	while (x >= y)
	{
		SCREENBUFFER[a_x + x + (a_y + y)*SCREEN_W] = a_color;
		SCREENBUFFER[a_x + y + (a_y + x)*SCREEN_W] = a_color;
		SCREENBUFFER[a_x - y + (a_y + x)*SCREEN_W] = a_color;
		SCREENBUFFER[a_x - x + (a_y + y)*SCREEN_W] = a_color;
		SCREENBUFFER[a_x - x + (a_y - y)*SCREEN_W] = a_color;
		SCREENBUFFER[a_x - y + (a_y - x)*SCREEN_W] = a_color;
		SCREENBUFFER[a_x + y + (a_y - x)*SCREEN_W] = a_color;
		SCREENBUFFER[a_x + x + (a_y - y)*SCREEN_W] = a_color;

		if (radiusError <= 0)
		{
			y++;
			radiusError += dy;
			dy += 2;
		}
		if (radiusError > 0)
		{
			x--;
			dx += 2;
			radiusError += (-a_r << 1) + dx;
		}
	}
}

void DrawFullCircle(s32 a_x, s32 a_y, s32 a_r, u16 a_color)
{
	int x = a_r - 1;
	int y = 0;
	int dx = 1;
	int dy = 1;
	int radiusError = dx - (a_r << 1);

	while (x >= y)
	{
		for (int i = a_x - x; i <= a_x + x; i++)
		{
			SCREENBUFFER[i + (a_y + y)*SCREEN_W] = a_color;
			SCREENBUFFER[i + (a_y - y)*SCREEN_W] = a_color;
		}

		for (int i = a_x - y; i <= a_x + y; i++)
		{
			SCREENBUFFER[i + (a_y + x)*SCREEN_W] = a_color;
			SCREENBUFFER[i + (a_y - x)*SCREEN_W] = a_color;
		}

		if (radiusError <= 0)
		{
			y++;
			radiusError += dy;
			dy += 2;
		}
		if (radiusError > 0)
		{
			x--;
			dx += 2;
			radiusError += (-a_r << 1) + dx;
		}
	}
}

void PlayDemo()
{
	DrawRect(10, 10, 20, 20, SetColor(0x1F, 0x00, 0x00));
	DrawRect(31, 10, 20, 20, SetColor(0x00, 0x1F, 0x00));
	DrawRect(52, 10, 20, 20, SetColor(0x00, 0x00, 0x1F));

	DrawRectOutline(73, 10, 20, 20, SetColor(0x1F, 0x00, 0x00));
	DrawRectOutline(94, 10, 20, 20, SetColor(0x00, 0x1F, 0x00));
	DrawRectOutline(115, 10, 20, 20, SetColor(0x00, 0x00, 0x1F));

	DrawCircle(145, 20, 10, SetColor(0x1F, 0x00, 0x00));
	DrawCircle(166, 20, 10, SetColor(0x00, 0x1F, 0x00));
	DrawCircle(187, 20, 10, SetColor(0x00, 0x00, 0x1F));

	DrawHorizontalLine(10, 32, 20, SetColor(0x1F, 0x00, 0x00));
	DrawHorizontalLine(10, 38, 20, SetColor(0x00, 0x1F, 0x00));
	DrawHorizontalLine(10, 45, 20, SetColor(0x00, 0x00, 0x1F));

	DrawVetricalLine(31, 32, 13, SetColor(0x1F, 0x00, 0x00));
	DrawVetricalLine(41, 32, 13, SetColor(0x00, 0x1F, 0x00));
	DrawVetricalLine(50, 32, 13, SetColor(0x00, 0x00, 0x1F));

	DrawSlashLine(57, 32, 15, SetColor(0x1F, 0x00, 0x00));
	DrawSlashLine(65, 32, 15, SetColor(0x00, 0x1F, 0x00));
	DrawSlashLine(72, 32, 15, SetColor(0x00, 0x00, 0x1F));

	DrawBackSlashLine(90, 32, 15, SetColor(0x1F, 0x00, 0x00));
	DrawBackSlashLine(98, 32, 15, SetColor(0x00, 0x1F, 0x00));
	DrawBackSlashLine(106, 32, 15, SetColor(0x00, 0x00, 0x1F));

	DrawRectTriangleLeftBottom(10, 50, 20, SetColor(0x1F, 0x00, 0x00));
	DrawRectTriangleLeftBottom(31, 50, 20, SetColor(0x00, 0x1F, 0x00));
	DrawRectTriangleLeftBottom(52, 50, 20, SetColor(0x00, 0x00, 0x1F));

	DrawRectTriangleRightTop(72, 51, 20, SetColor(0x1F, 0x00, 0x00));
	DrawRectTriangleRightTop(93, 51, 20, SetColor(0x00, 0x1F, 0x00));
	DrawRectTriangleRightTop(114, 51, 20, SetColor(0x00, 0x00, 0x1F));

	DrawFullCircle(145, 60, 10, SetColor(0x1F, 0x00, 0x00));
	DrawFullCircle(166, 60, 10, SetColor(0x00, 0x1F, 0x00));
	DrawFullCircle(187, 60, 10, SetColor(0x00, 0x00, 0x1F));

	DrawTriangle(10, 75, 20, SetColor(0x1F, 0x00, 0x00));
	DrawTriangle(31, 75, 20, SetColor(0x00, 0x1F, 0x00));
	DrawTriangle(52, 75, 20, SetColor(0x00, 0x00, 0x1F));

	DrawUpsideDownTriangle(73, 75, 20, SetColor(0x1F, 0x00, 0x00));
	DrawUpsideDownTriangle(94, 75, 20, SetColor(0x00, 0x1F, 0x00));
	DrawUpsideDownTriangle(115, 75, 20, SetColor(0x00, 0x00, 0x1F));
}