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
#include "menu.h"

#define SFX_ERROR 64
#define SFX_CLICK 65

#define PLACAR_Y 7

#define ALTURA_VU ALTURA_MIRA - 100
#define ALTURA_MULTIPLICADOR ALTURA_VU - 18
#define ALTURA_GUITARRISTA ALTURA_VU - 42


Sprite *btr2, *btg2, *bty2;
Sprite *cursor;
Sprite *vu;
Sprite *mult_s;
Sprite *guitarrista;

char text[64];


enum States
{
    CREDITOS,
    MENU_INICIAL,
    MUSICA,
    MUSICAS,
    PAUSA,
    FIM_MUSICA,
};

int main()
{
    SYS_disableInts();
    VDP_init();
    VDP_setScreenWidth320();
    VDP_setScreenHeight224();
    SYS_enableInts();

    XGM_setPCM(SFX_ERROR, error_sfx, sizeof(error_sfx));
    XGM_setPCM(SFX_CLICK, click_sfx, sizeof(click_sfx));

    //inicializacao de sprites
    SPR_init();

    JOY_init();
    controle_iniciaVariaveis();
    JOY_setEventHandler(&controle_Handler);

    s16 placar = 0;
    u8 consecutivas = 0;
    u8 multiplicador = 1;

    u32 init_time = getTick();
    u32 pause_time = 0;
    u32 final_time = 0xFFFFFFFF;
    bool start_music = 0;
    bool notas_faltando = 1;
    u16 nota_index = 0;
    bool resume = 0;

    u32 creditos_time = 0xFFFFFFFF;

    enum States state = CREDITOS;
    enum States state_anterior = -1;

    Musica musica = BACK_IN_BLACK;

    const Nota *notas;
    const u32 *tempos;
    const u16 *duracoes;
    u16 tamanho_musica;
    u8 velocidade;
    const u8 *musica_xgm;
    u16 delay;
    u8 loops;

    s16 cursorY;
    s16 cursorX;

    s16 yOffsetBg = 0;

    s8 menu_movendo = 0;

    const u16 k7_colors[] = {0x02e, 0xa00, 0x06a, 0x0a0};
    u8 color_index;
    
    s16 offset_mask [28] = {19, 17, 15, 14, 12, 10, 9, 8, 7, 6, 6, 5, 5, 5, 5, 5, 5, 6, 6, 7, 8, 9, 10, 12, 14, 15, 17, 19};

    while (1)
    {
        switch (state)
        {
            case CREDITOS:
            if (state_anterior != state)
            {
                VDP_clearPlane(BG_A, TRUE);
                VDP_setVerticalScroll(BG_A, 0);
                
                state_anterior = state;
                VDP_drawImageEx(BG_A, &creditos, TILE_ATTR(PAL0, FALSE, FALSE, FALSE), 0, 0, TRUE, TRUE);
                creditos_time = getTick();
            }
            if(getTick() - creditos_time > 900)
            {
                PAL_fadeOutPalette(PAL0, 60, FALSE);
                state = MENU_INICIAL;
            }
            break;
        case MENU_INICIAL:
            if (state_anterior != state)
            {
                VDP_setTextPlane(BG_B);
                // VDP_clearPlane(BG_A, TRUE);
                // VDP_setVerticalScroll(BG_A, 0);
                VDP_resetScreen();
                VDP_setPaletteColors(0, (u16 *)palette_black, 64); // set all palettes to black
                
                VDP_setPalette(PAL3, Cursor.palette->data);

                mostra_menu_inicial();
                cursor = SPR_addSprite(&Cursor, 10 * 8, 14 * 8, TILE_ATTR(PAL3, FALSE, FALSE, FALSE));
                state_anterior = state;
                VDP_drawImageEx(BG_A, &concert, TILE_ATTR(PAL0, FALSE, FALSE, FALSE), 0, 0, FALSE, TRUE);
                PAL_fadeInPalette(PAL0, concert.palette->data, 20, FALSE);
                VDP_setPalette(PAL0, concert.palette->data);
                PAL_setColor(15, RGB24_TO_VDPCOLOR(0xffFFFF));
            }
            if (J1S)
            {
                XGM_startPlayPCM(SFX_CLICK, 1, SOUND_PCM_CH2);
                J1S = 0;
                state = MUSICAS;
                SPR_releaseSprite(cursor);
                VDP_clearTextLine(14);
            }          
            break;
        case MUSICAS:
            if (state_anterior != state)
            {
                VDP_setTextPlane(BG_B);
                yOffsetBg =0;
                VDP_setScrollingMode(HSCROLL_TILE , VSCROLL_PLANE);
                VDP_clearPlane(BG_A, TRUE);
                VDP_clearPlane(BG_B, TRUE);
                VDP_setVerticalScroll(BG_A, yOffsetBg);
                VDP_setPaletteColors(0, (u16 *)palette_black, 64); // set all palettes to black
                PAL_setColor(15, 0x00e); // set color text red
                
                cursorY = 0;
                state_anterior = state;
                J1BAIXO = 0;
                J1CIMA = 0;
                VDP_drawImageEx(BG_A, &k7, TILE_ATTR(PAL1, FALSE, FALSE, FALSE), 15, -9, FALSE, TRUE);
                VDP_drawImageEx(BG_A, &k7, TILE_ATTR(PAL2, FALSE, FALSE, FALSE), 15, 7, FALSE, TRUE);
                VDP_drawImageEx(BG_B, &sonic_cover, TILE_ATTR_FULL(PAL3, TRUE, FALSE, FALSE, k7.tileset->numTile), 1, 7, FALSE, TRUE);

                VDP_setPalette(PAL1, k7.palette->data);
                VDP_setPalette(PAL2, k7.palette->data);
                VDP_setPalette(PAL3, sonic_cover.palette->data);
                VDP_drawText(opcoes_musicas[cursorY].texto, opcoes_musicas[cursorY].x,opcoes_musicas[cursorY ].y);
                VDP_drawText(opcoes_musicas[cursorY + 1].texto, opcoes_musicas[cursorY + 1].x, 26);
                menu_movendo = 0;
                color_index = 0;
                VDP_setHorizontalScrollTile(BG_A, 0, offset_mask, 28, DMA);
            }
            if (J1BAIXO)
            {
                menu_movendo = 1;
                J1BAIXO = 0;
                if (cursorY < NUM_MUSICAS -1)
                {
                    cursorY++;
                }
                else
                {
                    cursorY = 0;
                    
                }
                if( color_index < 3)
                {
                    color_index++;
                }
                else
                {
                    color_index =0;
                }
           
                VDP_clearTextArea(opcoes_musicas[cursorY].x-2, opcoes_musicas[cursorY].y, 32, 1);
                VDP_clearTextArea(opcoes_musicas[cursorY].x-2, 26, 32, 1);
                XGM_startPlayPCM(SFX_CLICK, 1, SOUND_PCM_CH2);
                PAL_setColor(19, k7_colors[color_index]);
                VDP_clearPlane(BG_B, TRUE);
                VDP_drawImageEx(BG_B, opcoes_musicas[cursorY].image, TILE_ATTR_FULL(PAL3, TRUE, FALSE, FALSE, k7.tileset->numTile), 1, 7, FALSE, TRUE);
                VDP_setPalette(PAL3, opcoes_musicas[cursorY].image->palette->data);      
            }
            if (J1CIMA)
            {
                menu_movendo = -1;
                J1CIMA = 0;
                if (cursorY > 0)
                {
                    cursorY--;
                }
                else
                {
                    cursorY = NUM_MUSICAS -1;
                }
                if( color_index > 0)
                {
                    color_index--;
                }
                else
                {
                    color_index = 3;
                }

                VDP_clearTextArea(opcoes_musicas[cursorY].x-2, opcoes_musicas[cursorY].y, 32, 1);
                VDP_clearTextArea(opcoes_musicas[cursorY].x-2, 26, 32, 1);
                XGM_startPlayPCM(SFX_CLICK, 1, SOUND_PCM_CH2);
                PAL_setColor(19, k7_colors[color_index]);  
                VDP_clearPlane(BG_B, TRUE);
                VDP_drawImageEx(BG_B, opcoes_musicas[cursorY].image, TILE_ATTR_FULL(PAL3, TRUE, FALSE, FALSE, k7.tileset->numTile), 1, 7, FALSE, TRUE);
                VDP_setPalette(PAL3, opcoes_musicas[cursorY].image->palette->data);   
            }
            if(menu_movendo)
            {
                yOffsetBg = yOffsetBg + 8*menu_movendo;
                VDP_setVerticalScroll(BG_A, yOffsetBg);
                // VDP_setHorizontalScrollTile(BG_A, 0, offset_mask, 28, DMA);
                if (yOffsetBg == 128 || yOffsetBg == -128)
                {
                    yOffsetBg = 0;
                    menu_movendo =0;
                    VDP_drawText(opcoes_musicas[cursorY].texto, opcoes_musicas[cursorY].x,opcoes_musicas[cursorY].y);
                    if (cursorY == NUM_MUSICAS - 1)
                    {
                        VDP_drawText(opcoes_musicas[0].texto, opcoes_musicas[0].x + 1, 26);
                    }
                    else
                    {
                        VDP_drawText(opcoes_musicas[cursorY +1].texto, opcoes_musicas[cursorY +1].x + 1,26);
                    }
                    PAL_setColor(35, k7_colors[color_index]);
                }
            }

            if (J1S | J1A | J1B | J1C)
            {
                J1S = 0;
                J1A = 0;
                J1B = 0;
                J1C = 0;

                // SPR_releaseSprite(cursor);
                VDP_clearTextLine(opcoes_musicas[cursorY].y);
                VDP_clearTextArea(opcoes_musicas[cursorY].x -2, 26, 32, 1);
                VDP_setHorizontalScroll(BG_A, 0); //reset horizontal scroll
                //selec sonic
                if (cursorY == 0)
                {
                    musica = SONIC;
                }
                //select back in black
                else if (cursorY == 1)
                {
                    musica = BACK_IN_BLACK;
                }
                else if (cursorY == 2)
                {
                    musica = STREETS_OF_RAGE;
                }
                else if (cursorY == 3)
                {
                    musica = GUILE_THEME;
                }
                else if (cursorY == 4)
                {
                    musica = ZELDA;
                }
                else if (cursorY == 5)
                {
                    musica = DRACULA;
                }
                else if (cursorY == 6)
                {
                    musica = TOP_GEAR;
                }
                else if (cursorY == 7)
                {
                    musica = ALTERED;
                }
                else if (cursorY == 8)
                {
                    musica = SMOOTH;
                }
                state = MUSICA;
            }
            break;
        case MUSICA:
            if (state_anterior != state)
            {
                VDP_setTextPlane(BG_A);
                if (resume == 0)
                {
                    VDP_resetScreen();
                    // VDP_setPlaneSize(32, 64, TRUE);
                    VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);

                    VDP_drawImageEx(BG_A, &blank, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, 0), 0, 0, FALSE, TRUE);
                    VDP_drawImageEx(BG_B, &bg_musica, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, blank.tileset->numTile), 0, 0, FALSE, TRUE);
                    
                    
                    VDP_setPalette(PAL3, Mult.palette->data);
                    VDP_setPalette(PAL1, bg_musica.palette->data);
                    VDP_setPalette(PAL2, btR2.palette->data);
                    VDP_setPalette(PAL0, Fogo.palette->data);
                    VDP_setPaletteColor(15, RGB24_TO_VDPCOLOR(0xff0000));
                    VDP_setPaletteColor(0, 0);
                    VDP_setBackgroundColor(0);
                    // bts_marca
                    btr2 = SPR_addSprite(&btR2, VEMELHO_X, ALTURA_MIRA, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
                    btg2 = SPR_addSprite(&btG2, VERDE_X, ALTURA_MIRA, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
                    bty2 = SPR_addSprite(&btY2, AMARELO_X, ALTURA_MIRA, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));


                    fogoR = SPR_addSprite(&Fogo, VEMELHO_X+6, ALTURA_MIRA-26, TILE_ATTR(PAL0, FALSE, FALSE, FALSE));
                    fogoG = SPR_addSprite(&Fogo, VERDE_X+6, ALTURA_MIRA-26, TILE_ATTR(PAL0, FALSE, FALSE, FALSE));
                    fogoY = SPR_addSprite(&Fogo, AMARELO_X+6, ALTURA_MIRA-26, TILE_ATTR(PAL0, FALSE, FALSE, FALSE));

                    vu = SPR_addSprite(&Vu, 270 , ALTURA_VU, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
                    mult_s = SPR_addSprite(&Mult, 290 , ALTURA_MULTIPLICADOR, TILE_ATTR(PAL3, FALSE, FALSE, FALSE));
                    guitarrista = SPR_addSprite(&Guitar,  260 , ALTURA_GUITARRISTA, TILE_ATTR(PAL3, FALSE, FALSE, FALSE));

                    SPR_setVisibility(fogoR, HIDDEN);
                    SPR_setVisibility(fogoG, HIDDEN);
                    SPR_setVisibility(fogoY, HIDDEN);

                    SPR_setDepth(btr2, SPR_MAX_DEPTH);
                    SPR_setDepth(btg2, SPR_MAX_DEPTH);
                    SPR_setDepth(bty2, SPR_MAX_DEPTH);

                    SPR_setDepth(fogoR, SPR_MIN_DEPTH);
                    SPR_setDepth(fogoG, SPR_MIN_DEPTH);
                    SPR_setDepth(fogoY, SPR_MIN_DEPTH);

                    nota_index = 0;
                    placar = 0;
                    consecutivas = 0;
                    multiplicador = 1;

                    init_time = getTick();
                    start_music = 0;
                    pause_time = 0;
                    notas_faltando = 1;

                    switch (musica)
                    {
                    case SONIC:
                        notas = notas_sonic;
                        tempos = tempos_sonic;
                        tamanho_musica = tamanho_sonic;
                        velocidade = velocidade_sonic;
                        duracoes = duracao_sonic;
                        musica_xgm = sonic_music;
                        delay = delay_sonic;
                        loops = loops_sonic;
                        break;

                    case BACK_IN_BLACK:
                        notas = notas_bib;
                        tempos = tempos_bib;
                        tamanho_musica = tamanho_bib;
                        velocidade = velocidade_bib;
                        duracoes = duracao_bib;
                        musica_xgm = back_music;
                        delay = delay_bib;
                        loops = loops_bib;
                        break;

                    case STREETS_OF_RAGE:
                        notas = notas_sor;
                        tempos = tempos_sor;
                        tamanho_musica = tamanho_sor;
                        velocidade = velocidade_sor;
                        duracoes = duracao_sor;
                        musica_xgm = sor_music;
                        delay = delay_sor;
                        loops = loops_sor;
                        break;
                    
                    case GUILE_THEME:
                        notas = notas_guile;
                        tempos = tempos_guile;
                        tamanho_musica = tamanho_guile;
                        velocidade = velocidade_guile;
                        duracoes = duracao_guile;
                        musica_xgm = guile_music;
                        delay = delay_guile;
                        loops = loops_guile;
                        break;
                    
                    case ZELDA:
                        notas = notas_zelda;
                        tempos = tempos_zelda;
                        tamanho_musica = tamanho_zelda;
                        velocidade = velocidade_zelda;
                        duracoes = duracao_zelda;
                        musica_xgm = zelda_music;
                        delay = delay_zelda;
                        loops = loops_zelda;
                        break;
                    
                    case DRACULA:
                        notas = notas_castle;
                        tempos = tempos_castle;
                        tamanho_musica = tamanho_castle;
                        velocidade = velocidade_castle;
                        duracoes = duracao_castle;
                        musica_xgm = castle_music;
                        delay = delay_castle;
                        loops = loops_castle;
                        break;
                    
                    case TOP_GEAR:
                        notas = notas_topGear;
                        tempos = tempos_topGear;
                        tamanho_musica = tamanho_topGear;
                        velocidade = velocidade_topGear;
                        duracoes = duracao_topGear;
                        musica_xgm = topGear_music;
                        delay = delay_topGear;
                        loops = loops_topGear;
                        break;
                    
                    case ALTERED:
                        notas = notas_ateredBeast;
                        tempos = tempos_ateredBeast;
                        tamanho_musica = tamanho_ateredBeast;
                        velocidade = velocidade_ateredBeast;
                        duracoes = duracao_ateredBeast;
                        musica_xgm = ateredBeast_music;
                        delay = delay_ateredBeast;
                        loops = loops_ateredBeast;
                        break;
                    
                    case SMOOTH:
                        notas = notas_smooth;
                        tempos = tempos_smooth;
                        tamanho_musica = tamanho_smooth;
                        velocidade = velocidade_smooth;
                        duracoes = duracao_smooth;
                        musica_xgm = smooth_music;
                        delay = delay_smooth;
                        loops = loops_smooth;
                        break;

                    default:
                        break;
                    }

                    CriaLista_Nota();
                    CriaLista_Barra();
                }
                else
                {
                    resume = 0;
                }

                state_anterior = state;
            }
            if (getTick() - init_time >= delay && !start_music)
            {
                XGM_startPlay(musica_xgm);
                XGM_setLoopNumber(loops);

                start_music = 1;
            }
            while (notas_faltando && (getTick() - init_time >= tempos[nota_index]))
            {
                if (notas[nota_index] & AMARELA)
                {
                    Insere_Nota(SPR_addSprite(&btY, 0, 0, TILE_ATTR(PAL2, FALSE, FALSE, FALSE)), AMARELO_X_INICIO, ALTURA_PISTA, AMARELA);
                    if (duracoes[nota_index] > 0)
                    {
                        Insere_Barra(SPR_addSprite(&barraY, AMARELO_B_X, ALTURA_PISTA, TILE_ATTR(PAL2, FALSE, FALSE, FALSE)), AMARELO_B_X_INICIO, ALTURA_PISTA, AMARELA, duracoes[nota_index]);
                    }
                }
                else if (notas[nota_index] & VERDE)
                {
                    Insere_Nota(SPR_addSprite(&btG, VERDE_X, ALTURA_PISTA, TILE_ATTR(PAL2, FALSE, FALSE, FALSE)), VERDE_X, ALTURA_PISTA, VERDE);
                    if (duracoes[nota_index] > 0)
                    {
                        Insere_Barra(SPR_addSprite(&barraG, VERDE_B_X, ALTURA_PISTA, TILE_ATTR(PAL2, FALSE, FALSE, FALSE)), VERDE_B_X, ALTURA_PISTA, VERDE, duracoes[nota_index]);
                    }
                }
                else if (notas[nota_index] & VEMELHA)
                {
                    Insere_Nota(SPR_addSprite(&btR, VEMELHO_X, ALTURA_PISTA, TILE_ATTR(PAL2, FALSE, FALSE, FALSE)), VEMELHO_X_INICIO, ALTURA_PISTA, VEMELHA);
                    if (duracoes[nota_index] > 0)
                    {
                        Insere_Barra(SPR_addSprite(&barraR, VEMELHO_B_X, ALTURA_PISTA, TILE_ATTR(PAL2, FALSE, FALSE, FALSE)), VEMELHO_B_X_INICIO, ALTURA_PISTA, VEMELHA, duracoes[nota_index]);
                    }
                }
                if (nota_index < tamanho_musica-1)
                {
                    nota_index++;
                }
                else
                {
                    notas_faltando = 0;
                    final_time = getTick();
                    break;
                }
            }
            s8 parcial = 0;
            parcial = atualizaPosicao_Nota(velocidade, parcial);
            if (parcial > 0)
            {
                placar = placar + multiplicador;
                if (multiplicador < 4)
                {
                    consecutivas++;
                    SPR_nextFrame(vu);

                    if (consecutivas == 10)
                    {
                        multiplicador++;
                        SPR_nextFrame(mult_s);
                        SPR_nextFrame(vu);
                        consecutivas = 0;
                    }

                }
            }
            else if (parcial < 0)
            {
                SPR_setFrame(vu, 0);
                SPR_setFrame(mult_s, 0);
                consecutivas = 0;
                multiplicador = 1;
            }

            esconde_fogo();

            parcial = 0;
            parcial = atualizaPosicao_Barra(velocidade, parcial);
            placar = parcial * multiplicador + placar;

            if (J1A && (J1ACount + 50) > (u16)getTick())
            {
                placar--;
                J1A = 0;
                consecutivas = 0;
                multiplicador = 1;
                SPR_setFrame(vu, 0);
                SPR_setFrame(mult_s, 0);
                XGM_startPlayPCM(SFX_ERROR, 1, SOUND_PCM_CH2);
            }
            if (J1B && (J1BCount + 50) > (u16)getTick())
            {
                placar--;
                J1B = 0;
                consecutivas = 0;
                multiplicador = 1;
                SPR_setFrame(vu, 0);
                SPR_setFrame(mult_s, 0);
                XGM_startPlayPCM(SFX_ERROR, 1, SOUND_PCM_CH2);
            }
            if (J1C && (J1CCount + 50) > (u16)getTick())
            {
                placar--;
                J1C = 0;
                consecutivas = 0;
                multiplicador = 1;
                SPR_setFrame(vu, 0);
                SPR_setFrame(mult_s, 0);
                XGM_startPlayPCM(SFX_ERROR, 1, SOUND_PCM_CH2);
            }

            SPR_setAnim(guitarrista, multiplicador - 1);

            if (J1S)
            {
                J1S = 0;
                state = PAUSA;
                pause_time = getTick();
            }

            u16 JOY1 = JOY_readJoypad(JOY_1);
            if ((JOY1 & BUTTON_A))
            {
                SPR_setFrame(bty2, 1);
            }
            else
            {
                SPR_setFrame(bty2, 0);
            }
            if ((JOY1 & BUTTON_B))
            {
                SPR_setFrame(btg2, 1);
            }
            else
            {
                SPR_setFrame(btg2, 0);
            }
            if ((JOY1 & BUTTON_C))
            {
                SPR_setFrame(btr2, 1);
            }
            else
            {
                SPR_setFrame(btr2, 0);
            }

            // yOffsetBg -= velocidade;
            // if (yOffsetBg < 0)
            // {
            //     yOffsetBg += 512;
            // }
            // VDP_setVerticalScroll(BG_A, yOffsetBg);

            // draw screen
            sprintf(text, "%05d", placar);
            VDP_drawText(text, 34, PLACAR_Y);

            if(notas_faltando  == 0 && getTick() - final_time > 1000)
            {
                state = FIM_MUSICA;

                limpa_listas();
                SPR_releaseSprite(btr2);
                SPR_releaseSprite(btg2);
                SPR_releaseSprite(bty2);

                SPR_releaseSprite(fogoR);
                SPR_releaseSprite(fogoG);
                SPR_releaseSprite(fogoY);

                SPR_releaseSprite(vu);
                SPR_releaseSprite(mult_s);
                SPR_releaseSprite(guitarrista);
                VDP_clearTextLine(PLACAR_Y);

                XGM_stopPlay(musica_xgm);

                SPR_reset();
                MEM_pack();
            }

            break;
        case PAUSA:
            if (state_anterior != state)
            {
                VDP_setTextPlane(BG_A);
                state_anterior = state;
                mostra_menu_pausa();
                // VDP_setPalette(PAL3, Cursor.palette->data);
                cursorY = 14;
                cursorX = 17 * 8;
                cursor = SPR_addSprite(&Cursor, cursorX, cursorY * 8, TILE_ATTR(PAL3, FALSE, FALSE, FALSE));
                XGM_pausePlay(musica_xgm);
                guitarrista->timer = 0;
                fogoR->timer = 0;
                fogoG->timer = 0;
                fogoY->timer = 0;
            }
            if (J1BAIXO)
            {
                J1BAIXO = 0;
                if (cursorY < 16)
                {
                    cursorY++;
                }
                else
                {
                    cursorY = 14;
                }
                SPR_setPosition(cursor, cursorX, cursorY * 8);
                XGM_startPlayPCM(SFX_CLICK, 1, SOUND_PCM_CH2);
            }
            if (J1CIMA)
            {
                J1CIMA = 0;
                if (cursorY > 14)
                {
                    cursorY--;
                }
                else
                {
                    cursorY = 16;
                }
                SPR_setPosition(cursor, cursorX, cursorY * 8);
                XGM_startPlayPCM(SFX_CLICK, 1, SOUND_PCM_CH2);
            }
            if (J1S | J1A | J1B | J1C)
            {
                J1S = 0;
                J1A = 0;
                J1B = 0;
                J1C = 0;

                SPR_releaseSprite(cursor);
                VDP_clearTextLine(14);
                VDP_clearTextLine(15);
                VDP_clearTextLine(16);

                //voltar
                if (cursorY == 14)
                {
                    state = MUSICA;
                    resume = 1;
                    init_time = getTick() - pause_time + init_time;
                    XGM_resumePlay(musica_xgm);
                    guitarrista->timer = 1;
                    fogoR->timer = 1;
                    fogoG->timer = 1;
                    fogoY->timer = 1;
                }
                //sair
                else if (cursorY == 15)
                {
                    state = MENU_INICIAL;
                    limpa_listas();
                    SPR_releaseSprite(btr2);
                    SPR_releaseSprite(btg2);
                    SPR_releaseSprite(bty2);

                    SPR_releaseSprite(fogoR);
                    SPR_releaseSprite(fogoG);
                    SPR_releaseSprite(fogoY);

                    SPR_releaseSprite(vu);
                    SPR_releaseSprite(mult_s);
                    SPR_releaseSprite(guitarrista);
                    VDP_clearTextLine(PLACAR_Y);

                    XGM_stopPlay(musica_xgm);

                    SPR_reset();
                    MEM_pack();
                }
                //reiniciar
                else
                {
                    state = MUSICA;
                    limpa_listas();
                    SPR_releaseSprite(btr2);
                    SPR_releaseSprite(btg2);
                    SPR_releaseSprite(bty2);

                    SPR_releaseSprite(fogoR);
                    SPR_releaseSprite(fogoG);
                    SPR_releaseSprite(fogoY);

                    SPR_releaseSprite(vu);
                    SPR_releaseSprite(mult_s);
                    SPR_releaseSprite(guitarrista);
                    
                    XGM_stopPlay(musica_xgm);

                    SPR_reset();
                    MEM_pack();
                }
            }
            break;
        case FIM_MUSICA:
            if (state_anterior != state)
            {
                VDP_setTextPlane(BG_B);
                VDP_resetScreen();
                VDP_setPaletteColors(0, (u16 *)palette_black, 64); // set all palettes to black
                state_anterior = state;
                mostra_menu(opcoes_fim, NUM_OPCOES_FIM);
                VDP_setPalette(PAL3, Cursor.palette->data);
                cursorY = 14;
                cursorX = 17 * 8;
                cursor = SPR_addSprite(&Cursor, cursorX, cursorY * 8, TILE_ATTR(PAL3, FALSE, FALSE, FALSE));
                XGM_stopPlay(musica_xgm);

                VDP_drawImageEx(BG_A, &concert, TILE_ATTR(PAL0, FALSE, FALSE, FALSE), 0, 0, FALSE, TRUE);
                VDP_setPalette(PAL0, concert.palette->data);

                sprintf(text, "Seu placar foi: %05d", placar);
                VDP_drawText(text, 10, 11);
                VDP_setPaletteColor(15, RGB24_TO_VDPCOLOR(0xffFFFF));
            }
            if (J1BAIXO)
            {
                J1BAIXO = 0;
                if (cursorY < 15)
                {
                    cursorY++;
                }
                else
                {
                    cursorY = 14;
                }
                SPR_setPosition(cursor, cursorX, cursorY * 8);
                XGM_startPlayPCM(SFX_CLICK, 1, SOUND_PCM_CH2);
            }
            if (J1CIMA)
            {
                J1CIMA = 0;
                if (cursorY > 14)
                {
                    cursorY--;
                }
                else
                {
                    cursorY = 15;
                }
                SPR_setPosition(cursor, cursorX, cursorY * 8);
                XGM_startPlayPCM(SFX_CLICK, 1, SOUND_PCM_CH2);
            }
            if (J1S | J1A | J1B | J1C)
            {
                J1S = 0;
                J1A = 0;
                J1B = 0;
                J1C = 0;

                SPR_releaseSprite(cursor);
                VDP_clearTextLine(11);
                VDP_clearTextLine(14);
                VDP_clearTextLine(15);

                 if (cursorY > 14)
                 {
                    state = MUSICA;
                 }
                 else
                 {
                    state = MENU_INICIAL;
                 }
            }
            break;
        }
        SPR_update();
        // KLog_S1("sprites: \0", SPR_getNumActiveSprite());
        SYS_doVBlankProcess();
    }
    return (0);
}
