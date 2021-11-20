#include "controles.h"
#include "timer.h"

void controle_iniciaVariaveis()
{
    J1A = 0;
    J1B = 0;
    J1C = 0;
    J1S = 0;
    J1BAIXO = 0;
    J1CIMA = 0;
    J1ACount = 0;
    J1BCount = 0;
    J1CCount = 0;
}

void controle_Handler(u16 joy, u16 changed, u16 state)
{
	if (joy == JOY_1)
	{
		// detecta botão A pressionado
        if (changed & BUTTON_A)
        {
            if(state & BUTTON_A )
            {
                J1A = 1;
                J1ACount = getTick();
            }
            // botão A solto
            else
            {
                J1A = 0;
            }
        }
        // detecta botão B pressionado
        if (changed & BUTTON_B)
        {
            if(state & BUTTON_B )
            {
                J1B = 1;
                J1BCount = getTick();
            }
            // botão B solto
            else
            {
                J1B = 0;
            }
        }
        // detecta botão C pressionado
        if (changed & BUTTON_C)
        {
            if(state & BUTTON_C )
            {
                J1C = 1;
                J1CCount = getTick();
            }
            // botão C solto
            else
            {
                J1C = 0;
            }
        }
        if (changed & BUTTON_START)
        {
            if(state & BUTTON_START )
            {
                J1S = 1;
            }
            // botão C solto
            else
            {
                J1S = 0;
            }
        }

        if (changed & BUTTON_DOWN)
        {
            if(state & BUTTON_DOWN )
            {
                J1BAIXO = 1;
            }
            else
            {
                J1BAIXO = 0;
            }
        }
        if (changed & BUTTON_UP)
        {
            if(state & BUTTON_UP )
            {
                J1CIMA = 1;
            }
            else
            {
                J1CIMA = 0;
            }
        }
        
	}
}