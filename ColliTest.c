#include "ColliTest.h"


/* Funcion para iniciar variables del juego */
int gameInit(GAME_STATE* state){

/*----PRUEBA----*/

    state->objTotal = 4;

	state->OBJ = malloc(state->objTotal * sizeof(OBJECT));
    if(state->OBJ == NULL)
        return 1;

    if(loadObj(state, "data/A.txt", 0, 1))
        return 1;

    if(loadObj(state, "data/B.txt", 1, 1))
        return 1;

    if(loadObj(state, "data/C.txt", 2, 1))
        return 1;

    if(loadObj(state, "data/D.txt", 3, 1))
        return 1;


/*----PRUEBA----*/

	return 0;
}


/* Funcion del Juego */
int game(GAME_STATE* state){

/*----PRUEBA----*/

state->OBJ[0].pos.x = state->MSE.x;
state->OBJ[0].pos.y = state->MSE.y;
state->OBJ[1].pos.x = state->MSE.x;
state->OBJ[1].pos.y = state->MSE.y;
state->OBJ[2].pos.x = state->MSE.x;
state->OBJ[2].pos.y = state->MSE.y;

if(state->KEY.right)
    state->OBJ[3].pos.x += 5;
if(state->KEY.left)
    state->OBJ[3].pos.x -= 5;
if(state->KEY.up)
    state->OBJ[3].pos.y += 5;
if(state->KEY.down)
    state->OBJ[3].pos.y -= 5;
if(state->KEY.s){
    state->OBJ[0].scale +=0.1;
    state->OBJ[1].scale +=0.1;
    state->OBJ[2].scale +=0.1;
}
if(state->KEY.d){
    state->OBJ[0].scale -=0.1;
    state->OBJ[1].scale -=0.1;
    state->OBJ[2].scale -=0.1;
}

if(state->MSE.lButton){
    state->OBJ[0].rotation +=1.5;
    state->OBJ[1].rotation +=1.5;
    state->OBJ[2].rotation +=1.5;
}

if(state->MSE.rButton){
    state->OBJ[0].rotation -=1.5;
    state->OBJ[1].rotation -=1.5;
    state->OBJ[2].rotation -=1.5;
}

if(collision(&state->OBJ[0], &state->OBJ[3], state)){
    state->OBJ[3].pos.x += state->OBJ[3].collResp.mag*state->OBJ[3].collResp.axis.x*0.005;
    state->OBJ[3].pos.y += state->OBJ[3].collResp.mag*state->OBJ[3].collResp.axis.y*0.005;
    state->OBJ[0].transparency = 0.0;
    state->OBJ[1].transparency = 0.0;
    state->OBJ[2].transparency = 1.0;
                                                    }
else{
    state->OBJ[2].transparency = 0.0;
    if(!state->OBJ[3].collResp.cirCollided){
    state->OBJ[0].transparency = 1.0;
    state->OBJ[1].transparency = 0.0;
                             }
    else{
        state->OBJ[0].transparency = 0.0;
        state->OBJ[1].transparency = 1.0;
        }
    }

/*----PRUEBA----*/

	return 0;
}


