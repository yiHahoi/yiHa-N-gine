#include "yiHaNgine.h"


/* Funcion para calcular la logica del estado de juego */
int logic(GAME_STATE* state){
    GetTicks(state);
    if(!state->WIN.PAUSE){
        if(game(state))
            return 1;
                         }
    if(state->KEY.esc==1)
        state->WIN.Quit=1;
    return 0;
}

/* Producto punto entre dos vectores */
float dot(VECT* A, VECT* B){
  return A->x * B->x + A->y * B->y;
}

/* Normaliza un vector */
void normalize(VECT* vct){
    float temp = 1/sqrt(vct->x*vct->x+vct->y*vct->y);
    vct->x *= temp;
    vct->y *= temp;
}

/* Escala un poligono en "factor" unidades */
void polyScale(POLY* polygon, VECT* center, float factor){
  int ctrl1;
  polygon->pos.x -= center->x;
  polygon->pos.y -= center->y;
  polygon->pos.x *= factor;
  polygon->pos.y *= factor;
  polygon->pos.x += factor*center->x;
  polygon->pos.y += factor*center->y;
  polygon->radius *= factor;
  for(ctrl1=0; ctrl1<polygon->pointTotal; ctrl1++){
    polygon->point[ctrl1].x -= center->x;
    polygon->point[ctrl1].y -= center->y;
    polygon->point[ctrl1].x *= factor;
    polygon->point[ctrl1].y *= factor;
    polygon->point[ctrl1].x += factor*center->x;
    polygon->point[ctrl1].y += factor*center->y;
                                                  }
}

/* Rota un poligono en "angle" grados */
void polyRotate(POLY* polygon, VECT* center, float angle){
  int ctrl1;
  float temp, pi = 3.14159265;
  float COS=cos(2*0.002777*pi*angle), SEN=sin(2*0.002777*pi*angle);
  polygon->pos.x -= center->x;
  polygon->pos.y -= center->y;
  temp = polygon->pos.x*COS-polygon->pos.y*SEN;
  polygon->pos.y = polygon->pos.x*SEN+polygon->pos.y*COS;
  polygon->pos.x = temp;
  polygon->pos.x += center->x;
  polygon->pos.y += center->y;
  for(ctrl1=0; ctrl1<polygon->pointTotal; ctrl1++){
    polygon->point[ctrl1].x -= center->x;
    polygon->point[ctrl1].y -= center->y;
    temp = polygon->point[ctrl1].x*COS-polygon->point[ctrl1].y*SEN;
    polygon->point[ctrl1].y = polygon->point[ctrl1].x*SEN+polygon->point[ctrl1].y*COS;
    polygon->point[ctrl1].x = temp;
    polygon->point[ctrl1].x += center->x;
    polygon->point[ctrl1].y += center->y;
                                                  }
}

/* Traslada un poligono desde el origen a "pos" */
void polyTranslate(POLY* polygon, VECT* pos){
  int ctrl1;
  polygon->pos.x += pos->x;
  polygon->pos.y += pos->y;
  for(ctrl1=0; ctrl1<polygon->pointTotal; ctrl1++){
    polygon->point[ctrl1].x += pos->x;
    polygon->point[ctrl1].y += pos->y;
                                                  }
}

/* Deteccion de colision circular */
int cirCollision(VECT* A, float aRadius, VECT* B, float bRadius){
  if((A->x-B->x)*(A->x-B->x)+(A->y-B->y)*(A->y-B->y) < (aRadius+bRadius)*(aRadius+bRadius))
    return 1;
  return 0;
}

