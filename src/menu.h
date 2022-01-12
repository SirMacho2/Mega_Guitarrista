#ifndef __MENU_H__
#define __MENU_H__

#include <genesis.h>

#define NUM_MUSICAS 7
#define NUM_OPCOES_PAUSA 3
#define NUM_OPCOES_FIM 3

// define a estrutura das opções
typedef struct 
{
    s16 x;
    s16 y;
    char texto[32];
} Opcao;

typedef struct 
{
    s16 x;
    s16 y;
    char texto[32];
    const Image* image;
} OpcaoMusica;

const OpcaoMusica opcoes_musicas[NUM_MUSICAS];
const Opcao opcoes_pause[NUM_OPCOES_FIM];
const Opcao opcoes_fim[NUM_OPCOES_FIM];

void mostra_menu_inicial();
void mostra_menu_pausa();

void mostra_menu(const Opcao *opcoes, u8 tamanho);

#endif