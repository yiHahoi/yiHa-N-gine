#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_opengl.h"


/*---------
 Vector 2D
 ---------*/

typedef struct{	// Estructura que define un vector en 2 dimensiones.

	float x,y;	// Componentes x e y del vector.

}VECT;

/*--------
 Poligono
 --------*/

typedef struct{			// Estructura que define un poligono en dos dimensiones.

	VECT* point;		// Vector de puntos que forman el poligono.
	int pointTotal;		// Total de puntos que conforman el poligono.
	VECT pos;			// Ubicacion del centro del poligono (para deteccion de colision circular).
	float radius;		// Radio del poligono (para deteccion de colision circular).

}POLY;

/*---------
 Float Rect
 ---------*/

typedef struct{	// Estructura tipo SDL_Rect pero compuesta de floats.

	float x;	// Coordenada x.
	float y;	// Coordenada y.
	float w;	// Ancho.
	float h;	// Altura.

}QUAD;

/*---------
 Colisiones
 ---------*/

typedef struct{		// Estructura que guarda los poligonos de colision.

    POLY* poly;		// Poligonos de colision para un subFrame de una animacion.
    int polyTotal;	// Total de poligonos de colision de un subFrame.
    VECT pos;		// Centro del objeto.
    float radius;	// Radio del objeto;

}COLLISION;

/*--------------------
 Respuesta de Colision
---------------------*/

 typedef struct{

    int cirCollided;	// Sirve como flag para indicar que los objetos han colisionado radialmente.
    int objCollided;	// Sirve como flag para indicar que los objetos han colisionado poligonalmente.
    VECT axis;			// Direccion en la que existe la menor penetracion entre objetos.
    float mag;			// Magnitud de la penetracion entre objetos.

 }RESPONSE;

/*----------
 Animaciones
 ----------*/

typedef struct{				 // Estructura que almacena todos los datos de una animacion.

	GLuint frame;            // Textura en opengl que contiene la animación.
	QUAD* subFrame;      	 // Sub-imagenes de la animación (vector almacenado en memoria dinamica).
	int subFrameTotal;       // Total de imagenes que componen la animacion.
	int subFrameCurrent;	 // Imagen en la que se encuentra la animacion.
	int subFramePixelWidth;	 //	Ancho de sub-imagen en pixeles.
	int subFramePixelHeight; //	Alto de sub-imagen en pixeles.
	int loop;				 // La animacion se repite si loop=1.
	Uint32 lastClock;		 // Reloj actual de animacion.
	int fps;               	 // Sub-imagenes por segundo de la animación (velocidad).
    COLLISION* coll;      	 // Bordes de colision. Si coll=NULL se hace un solo test con ancho de subframe.

}ANIMATION;

/*-------
 Objetos
 -------*/

typedef struct{			// Estructura que almacena los datos de cada objeto o personaje en el juego.

    VECT pos;			// Vector de posicion del objeto.
    VECT vel;			// Vector de velocidad del objeto.
    VECT accel;			// Vector de aceleracion del objeto.
    int zPos;			// Profundidad en la que se encuentra el objeto.
    float mass;			// Masa del objeto.
    float scale;		// Ampliado de imagen.
    float rotation;		// Rotacion de imagen.
    RESPONSE collResp;	// Contiene flags que indican si el objeto se encuentra colisionando.
	int ghost;			// Objetos con ghost=1 no colisionan.
    int objCopy;        // Es una copia del objeto original (objCopy=1 es copia, objCopy=0 es original).
    ANIMATION* anim;	// Animaciones del objeto (vector almacenado en memoria dinamica).
    int animTotal;		// Total de animaciones del objeto.
    int animCurrent;	// Animacion actual del objeto.
    int camRelative;	// Posicion en el renderizado del objeto es afectado por posicion de camara.
    float transparency; // Transparencia del objeto, 0.0=transparente, 1.0=visible.
    int xMirror;		// Inversion horizontal.
    int yMirror;		// Inversion vertical.

}OBJECT;

/*-------
 Ventana
 -------*/

typedef struct{					// Estructura que almacena las propiedades de la ventana de juego.

	char* NAME;					// Titulo de la ventana.
	char* ICON;                 // Icono del Motor.
 	SDL_Surface* SURFACE;		// Puntero a superficie que contiene la ventana.
	int WIDTH;					// Ancho de la ventana.
	int HEIGHT;					// Altura de la ventana.
	int BPP;					// Bits por pixel a utilizar en la ventana.
	int FPS;					// Tasa de refresco limite del programa (FPS=0 ilimitada).
	int zPosMax;				// Maximo nivel de profundidad para objetos.
	int polyMaxPoints;          // Maximo numero de puntos para definir un poligono.
	Uint32 mSeg;				// Milisegundos por cada frame (solo si FPS distinto de cero).
	Uint32 lastClock;			// Ultimo tiempo almacenado por funcion fps();
	int Quit;					// Variable que controla el cierre de la ventana.
	SDL_Event event;			// Evento que se encarga de controlar las opciones de ventana.
    int console_Enable;			// Variable que controla la salida de datos por consola.
    Uint32 console_FpsUpdate;	// Variable que controla cada cuantos milisegundos se actualizan los fps.
    Uint32 console_Clocks;		// Variable que ayuda a controlar console_FpsUpdate.
    int console_Frames;			// Contador de Frames de juego para calcular FPS.
    int fileCharLimit;          // Largo maximo de caracteres en nombre de archivos de datos.
    int PAUSE;                  // Variable que identifica si el juego se encuentra en pausa (detiene variables de tiempo).
    Uint32 minPauseTime;        // Tiempo minimo para que cambie el valor de la variable PAUSA al apretar la tecla p.
    Uint32 totalPauseTime;      // Variable que aloja el tiempo total que se ha encontrado el estado de juego en pausa.
    Uint32 stateSubTicks;       // Variable temporal para alojar un periodo de tiempo.

}WINDOW;