/* Deteccion de colision entre poligonos */
int polyCollision(POLY* A, POLY* B, RESPONSE* collResp1, RESPONSE* collResp2){
  int i, j, k;
  float aMin, aMax, bMin, bMax, respMag;
  VECT axis, respAxis, pA, pB;
  for(i=0; i < A->pointTotal; i++){
    if(i==A->pointTotal-1){
      pA.x = A->point[0].x;
      pA.y = A->point[0].y;
                          }
    else{
      pA.x = A->point[i+1].x;
      pA.y = A->point[i+1].y;
        }
    pB.x = A->point[i].x;
    pB.y = A->point[i].y;
    axis.x = pA.y - pB.y;
    axis.y = pB.x - pA.x;
    aMin = dot(&pA, &axis);
    aMax = dot(&pA, &axis);
    for(k=0; k < A->pointTotal; k++){
      pA.x = A->point[k].x;
      pA.y = A->point[k].y;
        if(dot(&pA, &axis) < aMin)
          aMin = dot(&pA, &axis);
        if(dot(&pA, &axis) > aMax)
          aMax = dot(&pA, &axis);
                                    }
    for(j=0; j < B->pointTotal; j++){
      pB.x = B->point[j].x;
      pB.y = B->point[j].y;
      bMin = dot(&pB, &axis);
      bMax = dot(&pB, &axis);
      for(k=0; k < B->pointTotal; k++){
        pB.x = B->point[k].x;
        pB.y = B->point[k].y;
        if(dot(&pB, &axis) < bMin)
          bMin = dot(&pB, &axis);
        if(dot(&pB, &axis) > bMax)
          bMax = dot(&pB, &axis);
                                      }
      if(aMin < bMin){
        if(aMax < bMin)
          return 0;
        if(!i && !j){
          respMag = aMax-bMin;
          respAxis.x = -axis.x;
          respAxis.y = -axis.y;
        }
        if(aMax-bMin<respMag){
          respMag = aMax-bMin;
          respAxis.x = -axis.x;
          respAxis.y = -axis.y;
                             }
                     }
      if(bMin < aMin){
        if(bMax < aMin)
          return 0;
        if(!i && !j){
          respMag = bMax-aMin;
          respAxis.x = axis.x;
          respAxis.y = axis.y;
        }
        if(bMax-aMin<respMag){
          respMag = bMax-aMin;
          respAxis.x = axis.x;
          respAxis.y = axis.y;
                             }
                     }
                                    }
                                  }
// Segunda pasada. Cambio de poligono.
  for(i=0; i < B->pointTotal; i++){
    if(i==B->pointTotal-1){
      pB.x = B->point[0].x;
      pB.y = B->point[0].y;
                          }
    else{
      pB.x = B->point[i+1].x;
      pB.y = B->point[i+1].y;
        }
    pA.x = B->point[i].x;
    pA.y = B->point[i].y;
    axis.x = pB.y - pA.y;
    axis.y = pA.x - pB.x;
    bMin = dot(&pB, &axis);
    bMax = dot(&pB, &axis);
    for(k=0; k < B->pointTotal; k++){
      pB.x = B->point[k].x;
      pB.y = B->point[k].y;
      if(dot(&pB, &axis) < bMin)
        bMin = dot(&pB, &axis);
      if(dot(&pB, &axis) > bMax)
        bMax = dot(&pB, &axis);
                                    }
    for(j=0; j < A->pointTotal; j++){
      pA.x = A->point[j].x;
      pA.y = A->point[j].y;
      aMin = dot(&pA, &axis);
      aMax = dot(&pA, &axis);
      for(k=0; k < A->pointTotal; k++){
        pA.x = A->point[k].x;
        pA.y = A->point[k].y;
        if(dot(&pA, &axis) < aMin)
          aMin = dot(&pA, &axis);
        if(dot(&pA, &axis) > aMax)
          aMax = dot(&pA, &axis);
                                      }
      if(aMin < bMin){
        if(aMax < bMin)
          return 0;
        if(aMax-bMin<respMag){
          respMag = aMax-bMin;
          respAxis.x = -axis.x;
          respAxis.y = -axis.y;
                             }
                     }
      if(bMin < aMin){
        if(bMax < aMin)
          return 0;
        if(bMax-aMin<respMag){
          respMag = bMax-aMin;
          respAxis.x = axis.x;
          respAxis.y = axis.y;
                             }
                     }
                                    }
                                  }
  collResp1->axis.x = respAxis.x;
  collResp1->axis.y = respAxis.y;
  collResp1->mag = respMag;
  collResp2->axis.x = -respAxis.x;
  collResp2->axis.y = -respAxis.y;
  collResp2->mag = respMag;
  return 1;
}

