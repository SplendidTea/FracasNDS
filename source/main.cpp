#include <stdio.h>
#include <nds.h>
#include <ctime>
#include <gl2d.h>
#include "starfield.h"
#include "ship.h"
#include "barrel.h"
#include "fracas.h"
//Default barrel values
class BarrelInfo{
    public:
        float posX=95;
        float posY=77;
        float barrelSpeed=-2.125;
};

int main(void)
{
    //defualt player values
    float playerPosY=90;
    float playerPosX=77;
    float playerVertSpeed=0;
    float playerHoriSpeed=0;

    //Ship and Barrel stuff
    glImage Ship[1];
    glImage Barrel[1];
    glImage Fracas[1];
    
    irqInit();
	irqEnable(IRQ_VBLANK);
	//videoSetMode(MODE_FB0);
	vramSetBankA(VRAM_A_LCD);
    ClearScreen();
	InitStars();

    //Important stuff, allows sprites to function, without setting the VBank to VRAM_A_TEXTURE, it will cause sprites to appear white
    videoSetMode(MODE_5_3D);
    vramSetBankA(VRAM_A_TEXTURE);
    vramSetBankE(VRAM_E_TEX_PALETTE);
    consoleDemoInit();
    //Set the 3d engine to compute 2d stuff
    glScreen2D();
    //set random seed
    srand(time(NULL));

//set sprite params (if this is done wrong it'll cause some sprite errors)
    glLoadTileSet(Ship,32,32,32,32,GL_RGB16,TEXTURE_SIZE_32,TEXTURE_SIZE_32,GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT,shipPalLen,(u16*)shipPal,(u8*)shipBitmap);
    glLoadTileSet(Barrel,32,32,32,32,GL_RGB16,TEXTURE_SIZE_32,TEXTURE_SIZE_32,GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT,barrelPalLen,(u16*)barrelPal,(u8*)barrelBitmap);
    glLoadTileSet(Fracas,128,128,128,128,GL_RGB16,TEXTURE_SIZE_128,TEXTURE_SIZE_128,GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT,fracasPalLen,(u16*)fracasPal,(u8*)fracasBitmap);

    //Honestly this could be a random number generator but then this turns into an insane bullet hell, this ain't touhou
    int barrelNum= 8;
    BarrelInfo rdmBarrel[barrelNum];
    bool dead=false;
    //inital starting positions for the barrels
    for(int i=0; i<barrelNum; i++)
    {
        rdmBarrel[i].posX=79*i+256;
        rdmBarrel[i].posY=rand()%(256-32)+32;
    }
    while(1)
    {
        scanKeys();
        glBegin2D();
       if(keysDown())
			lastKey = keysHeld();
 
		swiWaitForVBlank();

//Draw stars using starfield.h 
		for(int i = 0; i < NUM_STARS; i++)
		{
			EraseStar(&stars[i]);
			MoveStar(&stars[i]);
			DrawStar(&stars[i]);
		}

//draw Player
    if(!dead){
        glSprite(playerPosX,playerPosY,GL_FLIP_NONE,Ship);
    }
    else if(dead){
        glSprite(64,64,GL_FLIP_NONE,Fracas);   
    }
//draw barrels and assign a random y pos
        for(int i=0; i<barrelNum; i++)
        {
        glSprite(rdmBarrel[i].posX,rdmBarrel[i].posY,GL_FLIP_NONE,Barrel);
        rdmBarrel[i].posX+=rdmBarrel[i].barrelSpeed;
        if(rdmBarrel[i].posX<0-32)
        {
            rdmBarrel[i].posX=256;
            rdmBarrel[i].posY=rand()%(256-32)+32;
            
        }
    //collision detection with barrels and borders
        if(((playerPosY>rdmBarrel[i].posY-16&&playerPosY<rdmBarrel[i].posY+16)&&(((playerPosX+32)>rdmBarrel[i].posX+3)&&((playerPosX+12)<rdmBarrel[i].posX+27)))||(playerPosX<-32||playerPosX>(256)||playerPosY<-32||playerPosY>(200)))
        {
            //this should, key word, should reset everything to a starting state, or at the very least something similar to a starting state
            consoleClear();
            dead=true;
           for(int i=0; i<barrelNum; i++)
        {
        rdmBarrel[i].posX=79*i+256;
        rdmBarrel[i].posY=rand()%(256-32)+32;
        } 
            barrelNum=0;
            playerPosY=77;
            playerPosX=90;
            fprintf(stderr, "Game over man, game over!\n Restart?\nPress Select");
        }
        }
        if(keysHeld()&KEY_SELECT)
        {//resets the dead
            consoleClear();
            if(dead){
                dead=false;
                barrelNum=8;             
            }
        }
        //Player Movement in X and Y
        if(keysHeld()&KEY_UP){
            playerVertSpeed=-2.6;
        }
        else if(keysHeld()&KEY_DOWN)
        {
            playerVertSpeed=2.6;
        }
        if(keysHeld()&KEY_LEFT)
        {
            playerHoriSpeed=-2.6;
        }
        else if(keysHeld()&KEY_RIGHT)
        {
            playerHoriSpeed=2.6;
        }
        playerPosY+=playerVertSpeed;
        playerPosX+=playerHoriSpeed;
        //on key release make player stationary
        playerVertSpeed=0;
        playerHoriSpeed=0;

        //end 2d 
        glEnd2D();
        glFlush(0);
        swiWaitForVBlank();
    }

    return 0;
}