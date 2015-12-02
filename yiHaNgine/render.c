#include "yiHaNgine.h"


/* Funcion para inicializar el estado de juego */
int init(GAME_STATE* state){
    state->WIN.NAME = "yiHa'N'gine";
    state->WIN.ICON = "yiHaNgine/yiHaPig.ico";
    state->WIN.WIDTH = 640;
    state->WIN.HEIGHT = 480;
    state->WIN.BPP = 32;
    state->WIN.FPS = 60;             // Limite de FPS del programa.
    state->WIN.zPosMax = 3;          // Limite de profundidad.
    state->WIN.polyMaxPoints = 128;  // Limite de puntos por poligono.
    state->WIN.console_Enable = 1;   // Mostrar datos en Consola.
    state->WIN.SURFACE = NULL;
    state->WIN.Quit = 0;
    state->WIN.PAUSE = 0;
    state->WIN.lastClock = 0;
    state->WIN.console_FpsUpdate = 1000;
    state->WIN.console_Frames = 0;
    state->WIN.console_Clocks = 0;
    state->WIN.fileCharLimit = 128;
    state->TICKS = 0;
    state->WIN.totalPauseTime = 0;
    state->WIN.stateSubTicks = 0;
    state->WIN.minPauseTime = 200;
    if(state->WIN.FPS != 0)
        state->WIN.mSeg = 1000/state->WIN.FPS;
    if(state->WIN.console_Enable){   // Datos a consola.
        freopen("CON", "w", stdout); // Redirige stdout.
        freopen("CON", "w", stderr); // Redirige stderr.
                                 }
    /* Iniciar ventana del juego */
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );  // Buffer doble.
    SDL_Surface* ICON = NULL;
    ICON = SDL_LoadBMP(state->WIN.ICON);
    SDL_WM_SetIcon(ICON, NULL);
    printf("%s",ICON);
    state->WIN.SURFACE = SDL_SetVideoMode(state->WIN.WIDTH,state->WIN.HEIGHT,state->WIN.BPP,SDL_OPENGL);
    if(state->WIN.SURFACE==NULL)
        return 1;
    SDL_WM_SetCaption( state->WIN.NAME, NULL);	// Nombre de la ventana.

    state->MSE.x = 0;
    state->MSE.y = 0;
    state->MSE.rButton = 0;
    state->MSE.lButton = 0;
    state->KEY.up = 0;
    state->KEY.down = 0;
    state->KEY.left = 0;
    state->KEY.right = 0;
    state->KEY.q = 0;
    state->KEY.w = 0;
    state->KEY.e = 0;
    state->KEY.r = 0;
    state->KEY.a = 0;
    state->KEY.s = 0;
    state->KEY.d = 0;
    state->KEY.f = 0;
    state->KEY.z = 0;
    state->KEY.x = 0;
    state->KEY.c = 0;
    state->KEY.v = 0;
    state->KEY.p = 0;
    state->KEY.space = 0;
    state->KEY.ctrl = 0;
    state->KEY.alt = 0;
    state->KEY.shift = 0;
    state->KEY.tab = 0;
    state->KEY.esc = 0;

    state->CAM.pos.x = 0;
    state->CAM.pos.y = 0;
    state->CAM.pan.x = 0;
    state->CAM.pan.y = 0;
    state->CAM.width = 1600;
    state->CAM.height = 1200;

    glClearColor( 1, 1, 1, 0 );
    glDisable(GL_DEPTH_TEST);
    glEnable( GL_TEXTURE_2D );
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glAlphaFunc(GL_GREATER,0.1);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0, state->CAM.width, 0, state->CAM.height, -1, 1 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    return 0;
}

/* Funcion para renderizar en pantalla el estado de juego */
int render(GAME_STATE* state){
    int ctrl1, ctrl2;
    for(ctrl1=0; ctrl1 < state->objTotal; ctrl1++){ // actualizamos la animacion de cada obj.
        if(state->OBJ[ctrl1].anim[state->OBJ[ctrl1].animCurrent].subFrameTotal > 1)
            animUpdate(&state->OBJ[ctrl1].anim[state->OBJ[ctrl1].animCurrent], state);
                                                  }
    // Clear the screen before drawing
    glClear( GL_COLOR_BUFFER_BIT );
    // Dibujamos en orden de profundidad.
    for(ctrl2=0; ctrl2 <= state->WIN.zPosMax; ctrl2++){
        for(ctrl1=0; ctrl1 < state->objTotal; ctrl1++){
            if(ctrl2==state->OBJ[ctrl1].zPos)
                if(drawObj(&state->OBJ[ctrl1], &state->CAM))
                    return 1;
                                                      }
                                                      }
    // Actualizamos la pantalla.
    SDL_GL_SwapBuffers();
    // Calculamos FPS y limitamos si corresponde.
    fps(state);
    return 0;
}

