#ifndef __MENU_H__
#define __MENU_H__

#include <genesis.h>

#define NUM_MUSICAS 3
#define NUM_OPCOES_PAUSA 3

// define a estrutura das opções
typedef struct 
{
    s16 x;
    s16 y;
    char texto[32];
} Opcao;

const Opcao opcoes_musicas[NUM_MUSICAS];
const Opcao opcoes_pause[NUM_OPCOES_PAUSA];

void mostra_menu_inicial();
void mostra_menu_pausa();

void mostra_menu(Opcao *opcoes, u8 tamanho);

#endif