/*-----
 Mouse
 -----*/

 typedef struct{	// Estructura con el estado del mouse.

 	int x;			// Posicion en el eje x del mouse dentro de la ventana.
 	int y;			// Posicion en el eje y del mouse dentro de la ventana.
 	int rButton;	// Estado del boton derecho del mouse.
 	int lButton;	// Estado del boton izquierdo del mouse.

 }MOUSE;

/*------
 Teclado
 ------*/

 typedef struct{	// Estructura con el estado del teclado.

 	int up;			// Estado de la tecla arriba.
 	int down;		// Estado de la tecla abajo.
	int left;		// Estado de la tecla izquierda.
 	int right;		// Estado de la tecla derecha.
 	int q;			// Estado de la tecla q.
 	int w;			// Estado de la tecla w.
 	int e;			// Estado de la tecla e.
 	int r;			// Estado de la tecla r.
 	int a;			// Estado de la tecla a.
 	int s;			// Estado de la tecla s.
 	int d;			// Estado de la tecla d.
	int f;			// Estado de la tecla f.
 	int z;			// Estado de la tecla z.
 	int x;			// Estado de la tecla x.
 	int c;			// Estado de la tecla c.
	int v;			// Estado de la tecla v.
	int p;			// Estado de la tecla p.
 	int space;		// Estado de la tecla espacio.
 	int ctrl;		// Estado de la tecla control izquierdo.
 	int alt;		// Estado de la tecla alt izquierdo.
 	int shift;		// Estado de la tecla shift izquierdo.
 	int tab;		// Estado de la tecla tab.
 	int esc;		// Estado de la tecla Esc.

 }KEYBOARD;

/*------
 Camara
 -----*/

 typedef struct{

 	VECT pos;		// Vector de posicion de camara.
 	VECT vel;		// Vector de velocidad de camara.
 	VECT accel;		// Vector de aceleracion de camara.
 	VECT pan;		// Vector de paneo de camara.
 	int width;      // Ancho de camara a mostrar en pantalla.
 	int height;     // Alto de camara a mostrar en pantalla.

 }CAMERA;

/*-----------------------------
 Estructura principal del juego
 -----------------------------*/

typedef struct{		// Estructura principal del juego que contiene el estado actual.

	WINDOW WIN;		// Ventana del juego.
	OBJECT* OBJ;	// Vector que contiene todos los objetos del juego (almacenado en memoria dinamica).
	int objTotal;	// Total de objetos en el juego.
    //GAME game;		// Variables de juego que queremos mantener separadas del motor.
	MOUSE MSE;		// Variables de mouse.
	KEYBOARD KEY;   // Variables de teclado.
	CAMERA CAM;		// Camara del juego.
	Uint32 TICKS;   // Tiempo del estado de juego en ms (no del programa, se detiene al pausar).
	Uint32 DELTA;   // Delta de tiempo para frame (para calcular posicion).

}GAME_STATE;

/*------------------------
 Declaracion de funciones
 ------------------------*/

int init(GAME_STATE*);					    					// Valor inicial a variables de estructuras y crea ventana.
int input(GAME_STATE*);					    					// Respuesta al teclado y mouse.
int logic(GAME_STATE*);     			    					// Deteccion de colisiones, fisica del juego, etc.
int render(GAME_STATE*);				    					// Muestra el estado actual de juego en pantalla.
int gameInit(GAME_STATE*);		    					        // Valor inicial a variables de estructuras de objetos del juego.
int game(GAME_STATE*);			    					        // Funcion que controla toda la logica del juego.
int clean(GAME_STATE*);					    					// Libera todas las variables almacenadas en memoria dinamica (malloc).
void fps(GAME_STATE*);					    					// Limita los frames por segundo del programa (FPS=0 ilimitado).
Uint32 GetTicks(GAME_STATE*);               					// Entrega los ticks del estado de juego (NO del PROGRAMA como SDL_GetTicks).
int collision(OBJECT*, OBJECT*, GAME_STATE*);			        // Deteccion de colisiones entre objetos.
int polyCollision(POLY*, POLY*, RESPONSE*, RESPONSE*); 			// Deteccion de colisiones entre poligonos.
int cirCollision(VECT*, float, VECT*, float);					// Deteccion de colisiones entre circulos mediante radios.
void polyScale(POLY*, VECT*, float);							// Escala un poligono en una cantidad dada por factor y con respecto al vector center.
void polyRotate(POLY*, VECT*, float);							// Rota un poligono en angle grados con respecto al punto dado por center.
void polyTranslate(POLY*, VECT*);								// Traslada un poligono al vector pos.
float dot(VECT*, VECT*);										// Producto punto entre dos vectores.
void normalize(VECT*);											// Normaliza un vector (le da modulo unitario a un vector).
int loadObj(GAME_STATE*, char*, int, int);  					// Carga los datos de un archivo de texto a un objeto.
void loadImage(char*, Uint8*, GLuint*);	    					// Carga una imagen en una textura de opengl.
int animUpdate(ANIMATION*, GAME_STATE*);    					// Actualiza la sub-imagen de la animacion actual de cada objeto.
int drawObj(OBJECT*, CAMERA*);			    					// Dibuja el objeto en la pantalla usando opengl.
