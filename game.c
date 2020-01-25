#include "game.h"
#include <stdlib.h>
#include <time.h>

field_t *createField()
{

    int numElementiPilaUno;
    int numElementiPilaDue;
    time_t t;

    srand((unsigned)time(&t));

    // Numeri di elementi delle due pile
    numElementiPilaUno = rand() % 20 + 2;
    numElementiPilaDue = rand() % 20 + 2;

    // Creazione del campo di gioco
    field_t *campoDiGioco = malloc(sizeof(field_t));

    // Assegnazione del numero di pendine in ogni pila
    campoDiGioco->pile[0] = numElementiPilaUno;
    campoDiGioco->pile[1] = numElementiPilaDue;

    return campoDiGioco;
}

int scegliPila(game_t *game, unsigned int pila)
{

    if (pila != 0 && pila != 1)
    {
        return INVALID_RANGE;
    }
    else if (game->field->pile[pila] == 0)
    {
        return INVALID_RANGE;
    }
    else
    {
        game->field->pilascelta = pila;
    }

    return 0;
}

int getMaxPedine(game_t *game)
{
    field_t *field = game->field;
    if (field->pile[0] == 0)
    {
        return field->pile[1]-1;
    }

    if (field->pile[1] == 0)
    {
        return field->pile[0]-1;
    }
    
    return field->pile[field->pilascelta];
}

int mossa(game_t *game, unsigned int pedine)
{
    int maxPedine = getMaxPedine(game);
    if (pedine > maxPedine || pedine == 0)
    {
        return INVALID_RANGE;
    }
    else
    {
        game->field->pile[game->field->pilascelta] -= pedine;
    }
    
    return 0;
}

int winner(game_t *game)
{
    int pedineRimaste;

    pedineRimaste = game->field->pile[0] + game->field->pile[1];

    if (pedineRimaste == 1)
    {
        return game->players[!game->turn];
    }

    return NO_WINNER;
}

void cambiaTurno(game_t *game)
{
    game->turn = !game->turn;
}