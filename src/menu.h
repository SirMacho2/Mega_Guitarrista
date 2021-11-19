#ifndef __MENU_H__
#define __MENU_H__

#include <genesis.h>

#define NUM_MUSICAS 1

// define a estrutura das opções
typedef struct 
{
    s16 x;
    s16 y;
    char texto[32];
} Opcao;


// Opcao opcoes_musicas[NUM_MUSICAS];
// Opcao opcoes_pause[3];

void mostra_menu_inicial();


#endif