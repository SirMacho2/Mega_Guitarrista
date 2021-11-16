#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include <genesis.h>

// define a estrutura do nodo
typedef struct Temp
{
    Sprite *sprite;
    s16 x;
    s16 y;
    struct Temp *prox;
} LNotas;
// cria o inicio da lista
LNotas *inicio = NULL;
//--------------------------------------------------------
// Funcao que define a lista como vazia.
void CriaLista(LNotas *inicio)
{
    inicio = NULL;
}
//--------------------------------------------------------
// Funcao que insere um elemento do inicio da lista.
// Retorna:
//	0 - se nao ha'  memoria para inserir
//	1 - se conseguiu inserir
int Insere(Sprite *sprite, s16 x, s16 y)
{
    LNotas *p;
    p = (LNotas *)MEM_alloc(sizeof(LNotas));
    if (p == NULL)
    {
        // erro de alocação
        return 0;
    }
    p->sprite = sprite;
    p->x = x;
    p->y = y;
    p->prox = NULL;
    if (inicio == NULL)
        inicio = p;
    else
    {
        p->prox = inicio;
        inicio = p;
    }
    return 1;
}
//--------------------------------------------------------
// Funcao que remove um elemento do inicio da lista.
// Retorna:
//	0 - se a lista ja'  estava vazia
//	1 - se conseguiu remover
int RemoveDoInicio()
{
    LNotas *ptr;
    if (inicio == NULL)
        return 0;
    else
    {
        ptr = inicio;
        inicio = inicio->prox;
        SPR_releaseSprite(ptr->sprite);
        MEM_free(ptr);
        return 1;
    }
}

//--------------------------------------------------------
// Funcao que busca um elemento na lista.
// Retorna:
//	- NULL caso nao encontre
//	- ponteiro para o NODO onde esta' o dado, se conseguir encontrar
LNotas *BuscaDado(int dado)
{
    LNotas *ptr;
    if (inicio == NULL)
    {
        return NULL; // Lista Vazia
    }
    // Caso a lista nao esteja vazia
    ptr = inicio;
    while (ptr != NULL)
    {
        if (ptr->info == dado) // achou !!
            return (ptr);      // retorna um ponteiro para
                               //o inicio da lista
        else
            ptr = ptr->prox;
    }
    return NULL;
}
//--------------------------------------------------------
// Funcao que remove um elemento especificado por 'dado'
// Retorna:
//	0 - se nao conseguiu achar
//	1 - se conseguiu remover
int RemoveDado(int dado)
{
    LNotas *ptr, *antes;
    if (inicio == NULL)
    {
        return 0; // Lista vazia !!!
    }
    else
    { // Caso a lista nao esteja vazia
        ptr = inicio;
        antes = inicio;
        while (ptr != NULL)
        {
            if (ptr->info == dado) // achou !!
            {
                if (ptr == inicio) // se esta removendo o primeiro da lista
                {
                    inicio = inicio->prox;
                    SPR_releaseSprite(ptr->sprite);
                    MEM_free(ptr);
                    return 1; // removeu !!
                }
                else // esta removendo do meio da lista
                {
                    antes->prox = ptr->prox; // Refaz o encadeamento
                    SPR_releaseSprite(ptr->sprite);
                    MEM_free(ptr); // Libera a area do nodo
                    return 1;      // removeu !!
                }
            }
            else // continua procurando no prox. nodo
            {
                antes = ptr;
                ptr = ptr->prox;
            }
        }
        return 0; // Nao achou !!!
    }
}

void atualiza_posicao(LNotas *inicio, u8 velocidade)
{
    LNotas *ptr, *antes;
    if (inicio == NULL)
    {
        return 0; // Lista vazia !!!
    }
    else
    {
        ptr = inicio;
        antes = inicio;
        while (ptr != NULL)
        {
            ptr->y = ptr->y + velocidade;
            if (ptr->y > ALTURA) // passou do limite
            {
                if (ptr == inicio) // se esta removendo o primeiro da lista
                {
                    inicio = inicio->prox;
                    SPR_releaseSprite(ptr->sprite);
                    MEM_free(ptr);
                }
                else // esta removendo do meio da lista
                {
                    antes->prox = ptr->prox; // Refaz o encadeamento
                    SPR_releaseSprite(ptr->sprite);
                    MEM_free(ptr); // Libera a area do nodo
                }
            }
            if(ptr->y > ALTURA_MIRA - 15 && ptr->y < ALTURA_MIRA + 15 )
            {
                if(SPR_isVisible(ptr->sprite, 1))
                {
                    // sobe placar e deixa sprite invisivel 
                    if( posicoesX[i] == AMARELO_X && J1A )
                    {
                        placar++;
                        J1A = 0;
                        SPR_setVisibility(ptr->sprite, HIDDEN);
                    }
                    if(posicoesX[i] == VERDE_X && J1B)
                    {
                        placar++;
                        J1B = 0;
                        SPR_setVisibility(ptr->sprite, HIDDEN);
                    }
                    if (posicoesX[i] ==  VEMELHO_X && J1C )
                    {
                        placar++;
                        J1C = 0;
                        SPR_setVisibility(ptr->sprite, HIDDEN);
                    }
                }
            }

            antes = ptr;
            ptr = ptr->prox;
        }
    }
}