/* Funcion para limpiar memoria */
int clean(GAME_STATE* state){
    int ctrl1, ctrl2, ctrl3, ctrl4;
    for(ctrl1=0; ctrl1<state->objTotal; ctrl1++){
        for(ctrl2=0; ctrl2<state->OBJ[ctrl1].animTotal; ctrl2++){
            for(ctrl3=0; ctrl3<state->OBJ[ctrl1].anim[ctrl2].subFrameTotal; ctrl3++){
                for(ctrl4=0; ctrl4 < state->OBJ[ctrl1].anim[ctrl2].coll[ctrl3].poly[ctrl4].pointTotal; ctrl4++){
                    if(!state->OBJ[ctrl1].objCopy){
                        free(state->OBJ[ctrl1].anim[ctrl2].coll[ctrl3].poly[ctrl4].point);
                                                  }
                                                                                                      }
                if(!state->OBJ[ctrl1].objCopy){
                    free(state->OBJ[ctrl1].anim[ctrl2].coll[ctrl3].poly);
                    state->OBJ[ctrl1].anim[ctrl2].coll[ctrl3].poly = NULL;
                                              }
                                                                                    }
            if(!state->OBJ[ctrl1].objCopy){
                glDeleteTextures(1, &state->OBJ[ctrl1].anim[ctrl2].frame);
                free(state->OBJ[ctrl1].anim[ctrl2].subFrame);
                free(state->OBJ[ctrl1].anim[ctrl2].coll);
                                          }
                                                                }
        free(state->OBJ[ctrl1].anim);
                                                }
    free(state->OBJ);
    return 0;
}

/* Funcion para controlar los FPS */
void fps(GAME_STATE* state){
    Uint32 clocks, newTime = SDL_GetTicks();
    if(state->WIN.console_Enable){
        state->WIN.console_Frames += 1;
        clocks = newTime - state->WIN.console_Clocks;
        if(clocks > state->WIN.console_FpsUpdate){
            printf("FPS: %0.2f\n", 1000*state->WIN.console_Frames/(float)clocks);
            state->WIN.console_Clocks = newTime;
            state->WIN.console_Frames = 0;
                                                 }
                                 }
    if(state->WIN.FPS){
        clocks = newTime - state->WIN.lastClock;
        if(clocks < state->WIN.mSeg)
            SDL_Delay(state->WIN.mSeg - clocks);
        state->WIN.lastClock = SDL_GetTicks();
                      }
}

/* Funcion para cargar imagenes con mismo formato de pantalla */
void loadImage(char* file, Uint8* RGB, GLuint* texture){
    SDL_Surface* img = NULL;
    SDL_Surface* optImg = NULL;
    SDL_Surface* optImg2 = NULL;
    GLenum texture_format;
    GLint nofcolors;
    img = IMG_Load(file);
    if(img != NULL){
        optImg = SDL_DisplayFormat(img);
        SDL_FreeSurface(img);
        if(optImg!=NULL){
        	Uint32 alpha = SDL_MapRGB( optImg->format, RGB[0], RGB[1], RGB[2] );
        	SDL_SetColorKey( optImg, SDL_SRCCOLORKEY, alpha );
        	optImg2 = SDL_DisplayFormatAlpha(optImg);
        	SDL_FreeSurface(optImg);
        			    }
                   }
    if(optImg2 != NULL){
        //get number of channels in the SDL surface
        nofcolors=optImg2->format->BytesPerPixel;
        //contains an alpha channel
        if(nofcolors==4){
            if(optImg2->format->Rmask==0x000000ff)
                texture_format=GL_RGBA;
            else
                texture_format=GL_BGRA;
                        }
        else{
            if(nofcolors==3){ //no alpha channel
                if(optImg2->format->Rmask==0x000000ff)
                    texture_format=GL_RGB;
                else
                    texture_format=GL_BGR;
                            }
            else
                printf("warning: the image is not truecolor...this will break ");
            }

        // Have OpenGL generate a texture object handle for us
        glGenTextures( 1, texture );
        // Bind the texture object
        glBindTexture( GL_TEXTURE_2D, *texture );

        // Set the texture's stretching properties
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        glTexImage2D( GL_TEXTURE_2D, 0, nofcolors, optImg2->w, optImg2->h, 0,
                      texture_format, GL_UNSIGNED_BYTE, optImg2->pixels );
        SDL_FreeSurface(optImg2);
                      }
}

/* Funcion para actualizar animaciones */
int animUpdate(ANIMATION* anim, GAME_STATE* state){
    if((!anim->loop) && (anim->subFrameCurrent==anim->subFrameTotal-1))
        return 0;
    Uint32 mSec = 1000/anim->fps;
    Uint32 mSecTotal = mSec*anim->subFrameTotal;
    Uint32 clocks = state->TICKS - anim->lastClock;
    if(clocks >= mSecTotal){
        while(clocks >= mSecTotal){
            clocks -= mSecTotal;
                                  }
        anim->lastClock = state->TICKS - clocks;
                           }
    anim->subFrameCurrent = clocks/mSec;
    return 0;
}

