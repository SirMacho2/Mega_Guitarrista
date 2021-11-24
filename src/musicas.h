#ifndef __MUSICAS_H__
#define __MUSICAS_H__

#include <genesis.h>

#define VEMELHO_X 200
#define VERDE_X 141
#define AMARELO_X 79

#define VEMELHO_B_X VEMELHO_X +15
#define VERDE_B_X VERDE_X +15
#define AMARELO_B_X AMARELO_X +16


#define TAMANHO_SONIC 434

typedef enum // Cria uma ENUM para indetificar uma nota
{
	AMARELA = 1,
	VERDE = 2,
    VEMELHA = 4
} Nota; // Define uma nota

typedef enum // Cria uma ENUM para indetificar uma nota
{
	SONIC,
	BACK_IN_BLACK,
} Musica; // Define uma nota


// typedef struct // Cria uma STRUCT para armazenar os dados de uma nota
// {
// 	Nota *notas; // notas que seram tocadas
// 	s16 *tempos; // intervalo de tempo para proxima nota
// 	u16 tamanho;
// 	u8 velocidade;
// }Musica;




Musica cria_musica(	Nota *notas, s16 *tempos, u16 tamanho,	u8 velocidade);

const u32 tempos_sonic[434];
const Nota notas_sonic[434];
const u16 duracao_sonic[434];
const u16 tamanho_sonic;
const u8 velocidade_sonic;
const u16 delay_sonic;


const Nota notas_bib[223];
const u16 duracao_bib[223];
const u32 tempos_bib[223];
const u16 tamanho_bib;
const u8 velocidade_bib;
const u16 delay_bib;


#endif