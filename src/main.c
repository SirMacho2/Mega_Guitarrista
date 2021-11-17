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
#include <timer.h>
#include "sound.h"
#include "lista.h"

#define SFX_ERROR 64

Sprite *btr2,*btg2,*bty2;

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

    // barra_r = SPR_addSprite(&barraR , VEMELHO_X +10, ALTURA_MIRA -7, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
 

    //adicona notas
    //seleciona palheta das notas
    VDP_setPalette(PAL2, btR.palette->data);
    

    u16 nota_index = 0;
    
    VDP_setPaletteColor(15,RGB24_TO_VDPCOLOR(0xff0000));
    
    s16 placar = 0;

    const u32 init_time = getTick();
    bool start_music = 0;

    CriaLista_Nota();
    CriaLista_Barra();
    u8 ret = 0;
    while(1)
    {
        
        if(getTick() - init_time >= 300 && !start_music)
        {
            XGM_startPlay(sonic_music);
            XGM_setLoopNumber(4);
            start_music = 1;
        }
        while(getTick() - init_time >= tempos_sonic[nota_index])
        {   
            if(notas_sonic[nota_index] & AMARELA)
            {
                ret = Insere_Nota(SPR_addSprite(&btY , AMARELO_X, 0, TILE_ATTR(PAL2, FALSE, FALSE, FALSE)), AMARELO_X, 0);
                if(duracao_sonic[nota_index] > 0)
                {
                    Insere_Barra(SPR_addSprite(&barraY , AMARELO_X+10, 0, TILE_ATTR(PAL2, FALSE, FALSE, FALSE)), AMARELO_X+10, 0, duracao_sonic[nota_index]);
                }
            }
            else if(notas_sonic[nota_index] & VERDE)
            {
                ret = Insere_Nota(SPR_addSprite(&btG , VERDE_X, 0, TILE_ATTR(PAL2, FALSE, FALSE, FALSE)), VERDE_X, 0);
                if(duracao_sonic[nota_index] > 0)
                {
                    Insere_Barra(SPR_addSprite(&barraG , VERDE_X+10, 0, TILE_ATTR(PAL2, FALSE, FALSE, FALSE)), VERDE_X+10, 0, duracao_sonic[nota_index]);
                }
            }
            else if(notas_sonic[nota_index] & VEMELHA)
            {
                ret = Insere_Nota(SPR_addSprite(&btR , VEMELHO_X, 0, TILE_ATTR(PAL2, FALSE, FALSE, FALSE)), VEMELHO_X, 0);
                if(duracao_sonic[nota_index] > 0)
                {
                    Insere_Barra(SPR_addSprite(&barraR , VEMELHO_X+10, 0, TILE_ATTR(PAL2, FALSE, FALSE, FALSE)), VEMELHO_X+10, 0, duracao_sonic[nota_index]);
                }
            }
            nota_index++;
        }

        placar =  atualizaPosicao_Barra(velocidade_sonic, placar);
        placar =  atualizaPosicao_Nota(velocidade_sonic, placar);

        if (J1A && (J1ACount + 50) > (u16) getTick())                   
        {
            placar--;
            J1A = 0;
            XGM_startPlayPCM(SFX_ERROR, 1, SOUND_PCM_CH2);
        }
        if (J1B && (J1BCount + 50) > (u16) getTick())
        {
            placar--;
            J1B = 0;
            XGM_startPlayPCM(SFX_ERROR, 1, SOUND_PCM_CH2);
        } 
        if (J1C && (J1CCount + 50) > (u16) getTick())
        {
            placar--;
            J1C = 0;
            XGM_startPlayPCM(SFX_ERROR, 1, SOUND_PCM_CH2);
        }
            

       

        // draw screen
        u16 tamanho = tamanhoLista_Nota(1);
        sprintf(text, "p = %3d, tam = %2d", placar,  tamanho);
        ret = 0;
        // VDP_clearTextLine(0);
        VDP_drawText(text, 0,0);
        
		SPR_update();
        SYS_doVBlankProcess();
    }
    return (0);
}