int drawObj(OBJECT* obj, CAMERA* cam){
    int animCurr = obj->animCurrent;
    int subFrameCurr = obj->anim[animCurr].subFrameCurrent;
    QUAD rect2, rect1 = obj->anim[animCurr].subFrame[subFrameCurr];
    rect2.x = obj->pos.x;
    rect2.y = obj->pos.y;
    rect2.w = obj->anim[animCurr].subFramePixelWidth;
    rect2.h = obj->anim[animCurr].subFramePixelHeight;

    // Bind the texture to which subsequent calls refer to
    glBindTexture( GL_TEXTURE_2D, obj->anim[animCurr].frame );
    glColor4f( 1, 1, 1, obj->transparency );
    glLoadIdentity();

    // Camara
    if(obj->camRelative)
        glTranslatef(-cam->pos.x, -cam->pos.y, 0);

    glTranslatef(obj->pos.x + obj->scale*rect2.w*0.5, obj->pos.y + obj->scale*rect2.h*0.5, 0);
    glRotatef(obj->rotation, 0, 0, 1);
    glTranslatef(-obj->scale*rect2.w*0.5, -obj->scale*rect2.h*0.5, 0);
    glScalef(obj->scale , obj->scale, 1.0);
    glTranslatef(-obj->pos.x, -obj->pos.y, 0);

if(!obj->xMirror && !obj->yMirror){
    glBegin( GL_QUADS );
    // Top-left vertex (corner)
    glTexCoord2f( rect1.x, rect1.y );
    glVertex3f( rect2.x, rect2.y + rect2.h, 0 );
    // Top-right vertex (corner)
    glTexCoord2f( rect1.x + rect1.w, rect1.y );
    glVertex3f( rect2.x + rect2.w, rect2.y + rect2.h, 0 );
    // Bottom-right vertex (corner)
    glTexCoord2f( rect1.x + rect1.w, rect1.y + rect1.h);
    glVertex3f( rect2.x + rect2.w, rect2.y, 0 );
    // Bottom-left vertex (corner)
    glTexCoord2f( rect1.x, rect1.y + rect1.h);
    glVertex3f( rect2.x, rect2.y, 0 );
    glEnd();
                                  }
else if(obj->xMirror && !obj->yMirror){
    glBegin( GL_QUADS );
    // Top-left vertex (corner)
    glTexCoord2f( rect1.x + rect1.w, rect1.y );
    glVertex3f( rect2.x, rect2.y + rect2.h, 0 );
    // Top-right vertex (corner)
    glTexCoord2f( rect1.x, rect1.y );
    glVertex3f( rect2.x + rect2.w, rect2.y + rect2.h, 0 );
    // Bottom-right vertex (corner)
    glTexCoord2f( rect1.x, rect1.y + rect1.h);
    glVertex3f( rect2.x + rect2.w, rect2.y, 0 );
    // Bottom-left vertex (corner)
    glTexCoord2f( rect1.x + rect1.w, rect1.y + rect1.h);
    glVertex3f( rect2.x, rect2.y, 0 );
    glEnd();
                                       }
else if(!obj->xMirror && obj->yMirror){
    glBegin( GL_QUADS );
    // Top-left vertex (corner)
    glTexCoord2f( rect1.x, rect1.y + rect1.h );
    glVertex3f( rect2.x, rect2.y + rect2.h, 0 );
    // Top-right vertex (corner)
    glTexCoord2f( rect1.x + rect1.w, rect1.y + rect1.h );
    glVertex3f( rect2.x + rect2.w, rect2.y + rect2.h, 0 );
    // Bottom-right vertex (corner)
    glTexCoord2f( rect1.x + rect1.w, rect1.y);
    glVertex3f( rect2.x + rect2.w, rect2.y, 0 );
    // Bottom-left vertex (corner)
    glTexCoord2f( rect1.x, rect1.y);
    glVertex3f( rect2.x, rect2.y, 0 );
    glEnd();
                                       }
else{
    glBegin( GL_QUADS );
    // Top-left vertex (corner)
    glTexCoord2f( rect1.x + rect1.w, rect1.y + rect1.h );
    glVertex3f( rect2.x, rect2.y + rect2.h, 0 );
    // Top-right vertex (corner)
    glTexCoord2f( rect1.x, rect1.y + rect1.h );
    glVertex3f( rect2.x + rect2.w, rect2.y + rect2.h, 0 );
    // Bottom-right vertex (corner)
    glTexCoord2f( rect1.x, rect1.y);
    glVertex3f( rect2.x + rect2.w, rect2.y, 0 );
    // Bottom-left vertex (corner)
    glTexCoord2f( rect1.x + rect1.w, rect1.y);
    glVertex3f( rect2.x, rect2.y, 0 );
    glEnd();
    }
    glLoadIdentity();
    return 0;
}

Uint32 GetTicks(GAME_STATE* state){
    if(state->WIN.PAUSE)
        state->WIN.totalPauseTime = SDL_GetTicks() - state->TICKS;
    else
        state->TICKS = SDL_GetTicks() - state->WIN.totalPauseTime;
    return state->TICKS;
}
