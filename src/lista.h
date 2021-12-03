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

// define a estrutura do nodo
typedef struct TempB
{
    Sprite *sprite;
    s16 x;
    s16 y;
    s16 duracao;
    struct TempB *prox;
} LBarras;



LNotas *inicio_Nota;
LBarras *inicio_Barra;

Sprite *fogoR, *fogoG, *fogoY;

void CriaLista_Nota();
u8 Insere_Nota(Sprite *sprite, s16 x, s16 y);
s16 atualizaPosicao_Nota(u8 velocidade, s16 placar);
u16 tamanhoLista_Nota(u8 lista);

void CriaLista_Barra();
u8 Insere_Barra(Sprite *sprite, s16 x, s16 y, s16 duracao);
s16 atualizaPosicao_Barra(u8 velocidade, s16 placar);

void limpa_listas();

void esconde_fogo();

#endif