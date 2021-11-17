#include "lista.h"
#include "controles.h"
#include "timer.h"
#include "musicas.h"
#include "bt.h"
//--------------------------------------------------------
// Funcao que define a lista como vazia.
void CriaLista_Nota()
{
    inicio_Nota = NULL;
}
//--------------------------------------------------------
// Funcao que insere um elemento do inicio da lista.
// Retorna:
//	0 - se nao ha'  memoria para inserir
//	1 - se conseguiu inserir
u8 Insere_Nota(Sprite *sprite, s16 x, s16 y)
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

    if (inicio_Nota == NULL)
        inicio_Nota = p;
    else
    {
        p->prox = inicio_Nota;
        inicio_Nota = p;
    }
    return 1;
}



//--------------------------------------------------------
// Funcao que define a lista como vazia.
void CriaLista_Barra()
{
    inicio_Barra = NULL;
}
//--------------------------------------------------------
// Funcao que insere um elemento do inicio da lista.
// Retorna:
//	0 - se nao ha'  memoria para inserir
//	1 - se conseguiu inserir
u8 Insere_Barra(Sprite *sprite, s16 x, s16 y, s16 duracao)
{
    LBarras *p;
    

    p = (LBarras *)MEM_alloc(sizeof(LBarras));
    if (p == NULL)
    {
        // erro de alocação
        return 0;
    }
    
    p->sprite = sprite;
    p->x = x;
    p->y = y;
    p->duracao = duracao;
    p->prox = NULL;

    if (inicio_Barra == NULL)
        inicio_Barra = p;
    else
    {
        LBarras *atual = inicio_Barra;
        while ( atual->prox != NULL)
        {
            atual = atual->prox;
        }
        atual->prox = p;
    }
    return 1;
}

//--------------------------------------------------------
// Funcao diz o tamanho da lista.
// Retorna:
//	tamanho da lista
u16 tamanhoLista_Nota(u8 lista)
{
    u16 tamanho = 0;
    if ( lista == 0)
    {
        if (inicio_Nota == NULL)
            return 0;
        LNotas *ptr = inicio_Nota;
        for (tamanho = 0; ptr != NULL; tamanho++)
        {
            ptr = ptr->prox;
        }
    }
    else if(lista == 1)
    {
        if (inicio_Barra == NULL)
            return 0;
        LBarras *ptr = inicio_Barra;
        for (tamanho = 0; ptr != NULL; tamanho++)
        {
            ptr = ptr->prox;
        }
    }
    
    return tamanho;

}


