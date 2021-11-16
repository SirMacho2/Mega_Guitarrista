#ifndef __LISTA_H__
#define __LISTA_H__

#include <genesis.h>
#include <memory.h>


#define LARGURA 320
#define ALTURA 224
#define ALTURA_MIRA 180

// define a estrutura do nodo
typedef struct Temp
{
    Sprite *sprite;
    s16 x;
    s16 y;
    struct Temp *prox;
} LNotas;

LNotas *inicio;

void CriaLista();
u8 Insere(Sprite *sprite, s16 x, s16 y);
s16 atualiza_posicao(u8 velocidade, s16 placar);
u16 tamanho_lista();

#endif