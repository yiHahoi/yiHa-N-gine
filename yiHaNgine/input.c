#include "yiHaNgine.h"


/* Funcion para recibir entradas al estado de juego */
int input(GAME_STATE* state){
    while(SDL_PollEvent(&(state->WIN.event))){
        switch(state->WIN.event.type){
            case SDL_MOUSEMOTION:
                state->MSE.x = (state->CAM.width/state->WIN.WIDTH)*state->WIN.event.motion.x;
                state->MSE.y = (state->CAM.height/state->WIN.HEIGHT)*(state->WIN.HEIGHT - state->WIN.event.motion.y); // Restamos por coord. invertida de SDL.
                break;
            case SDL_MOUSEBUTTONDOWN:
                if(state->WIN.event.button.button == SDL_BUTTON_RIGHT)
                    state->MSE.rButton = 1;
                if(state->WIN.event.button.button == SDL_BUTTON_LEFT)
                    state->MSE.lButton = 1;
                break;
            case SDL_MOUSEBUTTONUP:
                if(state->WIN.event.button.button == SDL_BUTTON_RIGHT)
                    state->MSE.rButton = 0;
                if(state->WIN.event.button.button == SDL_BUTTON_LEFT)
                    state->MSE.lButton = 0;
                break;
            case SDL_KEYDOWN:
                switch(state->WIN.event.key.keysym.sym){
                    case SDLK_UP: state->KEY.up = 1; break;
                    case SDLK_DOWN: state->KEY.down = 1; break;
                    case SDLK_LEFT: state->KEY.left = 1; break;
                    case SDLK_RIGHT: state->KEY.right = 1; break;
                    case SDLK_q: state->KEY.q = 1; break;
                    case SDLK_w: state->KEY.w = 1; break;
                    case SDLK_e: state->KEY.e = 1; break;
                    case SDLK_r: state->KEY.r = 1; break;
                    case SDLK_a: state->KEY.a = 1; break;
                    case SDLK_s: state->KEY.s = 1; break;
                    case SDLK_d: state->KEY.d = 1; break;
                    case SDLK_f: state->KEY.f = 1; break;
                    case SDLK_z: state->KEY.z = 1; break;
                    case SDLK_x: state->KEY.x = 1; break;
                    case SDLK_c: state->KEY.c = 1; break;
                    case SDLK_v: state->KEY.v = 1; break;
                    case SDLK_p:
                        if(SDL_GetTicks()-state->WIN.stateSubTicks > state->WIN.minPauseTime){
                            state->WIN.PAUSE = !state->WIN.PAUSE;
                            state->WIN.stateSubTicks = SDL_GetTicks();
                                                                                             }
                        break;
                    case SDLK_SPACE: state->KEY.space = 1; break;
                    case SDLK_LCTRL: state->KEY.ctrl = 1; break;
                    case SDLK_LALT: state->KEY.alt = 1; break;
                    case SDLK_LSHIFT: state->KEY.shift = 1; break;
                    case SDLK_TAB: state->KEY.tab = 1; break;
                    case SDLK_ESCAPE: state->KEY.esc = 1; break;
                                                       }
                break;
            case SDL_KEYUP:
                switch(state->WIN.event.key.keysym.sym){
                    case SDLK_UP: state->KEY.up = 0; break;
                    case SDLK_DOWN: state->KEY.down = 0; break;
                    case SDLK_LEFT: state->KEY.left = 0; break;
                    case SDLK_RIGHT: state->KEY.right = 0; break;
                    case SDLK_q: state->KEY.q = 0; break;
                    case SDLK_w: state->KEY.w = 0; break;
                    case SDLK_e: state->KEY.e = 0; break;
                    case SDLK_r: state->KEY.r = 0; break;
                    case SDLK_a: state->KEY.a = 0; break;
                    case SDLK_s: state->KEY.s = 0; break;
                    case SDLK_d: state->KEY.d = 0; break;
                    case SDLK_f: state->KEY.f = 0; break;
                    case SDLK_z: state->KEY.z = 0; break;
                    case SDLK_x: state->KEY.x = 0; break;
                    case SDLK_c: state->KEY.c = 0; break;
                    case SDLK_v: state->KEY.v = 0; break;
                    case SDLK_SPACE: state->KEY.space = 0; break;
                    case SDLK_LCTRL: state->KEY.ctrl = 0; break;
                    case SDLK_LALT: state->KEY.alt = 0; break;
                    case SDLK_LSHIFT: state->KEY.shift = 0; break;
                    case SDLK_TAB: state->KEY.tab = 0; break;
                    case SDLK_ESCAPE: state->KEY.esc = 0; break;
                                                       }
                break;
            case SDL_QUIT:
                state->WIN.Quit=1;
                break;
                                     }
                                             }
	return 0;
}

