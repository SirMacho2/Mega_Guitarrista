#ifndef __MUSICAS_H__
#define __MUSICAS_H__

#include <genesis.h>

#define VEMELHO_X 205
#define VERDE_X 145
#define AMARELO_X 85

#define TAMANHO_SONIC 434

typedef enum // Cria uma ENUM para indetificar uma nota
{
	AMARELA = 1,
	VERDE = 2,
    VEMELHA = 4
} Nota; // Define uma nota


typedef struct // Cria uma STRUCT para armazenar os dados de uma nota
{
	Nota *notas; // notas que seram tocadas
	s16 *tempos; // intervalo de tempo para proxima nota
	u16 tamanho;
	u8 velocidade;
}Musica;


const u32 tempos_sonic[434];
const Nota notas_sonic[434];
const u8 velocidade_sonic;


void musica_caregamento(Musica *musica);
void notas_atualizaPosicao(s16 *posicao, u8 velocidade, u8 tamanho);
Nota notas_atualizar(Musica m, u16 tempoAtual);

#endif