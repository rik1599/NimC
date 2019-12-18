#include "game.h"
#include <time.h>

/**
 * Crea il campo di gioco (estrae a caso il numero di pedine di ogni pila)
 * 
 * @return una struct rappresentante il campo di gioco
 */
field_t *startGame(){

    int numElementiPilaUno;
    int numElementiPilaDue;
    time_t t;
    
    srand((unsigned) time(&t));
    
    //Numeri di elementi delle due pile
    numElementiPilaUno = rand() % 20;
    numElementiPilaDue = rand() % 20;
    
    //Creazione del campo di gioco
    field_t campoDiGioco;
    
    //Assegnazione del numero di pendine in ogni pila
    campoDiGioco.pile[0] = numElementiPilaUno;
    campoDiGioco.pile[1] = numElementiPilaDue;

    return &campoDiGioco;
}

/**
 * Imposta la pila scelta dal giocatore di turno, se il numero dovesse essere diverso da (0,1)
 * torna -1
 * Se una pila è vuota e viene selezionata torna -2
 * 
 * @param game il gioco in corso
 * @param pila la pila da scegliere (0,1)
 */
int scegliPila(game_t *game, unsigned int pila){  
    
    game_t gioco = *game;// Accesso al gioco
    field_t campo = *gioco.field;// Accesso al campo
    campo.pilascelta = pila;

    if(pila != 0 || pila != 1){
        return -1;
    }else if (campo.pile[campo.pilascelta] == 0)
    {
        return -2;
    }

    return 0;
}

/**
 * Rimuove le pedine dalla pila scelta dal giocatore
 * Se il numero di pedine inserito dovesse essere maggiore di quelle rimaste nella pila scelta
 * torna -1
 * Se una delle due pile dovesse essere vuota e nell'altra sono presenti ancora pedine,
 * il numero massimo di pedine che possono essere tolte è il numero delle pedine presenti - 1
 * (vince chi toglie l'ultima pedina)
 * 
 * @param game il gioco in corso
 * @param pedine il numero di pedine da rimuovere dalla pila scelta in precedenza
 */
int mossa(game_t *game, unsigned int pedine){
    
    game_t gioco = *game;// Accesso al gioco
    field_t campo = *gioco.field;// Accesso al campo

    if(campo.pile[campo.pilascelta] < pedine){
        return -1;
    }
    // TO DO: Chiedere chiarimenti sulla seconda parte del contratto
    return 0;
}


/**
 * Controlla se è presente al più una pedina tra tutte e due le pile
 * Se è vero torna il giocatore di turno che viene dichiarato vincitore
 */
int winner(game_t *game){
    game_t gioco = *game;// Accesso al gioco
    field_t campo = *gioco.field;// Accesso al campo
    int pedineRimaste;

    pedineRimaste = campo.pile[0] + campo.pile[1];

    if(pedineRimaste = 1){
        return gioco.turn;
    }
    
    // TO DO: Chiedere il valore da restituire nel caso in cui le pedine > 1
    return -2;
}

/**
 * Cambia il turno tra i giocatori
 */
void cambiaTurno(game_t *game){
   
    game_t gioco = *game;// Accesso al gioco
    
    switch (gioco.turn)
    {
    case 0:
        gioco.turn = 1;
        break;
    case 1:
        gioco.turn = 0;
        break;
    }

}