/* Funcion que se encarga de cargar los datos a un objeto */
int loadObj(GAME_STATE* state, char* dataFile, int objPosition, int objTotal){
    FILE* datos;
    char imgFile[state->WIN.fileCharLimit];
    char line[state->WIN.fileCharLimit];
    int objNumber = objPosition;
    Uint8 alphaColor[3];
    int ctrl1, ctrl2, ctrl3, ctrl4, temp;

    while(objNumber < objPosition + objTotal){

        datos = NULL;
        datos = fopen(dataFile, "r");
        if(datos == NULL)
            return 1;

        if(objNumber == objPosition)
            state->OBJ[objNumber].objCopy = 0;
        else
            state->OBJ[objNumber].objCopy = 1;

        fgets(line, state->WIN.fileCharLimit+1, datos);
        sscanf(line,"%d", &state->OBJ[objNumber].animTotal);
        fgets(line, state->WIN.fileCharLimit+1, datos);
        sscanf(line,"%d", &state->OBJ[objNumber].animCurrent);
        fgets(line, state->WIN.fileCharLimit+1, datos);
        sscanf(line,"%d", &state->OBJ[objNumber].camRelative);
        fgets(line, state->WIN.fileCharLimit+1, datos);
        sscanf(line,"%f", &state->OBJ[objNumber].scale);
        fgets(line, state->WIN.fileCharLimit+1, datos);
        sscanf(line,"%f", &state->OBJ[objNumber].rotation);
        fgets(line, state->WIN.fileCharLimit+1, datos);
        sscanf(line,"%f", &state->OBJ[objNumber].transparency);
        fgets(line, state->WIN.fileCharLimit+1, datos);
        sscanf(line,"%d", &state->OBJ[objNumber].xMirror);
        fgets(line, state->WIN.fileCharLimit+1, datos);
        sscanf(line,"%d", &state->OBJ[objNumber].yMirror);
        fgets(line, state->WIN.fileCharLimit+1, datos);
        sscanf(line,"%f", &state->OBJ[objNumber].pos.x);
        fgets(line, state->WIN.fileCharLimit+1, datos);
        sscanf(line,"%f", &state->OBJ[objNumber].pos.y);
        fgets(line, state->WIN.fileCharLimit+1, datos);
        sscanf(line,"%d", &state->OBJ[objNumber].zPos);
        fgets(line, state->WIN.fileCharLimit+1, datos);
        sscanf(line,"%f", &state->OBJ[objNumber].vel.x);
        fgets(line, state->WIN.fileCharLimit+1, datos);
        sscanf(line,"%f", &state->OBJ[objNumber].vel.y);
        fgets(line, state->WIN.fileCharLimit+1, datos);
        sscanf(line,"%f", &state->OBJ[objNumber].accel.x);
        fgets(line, state->WIN.fileCharLimit+1, datos);
        sscanf(line,"%f", &state->OBJ[objNumber].accel.y);
        fgets(line, state->WIN.fileCharLimit+1, datos);
        sscanf(line,"%d", &state->OBJ[objNumber].ghost);

        state->OBJ[objNumber].anim = malloc(state->OBJ[objNumber].animTotal * sizeof(ANIMATION));
        if(state->OBJ[objNumber].anim == NULL)
            return 1;

        for(ctrl1=0; ctrl1 < state->OBJ[objNumber].animTotal; ctrl1++){
            fgets(line, state->WIN.fileCharLimit+1, datos);
            sscanf(line,"%d", &state->OBJ[objNumber].anim[ctrl1].subFrameTotal);
            fgets(line, state->WIN.fileCharLimit+1, datos);
            sscanf(line,"%d", &state->OBJ[objNumber].anim[ctrl1].subFrameCurrent);
            fgets(line, state->WIN.fileCharLimit+1, datos);
            sscanf(line,"%d", &state->OBJ[objNumber].anim[ctrl1].subFramePixelWidth);
            fgets(line, state->WIN.fileCharLimit+1, datos);
            sscanf(line,"%d", &state->OBJ[objNumber].anim[ctrl1].subFramePixelHeight);
            fgets(line, state->WIN.fileCharLimit+1, datos);
            sscanf(line,"%d", &state->OBJ[objNumber].anim[ctrl1].loop);
            fgets(line, state->WIN.fileCharLimit+1, datos);
            sscanf(line,"%d", &state->OBJ[objNumber].anim[ctrl1].fps);
            fgets(line, state->WIN.fileCharLimit+1, datos);
            sscanf(line,"%u", &state->OBJ[objNumber].anim[ctrl1].lastClock);

            if(!state->OBJ[objNumber].objCopy){
                temp = state->OBJ[objNumber].anim[ctrl1].subFrameTotal;
                state->OBJ[objNumber].anim[ctrl1].subFrame = malloc( temp * sizeof(QUAD));
                if(state->OBJ[objNumber].anim[ctrl1].subFrame == NULL)
                    return 1;
                                              }
            else{
                state->OBJ[objNumber].anim[ctrl1].subFrame = state->OBJ[objPosition].anim[ctrl1].subFrame;
                }

            fgets(line, state->WIN.fileCharLimit+1, datos);
            sscanf(line,"%s", imgFile);
            fgets(line, state->WIN.fileCharLimit+1, datos);
            sscanf(line,"%u", &alphaColor[0]);
            fgets(line, state->WIN.fileCharLimit+1, datos);
            sscanf(line,"%u", &alphaColor[1]);
            fgets(line, state->WIN.fileCharLimit+1, datos);
            sscanf(line,"%u", &alphaColor[2]);

            if(!state->OBJ[objNumber].objCopy){
                loadImage(imgFile, alphaColor, &state->OBJ[objNumber].anim[ctrl1].frame);
                state->OBJ[objNumber].anim[ctrl1].coll = malloc(state->OBJ[objNumber].anim[ctrl1].subFrameTotal * sizeof(COLLISION));
                if(state->OBJ[objNumber].anim[ctrl1].coll == NULL)
                    return 1;
                                              }
            else{
                state->OBJ[objNumber].anim[ctrl1].frame = state->OBJ[objPosition].anim[ctrl1].frame;
                state->OBJ[objNumber].anim[ctrl1].coll = state->OBJ[objPosition].anim[ctrl1].coll;
                }

            for(ctrl2=0; ctrl2 < state->OBJ[objNumber].anim[ctrl1].subFrameTotal; ctrl2++){
                fgets(line, state->WIN.fileCharLimit+1, datos);
                sscanf(line,"%f", &state->OBJ[objNumber].anim[ctrl1].subFrame[ctrl2].x);
                fgets(line, state->WIN.fileCharLimit+1, datos);
                sscanf(line,"%f", &state->OBJ[objNumber].anim[ctrl1].subFrame[ctrl2].y);
                fgets(line, state->WIN.fileCharLimit+1, datos);
                sscanf(line,"%f", &state->OBJ[objNumber].anim[ctrl1].subFrame[ctrl2].w);
                fgets(line, state->WIN.fileCharLimit+1, datos);
                sscanf(line,"%f", &state->OBJ[objNumber].anim[ctrl1].subFrame[ctrl2].h);
                                                                                          }

             for(ctrl2=0; ctrl2 < state->OBJ[objNumber].anim[ctrl1].subFrameTotal; ctrl2++){
                 fgets(line, state->WIN.fileCharLimit+1, datos);
                 sscanf(line,"%d", &state->OBJ[objNumber].anim[ctrl1].coll[ctrl2].polyTotal);
                 fgets(line, state->WIN.fileCharLimit+1, datos);
                 sscanf(line,"%f", &state->OBJ[objNumber].anim[ctrl1].coll[ctrl2].pos.x);
                 fgets(line, state->WIN.fileCharLimit+1, datos);
                 sscanf(line,"%f", &state->OBJ[objNumber].anim[ctrl1].coll[ctrl2].pos.y);
                 fgets(line, state->WIN.fileCharLimit+1, datos);
                 sscanf(line,"%f", &state->OBJ[objNumber].anim[ctrl1].coll[ctrl2].radius);
                 if(!state->OBJ[objNumber].objCopy){
                     state->OBJ[objNumber].anim[ctrl1].coll[ctrl2].poly = malloc(state->OBJ[objNumber].anim[ctrl1].coll[ctrl2].polyTotal * sizeof(POLY));
                     if(state->OBJ[objNumber].anim[ctrl1].coll[ctrl2].poly == NULL)
                         return 1;
                                                   }
                 else{
                     state->OBJ[objNumber].anim[ctrl1].coll[ctrl2].poly = state->OBJ[objPosition].anim[ctrl1].coll[ctrl2].poly;
                     }

                 for(ctrl3=0; ctrl3 < state->OBJ[objNumber].anim[ctrl1].coll[ctrl2].polyTotal; ctrl3++){
                     fgets(line, state->WIN.fileCharLimit+1, datos);
                     sscanf(line,"%d", &state->OBJ[objNumber].anim[ctrl1].coll[ctrl2].poly[ctrl3].pointTotal);
                     fgets(line, state->WIN.fileCharLimit+1, datos);
                     sscanf(line,"%f", &state->OBJ[objNumber].anim[ctrl1].coll[ctrl2].poly[ctrl3].pos.x);
                     fgets(line, state->WIN.fileCharLimit+1, datos);
                     sscanf(line,"%f", &state->OBJ[objNumber].anim[ctrl1].coll[ctrl2].poly[ctrl3].pos.y);
                     fgets(line, state->WIN.fileCharLimit+1, datos);
                     sscanf(line,"%f", &state->OBJ[objNumber].anim[ctrl1].coll[ctrl2].poly[ctrl3].radius);
                    if(!state->OBJ[objNumber].objCopy){
                        state->OBJ[objNumber].anim[ctrl1].coll[ctrl2].poly[ctrl3].point = malloc(state->OBJ[objNumber].anim[ctrl1].coll[ctrl2].poly[ctrl3].pointTotal * sizeof(VECT));
                        if(state->OBJ[objNumber].anim[ctrl1].coll[ctrl2].poly[ctrl3].point == NULL)
                            return 1;
                                                      }
                    else{
                        state->OBJ[objNumber].anim[ctrl1].coll[ctrl2].poly[ctrl3].point = state->OBJ[objPosition].anim[ctrl1].coll[ctrl2].poly[ctrl3].point;
                        }
                     for(ctrl4=0; ctrl4 < state->OBJ[objNumber].anim[ctrl1].coll[ctrl2].poly[ctrl3].pointTotal; ctrl4++){
                        fgets(line, state->WIN.fileCharLimit+1, datos);
                        sscanf(line,"%f", &state->OBJ[objNumber].anim[ctrl1].coll[ctrl2].poly[ctrl3].point[ctrl4].x);
                        fgets(line, state->WIN.fileCharLimit+1, datos);
                        sscanf(line,"%f", &state->OBJ[objNumber].anim[ctrl1].coll[ctrl2].poly[ctrl3].point[ctrl4].y);
                                                                                                                        }



                                                                                                       }
                                                                                         }
                                                                    }
        fclose(datos);
        objNumber++;
                                             }
    return 0;
}


/* Rellenamos los datos en un archivo de texto con los siguientes datos:

animTotal
animCurrent
camRelative
scale
rotation
transparency
xMirror
yMirror
pos.x
pos.y
zPos
vel.x
vel.y
accel.x
accel.y
ghost
for(animTotal)
    subFrameTotal
    subFrameCurrent
    subFramePixelWidth
    subFramePixelHeight
    loop
    fps
    lastClock
    imgFile
    alphaColor[0]
    alphaColor[1]
    alphaColor[2]
        for(subFrameTotal)
        subFrame.x
        subFrame.y
        subFrame.w
        subFrame.h
        for(subFrameTotal)
        polyTotal
        pos.x
        pos.y
        radius
        for(polyTotal)
            pointTotal
            pos.x
            pos.y
            radius
            for(pointTotal)
                x
                y
*/
