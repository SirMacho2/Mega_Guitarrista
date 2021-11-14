/**
 * Hello World Example
 * Created With Genesis-Code extension for Visual Studio Code
 * Use "Genesis Code: Compile" command to compile this program.
 **/
#include <genesis.h>
#include <string.h>
#include "musicas.h"
#include "bg.h"
#include "bt.h"
#include "controles.h"
#include "timer.h"
#include "sound.h"

#define SFX_ERROR            64

#define LARGURA 320
#define ALTURA 224
#define ALTURA_MIRA 180

Sprite *bt, *btr,*btr2,*btg,*btg2,*bty,*bty2;


char text[64];

int main()
{

    int ind_tileset = 1;
    SYS_disableInts();
	VDP_init();
	VDP_setScreenWidth320();
	VDP_setScreenHeight224();
	SYS_enableInts();

    XGM_setPCM(SFX_ERROR, error_sfx, sizeof(error_sfx));
	
	//inicializacao de sprites
	SPR_init();

    JOY_init();
    controle_iniciaVariaveis();
    JOY_setEventHandler( &controle_Handler);
	

    VDP_setPaletteColors(0, (u16*) palette_black, 64); // set all palettes to black
    //bg_A
	VDP_drawImageEx(BG_A, &bga, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, ind_tileset), 0, 0, FALSE, TRUE);
	ind_tileset += bga.tileset->numTile;
	VDP_setPalette(PAL1, bga.palette->data);


	// bts_marca
    btr2 = SPR_addSprite(&btR2 , VEMELHO_X, ALTURA_MIRA, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
	btg2 = SPR_addSprite(&btG2 , VERDE_X, ALTURA_MIRA, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
	bty2 = SPR_addSprite(&btY2 , AMARELO_X, ALTURA_MIRA, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));


 

    //adicona notas
    //seleciona palheta das notas
    VDP_setPalette(PAL2, btR.palette->data);
    
    
    
    
    Sprite *spritesNotas[100];
    s16 posicoesX[100];
    s16 posicoesY[100];
    u16 spriteIndex = 0;
    u16 nota_index = 0;
    u32 tempo = 0;
    u16 i;
    u16 primeiroSprite = 0;
    

    // VDP_setTextPalette(PAL0);

    VDP_setPaletteColor(15,RGB24_TO_VDPCOLOR(0xff0000));
    
    s16 placar = 0;

    bool negativeA = 0;
    bool negativeB = 0;
    bool negativeC = 0;

    const u32 init_time = getTick();
    bool start_music = 0;

    while(1)
    {

        if(getTick() - init_time >= 300 && !start_music)
        {
            XGM_startPlay(sonic_music);
            XGM_setLoopNumber(4);
            start_music = 1;
        }
        if (getTick() - init_time >= tempos_sonic[nota_index])
        {   
            if(notas_sonic[nota_index] & AMARELA)
            {
                posicoesX[spriteIndex] = AMARELO_X;
                posicoesY[spriteIndex] = 0;
                spritesNotas[spriteIndex++] = SPR_addSprite(&btY , AMARELO_X, 0, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
                nota_index++;
            }
            else if(notas_sonic[nota_index] & VERDE)
            {
                posicoesX[spriteIndex] = VERDE_X;
                posicoesY[spriteIndex] = 0;
                spritesNotas[spriteIndex++] = SPR_addSprite(&btG , VERDE_X, 0, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
                nota_index++;
            }
            else if(notas_sonic[nota_index] & VEMELHA)
            {
                posicoesX[spriteIndex] = VEMELHO_X;
                posicoesY[spriteIndex] = 0;
                spritesNotas[spriteIndex++] = SPR_addSprite(&btR , VEMELHO_X, 0, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
                nota_index++;
            }
        }

        
        notas_atualizaPosicao(posicoesY, velocidade_sonic, spriteIndex);

        for (i= primeiroSprite; i < spriteIndex; i++)
        {
            SPR_setPosition(spritesNotas[i], posicoesX[i], posicoesY[i]);
            if(posicoesY[i] > ALTURA)
            {
                SPR_releaseSprite(spritesNotas[i]);
                primeiroSprite++;
                if(primeiroSprite == spriteIndex)
                {
                    primeiroSprite =0;
                    spriteIndex = 0;
                }
            }
            if(posicoesY[i] > ALTURA_MIRA - 15 && posicoesY[i] < ALTURA_MIRA+37 )
            {
                if(SPR_isVisible(spritesNotas[i], 1))
                {
                    // sobe placar e deixa sprite invisivel 
                    if( posicoesX[i] == AMARELO_X && J1A && (J1ACount + 50) > (u16) getTick())
                    {
                        placar++;
                        negativeA = 1;
                        SPR_setVisibility(spritesNotas[i], HIDDEN);
                    }
                    if(posicoesX[i] == VERDE_X && J1B && (J1BCount + 50) > (u16) getTick())
                    {
                        placar++;
                        negativeB = 1;
                        SPR_setVisibility(spritesNotas[i], HIDDEN);
                    }
                    if (posicoesX[i] ==  VEMELHO_X && J1C && (J1CCount + 50) > (u16) getTick())
                    {
                        placar++;
                        negativeC = 1;
                        SPR_setVisibility(spritesNotas[i], HIDDEN);
                    }
                }
            }
            else
            {
                if (J1A && negativeA == 0)
                {
                    placar--;
                    negativeA = 1;
                }
                else if (negativeA == 1 && J1A == 0)
                {
                    negativeA = 0;
                }
                
                if (J1B && negativeB == 0)
                {
                    placar--;
                    negativeB = 1;
                }
                else if (negativeB == 1 && J1B == 0)
                {
                    negativeB = 0;
                }
                
                if (J1C && negativeC == 0)
                {
                    placar--;
                    negativeC = 1;
                }
                else if (negativeC == 1 && J1C == 0)
                {
                    negativeC = 0;
                }
            }
        }
       

        // draw screen
        tempo = tempo + velocidade_sonic;
        
        sprintf(text, "p= %d, t= %lu, tp= %lu,i= %u", placar, tempo,  tempos_sonic[nota_index], nota_index);
        VDP_drawText(text, 0,0);
        sprintf(text,  "m= %d, si=%d, ps=%d",  XGM_isPlaying(), spriteIndex, primeiroSprite);
        VDP_drawText(text, 0,1);
		SPR_update();
        SYS_doVBlankProcess();
    }
    return (0);
}
