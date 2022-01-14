#ifndef __MUSICAS_H__
#define __MUSICAS_H__

#include <genesis.h>

#define VEMELHO_X 231
#define VEMELHO_X_INICIO FIX16(182.0)
#define VERDE_X 142
#define AMARELO_X 49
#define AMARELO_X_INICIO FIX16(98.0)

#define VEMELHO_B_X VEMELHO_X +15
#define VEMELHO_B_X_INICIO VEMELHO_X_INICIO + FIX16(15)
#define VERDE_B_X VERDE_X +16
#define AMARELO_B_X AMARELO_X + 16
#define AMARELO_B_X_INICIO AMARELO_X_INICIO + FIX16(17)


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
	STREETS_OF_RAGE,
	GUILE_THEME,
	ZELDA,
	DRACULA,
	TOP_GEAR,
	ALTERED,
	SMOOTH,
	NUM_MUSICAS,
} Musica; // Define uma nota

const u32 tempos_sonic[434];
const Nota notas_sonic[434];
const u16 duracao_sonic[434];
const u16 tamanho_sonic;
const u8 velocidade_sonic;
const u16 delay_sonic;
const u8 loops_sonic;


const Nota notas_bib[223];
const u16 duracao_bib[223];
const u32 tempos_bib[223];
const u16 tamanho_bib;
const u8 velocidade_bib;
const u16 delay_bib;
const u8 loops_bib;

const Nota notas_sor[165];
const u16 duracao_sor[165];
const u32 tempos_sor[165];
const u16 tamanho_sor;
const u8 velocidade_sor;
const u16 delay_sor;
const u8 loops_sor;

const Nota notas_guile[259];
const u16 duracao_guile[259];
const u32 tempos_guile[259];
const u16 tamanho_guile;
const u8 velocidade_guile;
const u16 delay_guile;
const u8 loops_guile;

const Nota notas_castle[443];
const u16 duracao_castle[443];
const u32 tempos_castle[443];
const u16 tamanho_castle;
const u8 velocidade_castle;
const u16 delay_castle;
const u8 loops_castle;

const Nota notas_zelda[491];
const u16 duracao_zelda[491];
const u32 tempos_zelda[491];
const u16 tamanho_zelda;
const u8 velocidade_zelda;
const u16 delay_zelda;
const u8 loops_zelda;

const Nota notas_topGear[1196];
const u16 duracao_topGear[1196];
const u32 tempos_topGear[1196];
const u16 tamanho_topGear;
const u8 velocidade_topGear;
const u16 delay_topGear;
const u8 loops_topGear;

const Nota notas_ateredBeast[125];
const u16 duracao_ateredBeast[125];
const u32 tempos_ateredBeast[125];
const u16 tamanho_ateredBeast;
const u8 velocidade_ateredBeast;
const u16 delay_ateredBeast;
const u8 loops_ateredBeast;

const Nota notas_smooth[404];
const u16 duracao_smooth[404];
const u32 tempos_smooth[404];
const u16 tamanho_smooth;
const u8 velocidade_smooth;
const u16 delay_smooth;
const u8 loops_smooth;

#endif