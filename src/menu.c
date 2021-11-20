#include "menu.h"
#include "string.h"

#define CENTRO_X 20
#define CENTRO_Y 14

#define NUM_OPCOES_PAUSA 3

const Opcao opcoes_musicas = {CENTRO_X -3, CENTRO_Y, "SONIC"};

const Opcao opcoes_pause[NUM_OPCOES_PAUSA] = {
    {CENTRO_X-2, CENTRO_Y, "VOLTAR"},
    {CENTRO_X-2, CENTRO_Y+1, "SAIR"},
    {CENTRO_X-2, CENTRO_Y+2, "REINICIAR"},
};

const Opcao opcoes_menu_inicial =  {CENTRO_X -8, CENTRO_Y, "Pressione Start"};



void mostra_menu_inicial()
{
    VDP_drawText(opcoes_menu_inicial.texto, opcoes_menu_inicial.x,opcoes_menu_inicial.y);
}

void mostra_menu_pausa()
{
    int i;
    for(i = 0; i < NUM_OPCOES_PAUSA; i++)
    {
        VDP_drawText(opcoes_pause[i].texto, opcoes_pause[i].x, opcoes_pause[i].y);
    }
}