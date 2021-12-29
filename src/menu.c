#include "menu.h"
#include "string.h"

#define CENTRO_X 20
#define CENTRO_Y 14

const Opcao opcoes_pause[NUM_OPCOES_PAUSA] = {
    {CENTRO_X-2, CENTRO_Y, "VOLTAR"},
    {CENTRO_X-2, CENTRO_Y+1, "SAIR"},
    {CENTRO_X-2, CENTRO_Y+2, "REINICIAR"},
};

const Opcao opcoes_musicas[NUM_MUSICAS] = {
    {CENTRO_X-2, CENTRO_Y-4, "Green Hill Zone"},
    {CENTRO_X-1, CENTRO_Y-4, "Back in Black"},
    {CENTRO_X-2, CENTRO_Y-4, "Streets of Rage"},
    {CENTRO_X, CENTRO_Y-4, "Guile theme"},
    {CENTRO_X-2, CENTRO_Y-4, "Zelda-overworld"},
    {CENTRO_X-2, CENTRO_Y-4, "Dracula's castle"},
    {CENTRO_X-2, CENTRO_Y-4, "Top Gear"},
};

const Opcao opcoes_menu_inicial =  {CENTRO_X -8, CENTRO_Y, "Pressione Start"};

const Opcao opcoes_fim[NUM_OPCOES_FIM] = {
    {CENTRO_X-2, CENTRO_Y, "SAIR"},
    {CENTRO_X-2, CENTRO_Y+1, "REINICIAR"},
};

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

void mostra_menu(const Opcao *opcoes, u8 tamanho)
{
    u8 i;
    for(i = 0; i < tamanho; i++)
    {
        VDP_drawText(opcoes[i].texto, opcoes[i].x, opcoes[i].y);
    }
}