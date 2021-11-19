#include "menu.h"
#include "string.h"

#define CENTRO_X 20
#define CENTRO_Y 14

const Opcao opcoes_musicas = {CENTRO_X -3, CENTRO_Y, "SONIC"};

const Opcao opcoes_pause[3] = {
    {CENTRO_X, CENTRO_Y, "VOLTAR"},
    {CENTRO_X, CENTRO_Y+1, "SAIR"},
    {CENTRO_X, CENTRO_Y+2, "REINICIAR"},
};

const Opcao opcoes_menu_inicial =  {CENTRO_X -8, CENTRO_Y, "Pressione Start"};



void mostra_menu_inicial()
{
    VDP_drawText(opcoes_menu_inicial.texto, opcoes_menu_inicial.x,opcoes_menu_inicial.y);
}