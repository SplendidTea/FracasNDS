#include <nds.h>
#include <stdlib.h>
#include <gl2d.h>
 
#define NUM_STARS 40
#define UPLEFT (KEY_UP | KEY_LEFT)
#define UPRIGHT (KEY_UP | KEY_RIGHT)
#define DOWNLEFT (KEY_DOWN | KEY_LEFT)
#define DOWNRIGHT (KEY_DOWN | KEY_RIGHT)
 
typedef struct 
{
	int x;
	int y;
	int speed;
	unsigned short color;
 
}Star;
 
// Function Prototypes
void setStar(Star* star, int xSpeed, int ySpeed, int xStart, int yStart);
void MoveStar(Star* star);
void ClearScreen(void);
void InitStars(void);
void DrawStar(Star* star);
void EraseStar(Star* star);
 
Star stars[NUM_STARS];
int lastKey;
 
void MoveStar(Star* star)
{
	if((DOWNRIGHT & lastKey) == DOWNRIGHT)
		setStar(star, star->speed, star->speed, rand() % SCREEN_WIDTH, 0);
	else if((DOWNLEFT & lastKey) == DOWNLEFT)
		setStar(star, -star->speed, +star->speed, rand() % SCREEN_WIDTH, 0);
	else if((UPLEFT & lastKey) == UPLEFT)
		setStar(star, -star->speed, -star->speed, rand() % SCREEN_WIDTH, SCREEN_HEIGHT);
	else if((UPRIGHT & lastKey) == UPRIGHT)
		setStar(star, star->speed, -star->speed, rand() % SCREEN_WIDTH, SCREEN_HEIGHT);
	else if(KEY_RIGHT & lastKey)
		setStar(star, star->speed, 0, 0, rand() % SCREEN_HEIGHT);
	else if(KEY_LEFT & lastKey)
		setStar(star, -star->speed, 0, SCREEN_WIDTH, rand() % SCREEN_HEIGHT);
	else if(KEY_UP & lastKey)
		setStar(star, 0, -star->speed, rand() % SCREEN_WIDTH, SCREEN_HEIGHT);
	else if(KEY_DOWN & lastKey)
		setStar(star, 0, star->speed, rand() % SCREEN_WIDTH, 0);
	else
		setStar(star, star->speed, 0, 0, rand() % SCREEN_HEIGHT);
}
 
void setStar(Star* star, int xSpeed, int ySpeed, int xStart, int yStart)
{
	star->x += xSpeed;
	star->y += ySpeed;
 
	if(star->y < 0 || star->y > SCREEN_HEIGHT)
	{
		star->x = xStart;
		star->y = yStart;
		star->speed = rand() % 4 + 1;	
	}
	else if(star->x < 0 || star->x > SCREEN_WIDTH)
	{
		star->x = xStart;
		star->y = yStart;
		star->speed = rand() % 4 + 1;	
	}
}
 
void ClearScreen(void)
{
     int i;    
     for(i = 0; i < 256 * 192; i++)
           VRAM_A[i] = RGB15(0,0,0);
}
 
void InitStars(void)
{
	int i;
	for(i = 0; i < NUM_STARS; i++)
	{
		stars[i].color = rand(); // both color & rand() are a 16bit value
		stars[i].x = rand() % 256;
		stars[i].y = rand() % 192;
		stars[i].speed = rand() % 4 + 1;
	}
}
void DrawStar(Star* star)
{
	glBoxFilled(star->x,star->y,star->x+1,star->y+1,star->color);
}
 
void EraseStar(Star* star)
{
	glBoxFilled(star->x,star->y,star->x+1,star->y+1,RGB15(0,0,0));
}