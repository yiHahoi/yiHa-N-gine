#include "yiHaNgine/yiHaNgine.h"


void camPan1(GAME_STATE* state){
    /*float t = 0.1;
    float camMaxForceX = 10;
    float error = 0.00001;
    float delta;
   // state->CAM.accel.x = 0;
    if(state->OBJ[0].vel.x > state->CAM.vel.x)
        state->CAM.accel.x = camMaxForceX;
    if(state->OBJ[0].vel.x < state->CAM.vel.x)
        state->CAM.accel.x = -camMaxForceX;
    state->CAM.vel.x +=  t*state->CAM.accel.x;
    delta = state->OBJ[0].vel.x - state->CAM.vel.x;
    if(delta <= error || delta >= -error){
        state->CAM.accel.x = 0;
        state->CAM.vel.x = state->OBJ[0].vel.x;
                                         }
    state->CAM.pos.x += t*state->CAM.vel.x;
    printf("%f  %f\n", state->OBJ[0].accel.x,state->CAM.accel.x);*/
state->CAM.pos.x = state->OBJ[0].pos.x - 650;
state->CAM.pos.y = state->OBJ[0].pos.y - 250;
}

/* Funcion para controlar la camara */
void camPan2(GAME_STATE* state){
    if(state->MSE.lButton){ // Control de camara.
        state->CAM.pos.x +=  state->CAM.pan.x - state->MSE.x;
        state->CAM.pos.y +=  state->CAM.pan.y - state->MSE.y;
        state->CAM.pan.x = state->MSE.x;
        state->CAM.pan.y = state->MSE.y;
                          }
    if(!state->MSE.lButton){
        state->CAM.pan.x = state->MSE.x;
        state->CAM.pan.y = state->MSE.y;
                           }
}