int collision(OBJECT* obj1, OBJECT* obj2, GAME_STATE* state){
  if(obj1->ghost || obj2->ghost){
    obj1->collResp.cirCollided = 0;
    obj1->collResp.objCollided = 0;
    obj1->collResp.axis.x = 0.0;
    obj1->collResp.axis.y = 0.0;
    obj1->collResp.mag = 0.0;
    obj2->collResp.cirCollided = 0;
    obj2->collResp.objCollided = 0;
    obj2->collResp.axis.x = 0.0;
    obj2->collResp.axis.y = 0.0;
    obj2->collResp.mag = 0.0;
    return 0;
                                }
  int animCurr1, animCurr2, subFrameCurr1, subFrameCurr2;
  int polyTot1, polyTot2, ctrl1, ctrl2, ctrl3;
  int pixelWidth1, pixelHeight1, pixelWidth2, pixelHeight2;
  float r1, r2;
  VECT pos1, pos2, pointA[state->WIN.polyMaxPoints], pointB[state->WIN.polyMaxPoints];
  POLY A, B;
  A.point = pointA;
  B.point = pointB;
  animCurr1 = obj1->animCurrent;
  subFrameCurr1 = obj1->anim[animCurr1].subFrameCurrent;
  pos1.x = obj1->anim[animCurr1].coll[subFrameCurr1].pos.x * obj1->scale + obj1->pos.x;
  pos1.y = obj1->anim[animCurr1].coll[subFrameCurr1].pos.y * obj1->scale + obj1->pos.y;
  r1 = obj1->scale * obj1->anim[animCurr1].coll[subFrameCurr1].radius;
  animCurr2 = obj2->animCurrent;
  subFrameCurr2 = obj2->anim[animCurr2].subFrameCurrent;
  pos2.x = obj2->anim[animCurr2].coll[subFrameCurr2].pos.x * obj2->scale + obj2->pos.x;
  pos2.y = obj2->anim[animCurr2].coll[subFrameCurr2].pos.y * obj2->scale + obj2->pos.y;
  r2 = obj2->scale * obj2->anim[animCurr2].coll[subFrameCurr2].radius;
  if(obj1->camRelative){
    pos1.x -= state->CAM.pos.x;
    pos1.y -= state->CAM.pos.y;
                       }
  if(obj2->camRelative){
    pos2.x -= state->CAM.pos.x;
    pos2.y -= state->CAM.pos.y;
                       }
  obj1->collResp.cirCollided = cirCollision(&pos1, r1, &pos2, r2);
  obj2->collResp.cirCollided = obj1->collResp.cirCollided;
  if(obj1->collResp.cirCollided){
    polyTot1 = obj1->anim[animCurr1].coll[subFrameCurr1].polyTotal;
    polyTot2 = obj2->anim[animCurr2].coll[subFrameCurr2].polyTotal;
    for(ctrl1=0; ctrl1<polyTot1; ctrl1++){
      pixelWidth1 = obj1->anim[animCurr1].subFramePixelWidth;
      pixelHeight1 = obj1->anim[animCurr1].subFramePixelHeight;
      A.pointTotal = obj1->anim[animCurr1].coll[subFrameCurr1].poly[ctrl1].pointTotal;
      for(ctrl3=0; ctrl3<A.pointTotal; ctrl3++){
        A.point[ctrl3].x = obj1->anim[animCurr1].coll[subFrameCurr1].poly[ctrl1].point[ctrl3].x;
        A.point[ctrl3].y = obj1->anim[animCurr1].coll[subFrameCurr1].poly[ctrl1].point[ctrl3].y;
                                               }
      A.pos.x = obj1->anim[animCurr1].coll[subFrameCurr1].poly[ctrl1].pos.x;
      A.pos.y = obj1->anim[animCurr1].coll[subFrameCurr1].poly[ctrl1].pos.y;
      A.radius = obj1->anim[animCurr1].coll[subFrameCurr1].poly[ctrl1].radius;
      pos1.x = 0.5*pixelWidth1;
      pos1.y = 0.5*pixelHeight1;
      polyScale(&A, &pos1, obj1->scale);
      pos1.x *= obj1->scale;
      pos1.y *= obj1->scale;
      polyRotate(&A, &pos1, obj1->rotation);
      pos1.x = obj1->pos.x;
      pos1.y = obj1->pos.y;
      if(obj1->camRelative){
        pos1.x -= state->CAM.pos.x;
        pos1.y -= state->CAM.pos.y;
                           }
      polyTranslate(&A, &pos1);
      for(ctrl2=0; ctrl2<polyTot2; ctrl2++){
        pixelWidth2 = obj2->anim[animCurr2].subFramePixelWidth;
        pixelHeight2 = obj2->anim[animCurr2].subFramePixelHeight;
        B.pointTotal = obj2->anim[animCurr2].coll[subFrameCurr2].poly[ctrl2].pointTotal;
        for(ctrl3=0; ctrl3<A.pointTotal; ctrl3++){
          B.point[ctrl3].x = obj2->anim[animCurr2].coll[subFrameCurr2].poly[ctrl2].point[ctrl3].x;
          B.point[ctrl3].y = obj2->anim[animCurr2].coll[subFrameCurr2].poly[ctrl2].point[ctrl3].y;
                                                 }
        B.pos.x = obj2->anim[animCurr2].coll[subFrameCurr2].poly[ctrl2].pos.x;
        B.pos.y = obj2->anim[animCurr2].coll[subFrameCurr2].poly[ctrl2].pos.y;
        B.radius = obj2->anim[animCurr2].coll[subFrameCurr2].poly[ctrl2].radius;
        pos2.x = 0.5*pixelWidth2;
        pos2.y = 0.5*pixelHeight2;
        polyScale(&B, &pos2, obj2->scale);
        pos2.x *= obj2->scale;
        pos2.y *= obj2->scale;
        polyRotate(&B, &pos2, obj2->rotation);
        pos2.x = obj2->pos.x;
        pos2.y = obj2->pos.y;
        if(obj2->camRelative){
          pos2.x -= state->CAM.pos.x;
          pos2.y -= state->CAM.pos.y;
                             }
        polyTranslate(&B, &pos2);
        if(cirCollision(&A.pos, A.radius, &B.pos, B.radius)){
          if(polyCollision(&A, &B, &obj1->collResp, &obj2->collResp)){
            obj1->collResp.objCollided = 1;
            obj2->collResp.objCollided = 1;
            normalize(&obj1->collResp.axis);
            normalize(&obj2->collResp.axis);
            return 1;
                                                                   }
                                                            }
                                           }
                                         }
                        }
  obj1->collResp.objCollided = 0;
  obj1->collResp.axis.x = 0.0;
  obj1->collResp.axis.y = 0.0;
  obj1->collResp.mag = 0.0;
  obj2->collResp.objCollided = 0;
  obj2->collResp.axis.x = 0.0;
  obj2->collResp.axis.y = 0.0;
  obj2->collResp.mag = 0.0;
  return 0;
}
