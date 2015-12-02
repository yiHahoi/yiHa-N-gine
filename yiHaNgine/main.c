#include "yiHaNgine.h"


/* MAIN */
int main(int argc, char* argv[]){

	/* Iniciar SDL y en caso de fallar terminar el programa */
	if(SDL_Init(SDL_INIT_EVERYTHING)<0)
    	return 1;

	/* Iniciar estructura de Estado del Juego (definido en yiHaNgine.h) */
	GAME_STATE state;	// state es la estructura principal del juego.
	if(init(&state))	// Le damos valores de inicio al programa.
		return 1;

	/* Inicio de las variables del juego */
	if(gameInit(&state))
    	return 1;

	/* Inicio del Loop de juego */
	while(state.WIN.Quit!=1){	// Mientras no se cierre la ventana.
		if(input(&state))	// Control de entrada.
			return 1;
		if(logic(&state))	// Control de estado de juego.
			return 1;
		if(render(&state))	// Control de renderizado.
			return 1;
	   		  				}

	if(clean(&state))	// Liberamos la memoria.
		return 1;
	SDL_Quit();			// Cerramos SDL.
	return 0;			// Terminamos el programa.
}