//--------------------------------------------------------
// Funcao que atualiza a posição das notas e detecta se o usuario aperto o botão na hora correta.
// Retorna:
//placar
s16 atualizaPosicao_Nota(u8 velocidade, s16 placar)
{
    LNotas *ptr, *antes;
    if (inicio_Nota == NULL)
    {
        return placar; // Lista vazia !!!
    }
    else
    {
        ptr = inicio_Nota;
        antes = inicio_Nota;
        while (ptr != NULL)
        {
            ptr->y = ptr->y + velocidade;
            SPR_setPosition(ptr->sprite,  ptr->x, ptr->y);
            if (ptr->y > ALTURA) // passou do limite
            {
                if (ptr == inicio_Nota) // se esta removendo o primeiro da lista
                {
                    // inicio = inicio->prox;
                    inicio_Nota = NULL;
                    SPR_releaseSprite(ptr->sprite);
                    MEM_free(ptr);
                    return placar;
                }
                else // esta removendo do meio da lista
                {
                    antes->prox = ptr->prox; // Refaz o encadeamento
                    SPR_releaseSprite(ptr->sprite);
                    MEM_free(ptr); // Libera a area do nodo
                    // ptr = antes->prox;
                    break;
                }
            }
            if(ptr->y > ALTURA_MIRA - 15 && ptr->y < ALTURA_MIRA + 15 )
            {
                if(SPR_isVisible(ptr->sprite, 1))
                {
                    // sobe placar e deixa sprite invisivel 
                    if( ptr->x == AMARELO_X && J1A )
                    {
                        placar++;
                        J1A = 0;
                        SPR_setVisibility(ptr->sprite, HIDDEN);
                    }
                    if(ptr->x == VERDE_X && J1B)
                    {
                        placar++;
                        J1B = 0;
                        SPR_setVisibility(ptr->sprite, HIDDEN);
                    }
                    if (ptr->x ==  VEMELHO_X && J1C )
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
    return placar;
}


s16 atualizaPosicao_Barra(u8 velocidade, s16 placar)
{
    LBarras *ptr, *antes;
    if (inicio_Barra == NULL)
    {
        return placar; // Lista vazia !!!
    }
    else
    {
        ptr = inicio_Barra;
        antes = inicio_Barra;
        while (ptr != NULL)
        {
            ptr->y = ptr->y + velocidade;
            SPR_setPosition(ptr->sprite,  ptr->x, ptr->y); 
            if(ptr->duracao > 0 && ptr->y == 8)
            {
                if(ptr->x == AMARELO_X + 10)
                {
                    Insere_Barra(SPR_addSprite(&barraY , ptr->x, 0, TILE_ATTR(PAL2, FALSE, FALSE, FALSE)), ptr->x, 0, ptr->duracao-80);
                }
                if(ptr->x == VERDE_X + 10)
                {
                    Insere_Barra(SPR_addSprite(&barraG , ptr->x, 0, TILE_ATTR(PAL2, FALSE, FALSE, FALSE)), ptr->x, 0, ptr->duracao-80);
                }
                if(ptr->x == VEMELHO_X + 10)
                {
                    Insere_Barra(SPR_addSprite(&barraR , ptr->x, 0, TILE_ATTR(PAL2, FALSE, FALSE, FALSE)), ptr->x, 0, ptr->duracao-80);
                }
            }
            if (ptr->y > ALTURA) // passou do limite
            {
                if (ptr == inicio_Barra) // se esta removendo o primeiro da lista
                {
                    inicio_Barra = inicio_Barra->prox;
                    SPR_releaseSprite(ptr->sprite);
                    MEM_free(ptr);
                }
                else // esta removendo do meio da lista
                {
                    antes->prox = ptr->prox; // Refaz o encadeamento
                    SPR_releaseSprite(ptr->sprite);
                    MEM_free(ptr); // Libera a area do nodo
                    ptr = antes->prox;
                }
            }
            if(ptr->y > ALTURA_MIRA - 15 && ptr->y < ALTURA_MIRA + 15 )
            {
                u16 JOY1 = JOY_readJoypad(JOY_1);
                if(SPR_isVisible(ptr->sprite, 1))
                {
                    // sobe placar e deixa sprite invisivel                         200
                    if( ptr->x == AMARELO_X +10)
                    {
                        if ((JOY1 & BUTTON_A))
                        {
                            placar++;
                            SPR_setVisibility(ptr->sprite, HIDDEN);
                        }
                        
                    }
                    if(ptr->x == VERDE_X +10 )
                    {
                        if ((JOY1 & BUTTON_B))
                        {
                            placar++;
                            SPR_setVisibility(ptr->sprite, HIDDEN);
                        }
                    }
                    if (ptr->x ==  VEMELHO_X +10)
                    {
                         if ((JOY1 & BUTTON_C))
                        {
                            placar++;
                            SPR_setVisibility(ptr->sprite, HIDDEN);
                        }
                    }
                }
            }

            antes = ptr;
            ptr = ptr->prox;
        }
    }
    return placar;
}

        // if(spriteBarraRIndex > 0)
        // {
        //     if(posicoesYBarraR[spriteBarraRIndex-1] == 8  && durationBarraR[spriteBarraRIndex -1 ] > 0) 
        //     {
        //         posicoesXBarraR[spriteBarraRIndex] = VEMELHO_X+10;
        //         posicoesYBarraR[spriteBarraRIndex] = 0;
        //         durationBarraR[spriteBarraRIndex] = durationBarraR[spriteBarraRIndex -1] - 32;
        //         spritesBarrasR[spriteBarraRIndex++] = SPR_addSprite(&barraR , VEMELHO_X+10,0, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
        //     }
        // }
        

        // for(i = primeiraBarraR; i < spriteBarraRIndex; i++ )
        // {
        //     SPR_setPosition(spritesBarrasR[i],  posicoesXBarraR[i], posicoesYBarraR[i]);
        //     if(posicoesYBarraR[i] > ALTURA)
        //     {
        //         SPR_releaseSprite(spritesBarrasR[i]);
        //         primeiraBarraR++;
        //         if(primeiraBarraR == spriteBarraRIndex)
        //         {
        //             primeiraBarraR =0;
        //             spriteBarraRIndex = 0;
        //         }
        //     }
        // }