#include "lista.h"
#include "controles.h"
#include "timer.h"
#include "musicas.h"
#include "bt.h"

#define TEMPO_FOGO 110
#define DIFY intToFix16(ALTURA_MIRA - ALTURA_PISTA)
#define DIFX fix16Sub(AMARELO_X_INICIO, intToFix16(AMARELO_X))
#define SCALE fix16Div(DIFX, DIFY) 


static u32 tempoFogoR = 0, tempoFogoG = 0, tempoFogoY = 0;

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
u8 Insere_Nota(Sprite *sprite, s16 x, s16 y, Nota tipo)
{
    LNotas *p;
    // MEM_pack();
    p = (LNotas *)MEM_alloc(sizeof(LNotas));
    if (p == NULL || sprite == NULL)
    {
        // erro de alocação
        KLog("error in allocation");
        return 0;
    }
    
    p->sprite = sprite;
    p->x = x;
    p->y = y;
    p->tipo =  tipo;
    p->prox = NULL;

    SPR_setDepth(sprite, SPR_MIN_DEPTH);

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
u8 Insere_Barra(Sprite *sprite, s16 x, s16 y, Nota tipo, s16 duracao)
{
    LBarras *p;
    
    // MEM_pack();
    p = (LBarras *)MEM_alloc(sizeof(LBarras));
    if (p == NULL || sprite == NULL)
    {
        // erro de alocação
        KLog("error in allocation");
        return 0;
    }
    
    p->sprite = sprite;
    p->x = x;
    p->y = y;
    p->tipo = tipo;
    p->duracao = duracao;
    p->prox = NULL;

    SPR_setDepth(sprite, SPR_MIN_DEPTH);

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
//numero de notas acertadas, ou -1 se passou uma nota sem acertar;
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

        f16 v = intToFix16(velocidade);
        while (ptr != NULL)
        {
            ptr->y = ptr->y + velocidade;
            if (ptr->y >= ALTURA) // passou do limite
            {
                if(SPR_isVisible(ptr->sprite, 1))
                {
                    placar --;
                }
                if (ptr == inicio_Nota) // se esta removendo o primeiro da lista
                {
                    inicio_Nota = NULL;
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
            else
            {
                if(ptr->tipo == AMARELA)
                {
                    ptr->x = ptr->x - fix16Mul(SCALE, v);
                    SPR_setPosition(ptr->sprite,  fix16ToInt(ptr->x), ptr->y);
                }
                else if(ptr->tipo == VEMELHA)
                {
                    ptr->x = ptr->x + fix16Mul(SCALE, v);
                    SPR_setPosition(ptr->sprite,  fix16ToInt(ptr->x), ptr->y);
                }
                else
                {
                    SPR_setPosition(ptr->sprite,  ptr->x, ptr->y);
                } 
                if((ptr->y - ALTURA_PISTA) % 15 == 0 && ptr->y < 210)
                {
                    SPR_setFrame(ptr->sprite, (ptr->y - ALTURA_PISTA)/15);
                }
                if(ptr->y > ALTURA_MIRA - 15 && ptr->y < ALTURA_MIRA + 15 )
                {
                    if(SPR_isVisible(ptr->sprite, 1))
                    {
                        // sobe placar e deixa sprite invisivel 
                        if( ptr->tipo == AMARELA && J1A )
                        {
                            placar++;
                            J1A = 0;
                            SPR_setVisibility(ptr->sprite, HIDDEN);
                            SPR_setFrame(fogoY,0);
                            SPR_setVisibility(fogoY, VISIBLE);
                            tempoFogoY = getTick();
                        }
                        if(ptr->tipo == VERDE && J1B)
                        {
                            placar++;
                            J1B = 0;
                            SPR_setVisibility(ptr->sprite, HIDDEN);
                            SPR_setFrame(fogoG,0);
                            SPR_setVisibility(fogoG, VISIBLE);
                            tempoFogoG = getTick();
                        }
                        if (ptr->tipo ==  VEMELHA && J1C )
                        {
                            placar++;
                            J1C = 0;
                            SPR_setVisibility(ptr->sprite, HIDDEN);
                            SPR_setFrame(fogoR,0);
                            SPR_setVisibility(fogoR, VISIBLE);
                            tempoFogoR = getTick();
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

//--------------------------------------------------------
// Funcao que atualiza a posição das barras e detecta se o usuario aperto o botão na hora correta.
// Retorna:
//pontuação referente as barras;
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
        f16 v = intToFix16(velocidade);
        while (ptr != NULL)
        {
            ptr->y = ptr->y + velocidade;

            if (ptr->y >= ALTURA) // passou do limite
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
            else
            {
                if(ptr->tipo == AMARELA)
                {
                    ptr->x = ptr->x - fix16Mul(SCALE, v);
                    SPR_setPosition(ptr->sprite,  fix16ToInt(ptr->x), ptr->y);
                }
                else if(ptr->tipo == VEMELHA)
                {
                    ptr->x = ptr->x + fix16Mul(SCALE, v);
                    SPR_setPosition(ptr->sprite,  fix16ToInt(ptr->x), ptr->y);
                }
                else
                {
                    SPR_setPosition(ptr->sprite,  ptr->x, ptr->y);
                }             
                if(ptr->duracao > 0 && ptr->y == 8 + ALTURA_PISTA)
                {
                    if(ptr->tipo == AMARELA)
                    {
                        Insere_Barra(SPR_addSprite(&barraY , ptr->x, ALTURA_PISTA, TILE_ATTR(PAL2, FALSE, FALSE, FALSE)), AMARELO_B_X_INICIO, ALTURA_PISTA, AMARELA, ptr->duracao-45);
                    }
                    if(ptr->tipo == VERDE)
                    {
                        Insere_Barra(SPR_addSprite(&barraG , ptr->x, ALTURA_PISTA, TILE_ATTR(PAL2, FALSE, FALSE, FALSE)), ptr->x, ALTURA_PISTA, VERDE, ptr->duracao-45);
                    }
                    if(ptr->tipo == VEMELHA)
                    {
                        Insere_Barra(SPR_addSprite(&barraR , ptr->x, ALTURA_PISTA, TILE_ATTR(PAL2, FALSE, FALSE, FALSE)), VEMELHO_B_X_INICIO, ALTURA_PISTA, VEMELHA, ptr->duracao-45);
                    }
                }
                if(ptr->y > ALTURA_MIRA - 5 && ptr->y < ALTURA_MIRA + 7 )
                {
                    u16 JOY1 = JOY_readJoypad(JOY_1);
                    if(SPR_isVisible(ptr->sprite, 1))
                    {
                        // sobe placar e deixa sprite invisivel                         200
                        if( ptr->tipo == AMARELA)
                        {
                            if ((JOY1 & BUTTON_A))
                            {
                                placar++;
                                SPR_setVisibility(ptr->sprite, HIDDEN);
                                SPR_setFrame(fogoY,0);
                                SPR_setVisibility(fogoY, VISIBLE);
                                tempoFogoY = getTick();
                            }
                            
                        }
                        if(ptr->tipo == VERDE )
                        {
                            if ((JOY1 & BUTTON_B))
                            {
                                placar++;
                                SPR_setVisibility(ptr->sprite, HIDDEN);
                                SPR_setFrame(fogoG,0);
                                SPR_setVisibility(fogoG, VISIBLE);
                                tempoFogoG = getTick();
                            }
                        }
                        if (ptr->tipo ==  VEMELHA)
                        {
                            if ((JOY1 & BUTTON_C))
                            {
                                placar++;
                                SPR_setVisibility(ptr->sprite, HIDDEN);
                                SPR_setFrame(fogoR,0);
                                SPR_setVisibility(fogoR, VISIBLE);    
                                tempoFogoR = getTick();
                            }
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


void limpa_listas()
{
    LNotas *ptr;
    LBarras *ptr2;
    for (ptr = inicio_Nota; ptr != NULL; ptr = inicio_Nota)
    {
        inicio_Nota = inicio_Nota->prox;
        SPR_releaseSprite(ptr->sprite);
        MEM_free(ptr);
    }
    
    for (ptr2 = inicio_Barra; ptr2 != NULL; ptr2 = inicio_Barra)
    {
        inicio_Barra = inicio_Barra->prox;
        SPR_releaseSprite(ptr2->sprite);
        MEM_free(ptr2);
    }
}

void esconde_fogo()
{
    u32 tempo =  getTick();
    if (tempo - tempoFogoR >= TEMPO_FOGO)
    {
        SPR_setVisibility(fogoR, HIDDEN);
    }
    if (tempo - tempoFogoG >= TEMPO_FOGO)
    {
        SPR_setVisibility(fogoG, HIDDEN);
    }
    if (tempo - tempoFogoY >= TEMPO_FOGO)
    {
        SPR_setVisibility(fogoY, HIDDEN);
    }
}