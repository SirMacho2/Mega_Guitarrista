#ifndef __CONTROLES_H__
#define __CONTROLES_H__

#include <genesis.h>


bool J1A, J1B, J1C, J1S, J1BAIXO,J1CIMA;
u16 J1ACount, J1BCount, J1CCount;

void controle_iniciaVariaveis();
void controle_Handler(u16 joy, u16 changed, u16 state);
#endif