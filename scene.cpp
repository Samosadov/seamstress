#include <QtGui>
#include <math.h>
#include "scene.h"

int num = 0;
float z0 = -120.0f, z1 = 50.0f;
float pi = 3.1416f, da = pi/180;
bool hcarcass = false, carcass=false;

tcur sBeg, sEnd;
tTable table1;
tChair chair1;
seamstress hum1;

tScene::tScene(QWidget* parent) : QGLWidget(parent)
{
  nSca = 0.01f;

  table1.x=75;
  hum1.h = 165;
  table1.h1 = 80; table1.h2 = 30; table1.h3 = 29; table1.w = 120;
  table1.pedal.h4=24; table1.pedal.h5=6;
  table1.d = 15;
  chair1.h=50; chair1.x = table1.x - (table1.Sh + table1.h3 + 20) - 25;
  hum1.broke = false; hum1.FlagM = false;
}

void tScene::defaultScene()
{
  hum1.h = 165;
  table1.h1 = 80; table1.h2 = 30; table1.h3 = 29; table1.w = 120;
  table1.pedal.h4=24; table1.pedal.h5=6;
  table1.d = 15;
  chair1.h=50; chair1.x = table1.x - (table1.Sh + table1.h3 + 20) - 25;
  Calc();
  Calc1();
  num=0;
  hcarcass = carcass = false;
  hum1.broke = false; hum1.FlagM = false;
  updateGL();
}

void tScene::Circle(float x0, float y0, float r)
{
  float x,y;
  glPointSize(1.0f);
  for (float a=0; a<=pi+pi; a+=da)
    {
      x=x0+r*cos(a); y=y0+r*sin(a);
      glBegin(GL_POINTS);
          glVertex2f(x,y);
      glEnd();
    }
}

float tScene::R(float x1, float y1, float x2, float y2)
{
  return sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}

void tScene::Calc()
{
  table1.pedal.h6 = table1.pedal.h5 + 10;
  table1.pedal.l = table1.h3 + table1.Sh;
  table1.w1 = table1.Sh + table1.h2 + 20;
  table1.w2 = table1.Sh + table1.h3 + 20;
}

void tScene::Calc1(bool f)
{
  hum1.L[1] = hum1.h*0.25f; //голень
  hum1.L[2] = hum1.h*0.29f; //бедро
  hum1.L[4] = hum1.h*0.36f; //корпус
  hum1.L[5] = hum1.h*0.375f; //шея+корпус
  hum1.L[6] = hum1.h*0.07f; //уровень глаз+шея
  hum1.L[7] = hum1.h*0.1f; //макушка
  hum1.L[3] = hum1.L[8] = hum1.h*0.02f; //опять глаза
  hum1.L[9] = hum1.h*0.19f; //плечо
  hum1.L[10] = hum1.h*0.17f; //локоть

  hum1.L[11] = hum1.h*0.125f; //плечи
//  hum1.L[12] = hum1.h*0.12f; //бёдра

  if (!f) {//если считаем первый раз
//--------------------------- Профиль
  //лодыжка правая
    hum1.joint[1].x = table1.x-table1.w2+table1.pedal.h4-8;
    hum1.joint[1].y = table1.pedal.h5+8;
    hum1.joint[1].f=false;
  //таз
    hum1.joint[3].x = chair1.x - 10;
    hum1.joint[3].y = chair1.h + 5;
    hum1.joint[3].f=false;

    hum1.joint[2].f = false;
    hum1.joint[4].f = false;
    hum1.joint[5].f = false;
    hum1.joint[6].f = false;
    hum1.joint[7].f = false;
    hum1.joint[8].f = false;
    hum1.joint[5].a = hum1.joint[4].a = pi/2;
    hum1.joint[7].a = hum1.joint[6].a = pi/2;
    hum1.joint[9].f = false;
    //запястье правое
    hum1.joint[10].x = table1.x-table1.w1;
    hum1.joint[10].y = table1.h1;
    hum1.joint[10].f=false;
    }
  else hum1.joint[10].y = table1.h1; //если не первый раз
//=========================== Профиль
  //колено правое
  float D = R(hum1.joint[1].x,hum1.joint[1].y,hum1.joint[3].x,hum1.joint[3].y);
  hum1.joint[3].a = acos((hum1.L[2]*hum1.L[2]-hum1.L[1]*hum1.L[1]+D*D)/(2*D*hum1.L[2]))-acos((hum1.joint[1].x-hum1.joint[3].x)/D);

  hum1.joint[2].x = hum1.joint[3].x+hum1.L[2]*cos(hum1.joint[3].a);
  hum1.joint[2].y = hum1.joint[3].y+hum1.L[2]*sin(hum1.joint[3].a);
  hum1.joint[2].a = atan((hum1.joint[1].y-hum1.joint[2].y)/(hum1.joint[1].x-hum1.joint[2].x));

  hum1.joint[1].a = pi/2-hum1.joint[2].a;
  //плечо правое
  hum1.joint[4].x = hum1.joint[3].x+hum1.L[4]*cos(hum1.joint[4].a);
  hum1.joint[4].y = hum1.joint[3].y+hum1.L[4]*sin(hum1.joint[4].a);
  //шея
  hum1.joint[5].x = hum1.joint[3].x+hum1.L[5]*cos(hum1.joint[5].a);
  hum1.joint[5].y = hum1.joint[3].y+hum1.L[5]*sin(hum1.joint[5].a);
  //глаза
  hum1.joint[6].x = hum1.joint[5].x+hum1.L[6]*cos(hum1.joint[6].a);
  hum1.joint[6].y = hum1.joint[5].y+hum1.L[6]*sin(hum1.joint[6].a);
  //макушка
  hum1.joint[7].x = hum1.joint[5].x+hum1.L[7]*cos(hum1.joint[7].a);
  hum1.joint[7].y = hum1.joint[5].y+hum1.L[7]*sin(hum1.joint[7].a);
  //опять глаза
  hum1.joint[8].a = hum1.joint[6].a-pi/2;
  hum1.joint[8].x = hum1.joint[6].x+hum1.L[3]*cos(hum1.joint[8].a);
  hum1.joint[8].y = hum1.joint[6].y+hum1.L[3]*sin(hum1.joint[8].a);
  hum1.L[8] = R(hum1.joint[5].x,hum1.joint[5].y,hum1.joint[8].x,hum1.joint[8].y);
  hum1.joint[8].a = hum1.joint[6].a-asin(hum1.L[3]/hum1.L[8]);
  //локоть правый
  D = R(hum1.joint[4].x,hum1.joint[4].y,hum1.joint[10].x,hum1.joint[10].y);
  hum1.joint[9].a = -acos((hum1.L[9]*hum1.L[9]-hum1.L[10]*hum1.L[10]+D*D)/(2*D*hum1.L[9]))-acos((hum1.joint[10].x-hum1.joint[4].x)/D);

  hum1.joint[9].x = hum1.joint[4].x+hum1.L[9]*cos(hum1.joint[9].a);
  hum1.joint[9].y = hum1.joint[4].y+hum1.L[9]*sin(hum1.joint[9].a);

  hum1.joint[10].a = -pi+atan((hum1.joint[10].y-hum1.joint[9].y)/(hum1.joint[10].x-hum1.joint[9].x));

//=========================== Вид сверху
/*  //правая лодыжка
  hum1.joint[11].x = z0+hum1.L[12];
  hum1.joint[11].y = z1+hum1.joint[1].x;
  hum1.joint[11].f=false;
  //левая лодыжка
  hum1.joint[21].x = z0-hum1.L[12];
  hum1.joint[21].y = hum1.joint[11].y;
  hum1.joint[21].f=false;
  //правое колено
  hum1.joint[12].x = z0+hum1.L[12];
  hum1.joint[12].y = z1+hum1.joint[2].x;
  hum1.joint[12].f = false;
  //левое колено
  hum1.joint[22].x = z0-hum1.L[12];
  hum1.joint[22].y = hum1.joint[12].y;
  hum1.joint[22].f = false;
  //таз
  hum1.joint[13].x = z0+hum1.L[12];
  hum1.joint[13].y = z1+hum1.joint[3].x;
  hum1.joint[13].f = false;

  hum1.joint[23].x = z0-hum1.L[12];
  hum1.joint[23].y = hum1.joint[13].y;
  hum1.joint[23].f = false;
*/
  //правое плечо
  hum1.joint[11].x = z0+hum1.L[11];
  hum1.joint[11].y = z1+hum1.joint[4].x;
  hum1.joint[11].f = false;
  //левое плечо
  hum1.joint[14].x = z0-hum1.L[11];
  hum1.joint[14].y = hum1.joint[11].y;
  hum1.joint[14].f = false;
  //правый локоть
  hum1.joint[12].x = z0+hum1.L[11];
  hum1.joint[12].y = z1+hum1.joint[9].x;
  hum1.joint[12].a = pi/2;
  //hum1.joint[12].o=pi/2;
  //hum1.joint[12].x = z0+hum1.L[11];
  //hum1.joint[12].y = z1+hum1.joint[9].x;
  hum1.joint[12].f = false;
  //левый локоть
  hum1.joint[15].x = z0-hum1.L[11];
  hum1.joint[15].y = hum1.joint[12].y;
  hum1.joint[15].a = pi/2;
  hum1.joint[16].o = hum1.joint[9].a;
  hum1.joint[15].f = false;
  //правое запястье
  hum1.joint[13].x = z0+hum1.L[11];
  hum1.joint[13].y = z1+hum1.joint[10].x;
  hum1.joint[13].a = pi/2;
  hum1.joint[13].f = false;
  //левое запястье
  hum1.joint[16].x = z0-hum1.L[11];
  hum1.joint[16].y = hum1.joint[13].y;
  hum1.joint[16].a = pi/2;
  hum1.joint[16].o = hum1.joint[10].a;
  hum1.joint[16].f = false;
/*  //шея  17
  hum1.joint[17].x = z0;
  hum1.joint[17].y = z1+hum1.joint[5].x;
  hum1.joint[17].f = false;*/
  //макушка  18
  hum1.joint[17].x = z0;
  hum1.joint[17].y = z1+hum1.joint[7].x;
  hum1.joint[17].f = false;
}

bool tScene::Check()
{
  bool tmp = false;
  float r1, r2;
  r1 = acos(((hum1.joint[1].x-hum1.joint[2].x)*(hum1.joint[1].y-hum1.joint[2].y)+(hum1.joint[3].x-hum1.joint[2].x)*(hum1.joint[3].y-hum1.joint[2].y))/(hum1.L[1]*hum1.L[2]));
  if ((r1<pi/2)||(r1>2*pi/3)) tmp = true;
  if (table1.h1-chair1.h<15) tmp = true;
  return tmp;
}

void tScene::MoveJoint(int k, float dx, float dy, float dt)
{
  float r1, t,t1;

  switch (k){
    case 1: {
        hum1.joint[1].x+=dx;
        if(R(hum1.joint[1].x,hum1.joint[1].y,hum1.joint[3].x,hum1.joint[3].y)>=hum1.L[1]+hum1.L[2]) MoveJoint(3,dx,0,0);
        hum1.joint[1].y+=dy;

        MoveJoint(2,0,0,0);
    } break;
    case 2: {
        float D = R(hum1.joint[1].x,hum1.joint[1].y,hum1.joint[3].x,hum1.joint[3].y);
        hum1.joint[3].a = acos((hum1.L[2]*hum1.L[2]-hum1.L[1]*hum1.L[1]+D*D)/(2*D*hum1.L[2]))-acos((hum1.joint[1].x-hum1.joint[3].x)/D);

        hum1.joint[2].x = hum1.joint[3].x+hum1.L[2]*cos(hum1.joint[3].a);
        hum1.joint[2].y = hum1.joint[3].y+hum1.L[2]*sin(hum1.joint[3].a);
        hum1.joint[2].f=false;
        hum1.joint[2].a = atan((hum1.joint[1].y-hum1.joint[2].y)/(hum1.joint[1].x-hum1.joint[2].x));

        hum1.joint[1].a = pi/2-hum1.joint[2].a;
    } break;
    case 3: {
        if ((dx!=0)&&(hum1.joint[3].x+dx>chair1.x-chair1.w)&&(hum1.joint[3].x+dx<chair1.x+chair1.w))
          {
            if ((dx<0)&&(R(hum1.joint[1].x,hum1.joint[1].y,hum1.joint[3].x,hum1.joint[3].y)>=hum1.L[1]+hum1.L[2]))
              {
                if (table1.pedal.h4>table1.pedal.minh4) {hum1.joint[1].x-=1; table1.pedal.h4-=1;}
              }
            else
              {
                hum1.joint[3].x+=dx;
                for (int i=4; i<=10; i++) hum1.joint[i].x+=dx;
                for (int i=11; i<=17; i++) hum1.joint[i].y+=dx;
              }
          }
        if (dy!=0){
            if ((R(hum1.joint[1].x,hum1.joint[1].y+dy,hum1.joint[3].x,hum1.joint[3].y)<hum1.L[1]+hum1.L[2])&&(R(hum1.joint[4].x,hum1.joint[4].y+dy,hum1.joint[10].x,hum1.joint[10].y)<hum1.L[9]+hum1.L[10])){
                hum1.joint[3].y+=dy;
                for (int i=4; i<=8; i++) hum1.joint[i].y+=dy;
                if (dy<0) MoveJoint(10,0,0,-da); else MoveJoint(10,0,0,da);
              }
            else chair1.h-=dy;
          }
        MoveJoint(2,0,0,0);

    } break;
    case 4: case 5: {
        if ((dx>0)&&(hum1.joint[k].a>pi/4)) t = -da;
        if ((dx<0)&&(hum1.joint[k].a<pi*0.75)) t = da;
        if ((dy>0)&&(hum1.joint[k].a<pi/2)) t = da;
        if ((dy>0)&&(hum1.joint[k].a>pi/2)) t = -da;
        if ((dy<0)&&(hum1.joint[k].a<pi*0.75)&&(hum1.joint[k].a>pi/2)) t = da;
        if ((dy<0)&&(hum1.joint[k].a>pi/4)&&(hum1.joint[k].a<pi/2)) t = -da;

        for (int i=4; i<=5; i++) {
            hum1.joint[i].a+=t;
            hum1.joint[i].x = hum1.joint[3].x + hum1.L[i]*cos(hum1.joint[i].a);
            hum1.joint[i].y = hum1.joint[3].y + hum1.L[i]*sin(hum1.joint[i].a);
        }
        MoveJoint(11,0,0,0);
        MoveJoint(14,0,0,0);
        MoveJoint(6,0,0,t);
        if (R(hum1.joint[4].x,hum1.joint[4].y,hum1.joint[10].x,hum1.joint[10].y)>=hum1.L[9]+hum1.L[10]) hum1.joint[10].x-=1;
        MoveJoint(9,0,0,0);
    } break;
    case 6: case 7: case 8: {
        if (hum1.joint[k].f) {
            if (((dx>0)||(dy<0))&&(hum1.joint[6].a>0)) dt=-da;
            if (((dx<0)||(dy>0))&&(hum1.joint[6].a<pi/2)) dt=da;
          }
        for (int i=6; i<=8; i++) {
            hum1.joint[i].a+=dt;

            hum1.joint[i].x = hum1.joint[5].x+hum1.L[i]*cos(hum1.joint[i].a);
            hum1.joint[i].y = hum1.joint[5].y+hum1.L[i]*sin(hum1.joint[i].a);
          }
        MoveJoint(17,0,0,0);

    } break;
    case 9: {
        if ((dx==0)&&(dy==0))
          {
            float D = R(hum1.joint[4].x,hum1.joint[4].y,hum1.joint[10].x,hum1.joint[10].y);
            if (D>=hum1.L[9]+hum1.L[10]) hum1.joint[10].x-=da*hum1.L[4];

            hum1.joint[9].a = -acos((hum1.L[9]*hum1.L[9]-hum1.L[10]*hum1.L[10]+D*D)/(2*D*hum1.L[9]))-acos((hum1.joint[10].x-hum1.joint[4].x)/D);

            hum1.joint[9].x = hum1.joint[4].x+hum1.L[9]*cos(hum1.joint[9].a);
            hum1.joint[9].y = hum1.joint[4].y+hum1.L[9]*sin(hum1.joint[9].a);
            hum1.joint[9].f=false;

            hum1.joint[10].a = -pi+atan((hum1.joint[10].y-hum1.joint[9].y)/(hum1.joint[10].x-hum1.joint[9].x));
          }
        else
          {
            r1 = R(hum1.joint[4].x,hum1.joint[4].y,hum1.joint[10].x,hum1.joint[10].y);

            if (((dx>0)&&(r1<hum1.L[k]+hum1.L[k+1])||(dy>0)&&(r1<hum1.L[k]+hum1.L[k+1]))&&(hum1.joint[k].a<-pi/4)) dt=da;
            if ((dx<0)&&(hum1.joint[k].a>-pi*0.75)||(dy<0)&&(hum1.joint[k].a>-pi/2)) dt=-da;

            hum1.joint[k].a+=dt;
            hum1.joint[k].x = hum1.joint[4].x+hum1.L[k]*cos(hum1.joint[k].a);
            hum1.joint[k].y = hum1.joint[4].y+hum1.L[k]*sin(hum1.joint[k].a);

            MoveJoint(10,0,0,dt);
          }
        hum1.joint[12].y = z1+hum1.joint[9].x;
        //hum1.joint[12].a =
        hum1.joint[13].y = z1+hum1.joint[10].x;

    } break;
    case 10: {
        if ((dx>0)&&(R(hum1.joint[4].x,hum1.joint[4].y,hum1.joint[10].x+dx,hum1.joint[10].y)<hum1.L[10]+hum1.L[9])||(dx<0)&&(hum1.joint[9].a>-pi*0.75))
          {
            hum1.joint[10].x+=dx;
            MoveJoint(9,0,0,0);
          }
        else if ((dy<0)&&(R(hum1.joint[4].x,hum1.joint[4].y,hum1.joint[10].x,hum1.joint[10].y)<hum1.L[10]+hum1.L[9])||(dy>0)&&(hum1.joint[10].y<table1.h1))
          {
            hum1.joint[10].y+=dy;
            MoveJoint(9,0,0,0);
          }
        else if (dt!=0) {hum1.joint[10].x+=hum1.L[9]*dt; MoveJoint(9,0,0,0);}

        hum1.joint[k].a=-pi+atan((hum1.joint[10].y-hum1.joint[9].y)/(hum1.joint[10].x-hum1.joint[9].x));

        hum1.joint[12].y = z1+hum1.joint[9].x;

        hum1.joint[13].y = z1+hum1.joint[10].x;

    } break;
    case 11: case 14: {
        if (hum1.joint[k].f) {
            hum1.joint[k].f = false;
            MoveJoint(4,dy,0,0);
            hum1.joint[k].f = true;
          }
        else {
            hum1.joint[11].y = z1+hum1.joint[4].x;
            hum1.joint[14].y = hum1.joint[11].y;
        }
    } break;
    case 12: {
        t=0;
        float D = R(hum1.joint[4].x,hum1.joint[4].y,hum1.joint[13].y-z1,table1.h1);
        r1 = R(hum1.joint[k].x,hum1.joint[k].y,hum1.joint[k-1].x,hum1.joint[k-1].y);
        float r2 = R(hum1.joint[k].x,hum1.joint[k].y,hum1.joint[k+1].x,hum1.joint[k+1].y);

        if (dx!=0) {
            if ((dx<0)&&(hum1.joint[k].a-da<pi-pi/6)) {
                if (hum1.joint[k].a>=hum1.joint[k+1].a) MoveJoint(k+1,0,0,da);
                t=da;
              }
            if ((dx>0)&&(hum1.joint[k].a+da>0)) t=-da;

            hum1.joint[k].a+=t;
            hum1.joint[k].x = hum1.joint[k-1].x + r1*cos(hum1.joint[k].a);
            hum1.joint[k].y = hum1.joint[k-1].y + r1*sin(hum1.joint[k].a);
            //MoveJoint(k+1,0,0,0);
            hum1.joint[k+1].x = hum1.joint[k].x + r2*cos(hum1.joint[k+1].a);
            hum1.joint[k+1].y = hum1.joint[k].y + r2*sin(hum1.joint[k+1].a);
            //Самый скользкий момент 1
            hum1.joint[9].x = hum1.joint[12].y-z1;
            hum1.joint[9].a = atan((hum1.joint[9].y-hum1.joint[4].y)/(hum1.joint[9].x-hum1.joint[4].x));
            hum1.joint[10].x = hum1.joint[13].y-z1;
            hum1.joint[10].a = -pi + atan((hum1.joint[10].y-hum1.joint[9].y)/(hum1.joint[10].x-hum1.joint[9].x));
          }
        else {
            if (dy!=0) {
                if (hum1.joint[k].a!=pi/2) {
                    if (dy>0) {
                        if (hum1.joint[k].a<pi/2) t=da;
                        else /*if (hum1.joint[k].a>pi/2)*/ t=-da;
                      }
                    else {
                        if ((hum1.joint[k].a<pi/2)&&(hum1.joint[k].a>0)) t=-da;
                        else if ((hum1.joint[k].a>pi/2)&&(hum1.joint[k].a<pi)) {
                            if (hum1.joint[k].a>=hum1.joint[k+1].a) MoveJoint(k+1,0,0,da);
                            t=da;
                        }
                      }
                    hum1.joint[k].a+=t;
                    hum1.joint[k].x = hum1.joint[k-1].x + r1*cos(hum1.joint[k].a);
                    hum1.joint[k].y = hum1.joint[k-1].y + r1*sin(hum1.joint[k].a);
                    //MoveJoint(k+1,0,0,0);
                    hum1.joint[k+1].x = hum1.joint[k].x + r2*cos(hum1.joint[k+1].a);
                    hum1.joint[k+1].y = hum1.joint[k].y + r2*sin(hum1.joint[k+1].a);
                  }
                else {
                    if ((dy>0)&&(D+dy<hum1.L[9]+hum1.L[10])) {
                        hum1.joint[k].y+=dy/sqrt(2);
                        hum1.joint[k+1].y+=dy/sqrt(2);
                      }
                    else if ((dy<0)&&((r1-dy<hum1.L[9]/sqrt(2))||(hum1.joint[k].y>hum1.joint[k-1].y))){
                        hum1.joint[k].y+=dy/sqrt(2);
                        hum1.joint[k+1].y+=dy/sqrt(2);
                      }
                  }
                //Самый скользкий момент 2
                hum1.joint[9].x = hum1.joint[12].y-z1;
                hum1.joint[9].a = atan((hum1.joint[9].y-hum1.joint[4].y)/(hum1.joint[9].x-hum1.joint[4].x));
                hum1.joint[10].x = hum1.joint[13].y-z1;
                hum1.joint[10].a = -pi + atan((hum1.joint[10].y-hum1.joint[9].y)/(hum1.joint[10].x-hum1.joint[9].x));
              }
            else if ((hum1.joint[k].a+dt<pi-pi/6)&&(hum1.joint[k].a>0)) {
                hum1.joint[k].a+=dt;
                hum1.joint[k].x = hum1.joint[k-1].x + r1*cos(hum1.joint[k].a);
                hum1.joint[k].y = hum1.joint[k-1].y + r1*sin(hum1.joint[k].a);
//                hum1.joint[k+1].x = hum1.joint[k].x + r2*cos(hum1.joint[k+1].a);
//                hum1.joint[k+1].y = hum1.joint[k].y + r2*sin(hum1.joint[k+1].a);
              }
          }

        /*        r1 = hum1.L[9];
        //r1 = R(hum1.joint[k].x,hum1.joint[k].y,hum1.joint[k-1].x,hum1.joint[k-1].y);
        if ((dx<0)&&(hum1.joint[12].a+da<pi-pi/6)) hum1.joint[12].a+=da;
        if ((dx>0)&&(hum1.joint[12].a-da>0)) hum1.joint[12].a-=da;
        if ((dy>0)&&(hum1.joint[9].a+da<hum1.joint[10].a)) hum1.joint[9].a+=da;
        if ((dy<0)&&(hum1.joint[9].a+da>-pi*0.75f)) hum1.joint[9].a-=da;

        hum1.joint[12].x = hum1.joint[11].x + r1*cos(hum1.joint[12].a)*cos(hum1.joint[9].a);
        hum1.joint[12].y = hum1.joint[11].y + r1*sin(hum1.joint[12].a)*cos(hum1.joint[9].a);

        hum1.joint[9].x = hum1.joint[4].x + r1*cos(hum1.joint[9].a)*cos(pi/2-hum1.joint[12].a);
        hum1.joint[9].y = hum1.joint[4].y + r1*sin(hum1.joint[9].a)*cos(pi/2-hum1.joint[12].a);
        //hum1.joint[9].x = hum1.joint[4].x + r1*cos(hum1.joint[9].a)*cos(hum1.joint[12].a);
        //hum1.joint[9].y = hum1.joint[4].y + r1*sin(hum1.joint[9].a)*cos(hum1.joint[12].a);

        /*r1 = R(hum1.joint[k].x,hum1.joint[k].y,hum1.joint[k-1].x,hum1.joint[k-1].y);
        if (dx!=0) {
            if ((dx<0)&&(hum1.joint[k].a<pi-pi/6)&&(hum1.joint[k].a<hum1.joint[k+1].a)) t=da;
            if ((dx>0)&&(hum1.joint[k].a>0)) t=-da;
            hum1.joint[k].a+=t;
            hum1.joint[k].x = hum1.joint[k-1].x + r1*cos(hum1.joint[k].a);
            hum1.joint[k].y = hum1.joint[k-1].y + r1*sin(hum1.joint[k].a);
            MoveJoint(k+1,0,0,t);
          }
        else {
            if (dy!=0) {
                if (hum1.joint[k].a!=pi/2) {
                    if (dy>0) {
                        if (hum1.joint[k].a<pi/2) t=da;
                        else if (hum1.joint[k].a>pi/2) t=-da;
                      }
                    else{
                        if (hum1.joint[k].a<pi/2) t=da;
                        else if (hum1.joint[k].a>pi/2) t=-da;
                      }
                    hum1.joint[k].a+=t;
                    hum1.joint[k].x = hum1.joint[k-1].x + r1*cos(hum1.joint[k].a);
                    hum1.joint[k].y = hum1.joint[k-1].y + r1*sin(hum1.joint[k].a);
                    MoveJoint(k+1,0,0,t);
                  }
                else if ((r1*sqrt(2)+dy<hum1.L[9])&&(r1*sqrt(2)+dy<hum1.L[9])) {hum1.joint[k].y+=dy/sqrt(2); hum1.joint[k+1].y+=dy/sqrt(2);}
              }
          }*/

    } break;
    case 13: {
        t=0;
        float D = R(hum1.joint[4].x,hum1.joint[4].y,hum1.joint[13].y-z1,table1.h1);
        r1 = R(hum1.joint[k].x,hum1.joint[k].y,hum1.joint[k-1].x,hum1.joint[k-1].y);

        if (dx!=0) {
            if (dx>0) {
                if (hum1.joint[k].a<=hum1.joint[k-1].a) MoveJoint(k-1,0,0,-da);
                t=-da;
              }
            if (dx<0) {
                if (hum1.joint[k].a<pi) t=da;
                else {MoveJoint(k-1,0,0,da);}
              }
            //Самый скользкий момент 3
            hum1.joint[9].x = hum1.joint[12].y-z1;
            hum1.joint[9].a = atan((hum1.joint[9].y-hum1.joint[4].y)/(hum1.joint[9].x-hum1.joint[4].x));
            hum1.joint[10].x = hum1.joint[13].y-z1;
            hum1.joint[10].a = -pi + atan((hum1.joint[10].y-hum1.joint[9].y)/(hum1.joint[10].x-hum1.joint[9].x));
          }
        else if (dy!=0) {
            if (dy<0) {
                if ((hum1.joint[k].a==pi/2)&&(R(hum1.joint[11].x,hum1.joint[11].y,hum1.joint[12].x,hum1.joint[12].y)/sqrt(2)-dy<hum1.L[9]/sqrt(2))) MoveJoint(k-1,0,dy,0);
                else {
                    if (hum1.joint[k].a<pi/2) {
                        if (hum1.joint[k].a<=hum1.joint[k-1].a) MoveJoint(k-1,0,0,-da);
                        t=-da;
                      }
                    if ((hum1.joint[k].a>pi/2)&&(hum1.joint[k].a<pi)) t=da;
                  }
              }
            else {
                if ((hum1.joint[k].a==pi/2)&&(D+dy<hum1.L[9]+hum1.L[10])) MoveJoint(k-1,0,dy,0);
                else {
                    if (hum1.joint[k].a+da>=pi/2) {
                        if (hum1.joint[k].a<=hum1.joint[k-1].a) MoveJoint(k-1,0,0,-da);
                        t=-da;
                      }
                    else t=da;
                  }
              }
            //Самый скользкий момент 4
            hum1.joint[9].x = hum1.joint[12].y-z1;
            hum1.joint[9].a = atan((hum1.joint[9].y-hum1.joint[4].y)/(hum1.joint[9].x-hum1.joint[4].x));
            hum1.joint[10].x = hum1.joint[13].y-z1;
            hum1.joint[10].a = -pi + atan((hum1.joint[10].y-hum1.joint[9].y)/(hum1.joint[10].x-hum1.joint[9].x));
          }
        else t = dt;

        hum1.joint[k].a+=t;
        hum1.joint[k].x = hum1.joint[k-1].x + r1*cos(hum1.joint[k].a);
        hum1.joint[k].y = hum1.joint[k-1].y + r1*sin(hum1.joint[k].a);
        /*r1 = R(hum1.joint[k].x,hum1.joint[k].y,hum1.joint[k-1].x,hum1.joint[k-1].y);
        if (dx!=0) {
            if ((dx<0)&&(hum1.joint[k].a<pi)) t=da;
            if ((dx>0)&&(hum1.joint[k].a>hum1.joint[k-1].a)) t=-da;
          }
        else if (dy!=0) {
            if (dy<0) {
                if ((hum1.joint[k].a<pi/2)&&(hum1.joint[k].a>hum1.joint[k-1].a)) t=-da;
                if ((hum1.joint[k].a>pi/2)&&(hum1.joint[k].a<pi)) t=da;
              }
            else {
                if (hum1.joint[k].a<pi/2) t=da;
                if ((hum1.joint[k].a>pi/2)&&(hum1.joint[k].a>hum1.joint[k-1].a)) t=-da;
              }
          }

        hum1.joint[k].a+=t;
        hum1.joint[k].x = hum1.joint[k-1].x + r1*cos(hum1.joint[k].a);
        hum1.joint[k].y = hum1.joint[k-1].y + r1*sin(hum1.joint[k].a);*/
    } break;
    case 15: {
        t=0;
        float D = R(hum1.joint[4].x,hum1.joint[4].y,hum1.joint[16].y-z1,table1.h1);
        r1 = R(hum1.joint[k].x,hum1.joint[k].y,hum1.joint[k-1].x,hum1.joint[k-1].y);
        float r2 =R(hum1.joint[k].x,hum1.joint[k].y,hum1.joint[k+1].x,hum1.joint[k+1].y);

        if (dx!=0) {
            if ((dx<0)&&(hum1.joint[k].a-da<pi)) t=da;
            if ((dx>0)&&(hum1.joint[k].a+da>pi/6)) {
                if (hum1.joint[k].a<=hum1.joint[k+1].a) MoveJoint(k+1,0,0,-da);
                t=-da;
              }
//            float r2 =R(hum1.joint[k].x,hum1.joint[k].y,hum1.joint[k+1].x,hum1.joint[k+1].y);
            hum1.joint[k].a+=t;
            hum1.joint[k].x = hum1.joint[k-1].x + r1*cos(hum1.joint[k].a);
            hum1.joint[k].y = hum1.joint[k-1].y + r1*sin(hum1.joint[k].a);
            //MoveJoint(k+1,0,0,0);
            hum1.joint[k+1].x = hum1.joint[k].x + r2*cos(hum1.joint[k+1].a);
            hum1.joint[k+1].y = hum1.joint[k].y + r2*sin(hum1.joint[k+1].a);
          }
        else {
            if (dy!=0) {
                if (hum1.joint[k].a!=pi/2) {
                    if (dy>0) {
                        if (hum1.joint[k].a<pi/2) t=da;
                        else if (hum1.joint[k].a>pi/2) t=-da;
                      }
                    else {
                        if ((hum1.joint[k].a<pi/2)&&(hum1.joint[k].a>0)) {
                            if (hum1.joint[k].a<=hum1.joint[k+1].a) MoveJoint(k+1,0,0,da);
                            t=-da;
                          }
                        else if ((hum1.joint[k].a>pi/2)&&(hum1.joint[k].a<pi)) t=da;
                      }
                    hum1.joint[k].a+=t;
                    hum1.joint[k].x = hum1.joint[k-1].x + r1*cos(hum1.joint[k].a);
                    hum1.joint[k].y = hum1.joint[k-1].y + r1*sin(hum1.joint[k].a);
                    //MoveJoint(k+1,0,0,0);
                    hum1.joint[k+1].x = hum1.joint[k].x + r2*cos(hum1.joint[k+1].a);
                    hum1.joint[k+1].y = hum1.joint[k].y + r2*sin(hum1.joint[k+1].a);
                  }
                else {
                    if ((dy>0)&&(D+dy<hum1.L[9]+hum1.L[10])) {
                        hum1.joint[k].y+=dy/sqrt(2);
                        hum1.joint[k+1].y+=dy/sqrt(2);
                      }
                    else if ((dy<0)&&((r1-dy<hum1.L[9]/sqrt(2))||(hum1.joint[k].y>hum1.joint[k-1].y))){
                        hum1.joint[k].y+=dy/sqrt(2);
                        hum1.joint[k+1].y+=dy/sqrt(2);
                      }
                  }
              }
            else if ((hum1.joint[k].a+dt<pi)&&(hum1.joint[k].a>pi/6)) {
                hum1.joint[k].a+=dt;
                hum1.joint[k].x = hum1.joint[k-1].x + r1*cos(hum1.joint[k].a);
                hum1.joint[k].y = hum1.joint[k-1].y + r1*sin(hum1.joint[k].a);
//                hum1.joint[k+1].x = hum1.joint[k].x + r2*cos(hum1.joint[k+1].a);
//                hum1.joint[k+1].y = hum1.joint[k].y + r2*sin(hum1.joint[k+1].a);
              }
          }
    } break;
    case 16: {
        t=0;
        float D = R(hum1.joint[4].x,hum1.joint[4].y,hum1.joint[16].y-z1,table1.h1);
        r1 = R(hum1.joint[k].x,hum1.joint[k].y,hum1.joint[k-1].x,hum1.joint[k-1].y);
        if (dx!=0) {
            if (dx<0) {
                if (hum1.joint[k].a>=hum1.joint[k-1].a) MoveJoint(k-1,0,0,da);
                t=da;
              }
            if (dx>0) {
                if (hum1.joint[k].a>0) t=-da;
                else {MoveJoint(k-1,0,0,-da);}
              }
          }
        else if (dy!=0) {
            if (dy<0) {
                if ((hum1.joint[k].a==pi/2)&&(R(hum1.joint[14].x,hum1.joint[14].y,hum1.joint[15].x,hum1.joint[15].y)/sqrt(2)-dy<hum1.L[9]/sqrt(2))) MoveJoint(k-1,0,dy,0);
                else {
                    if ((hum1.joint[k].a<pi/2)&&(hum1.joint[k].a>0)) t=-da;
                    if (hum1.joint[k].a>pi/2){
                        if (hum1.joint[k].a>=hum1.joint[k-1].a) MoveJoint(k-1,0,0,da);
                        t=da;
                      }
                  }
              }
            else {
                if ((hum1.joint[k].a==pi/2)&&(D+dy<hum1.L[9]+hum1.L[10])) MoveJoint(k-1,0,dy,0);
                else {
                    if (hum1.joint[k].a+da<=pi/2) {
                        if (hum1.joint[k].a>=hum1.joint[k-1].a) MoveJoint(k-1,0,0,da);
                        t=da;
                      }
                    else t=-da;
                  }
              }
          }
        else t = dt;

        hum1.joint[k].a+=t;
        hum1.joint[k].x = hum1.joint[k-1].x + r1*cos(hum1.joint[k].a);
        hum1.joint[k].y = hum1.joint[k-1].y + r1*sin(hum1.joint[k].a);
    } break;
    case 17: {
        if (hum1.joint[k].f) {
            hum1.joint[k].f = false;
            MoveJoint(6,dy,0,0);
            hum1.joint[k].f = true;
          }
        else hum1.joint[17].y = z1+hum1.joint[7].x;
    } break;

  }
}

void tScene::initializeGL()
{
   qglClearColor(Qt::black);
   glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_FLAT);
   glEnable(GL_CULL_FACE);

   Calc();
   Calc1();
   //Calc1(true);
}

void tScene::resizeGL(int nWidth, int nHeight)
{
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   GLfloat ratio=(GLfloat)nHeight/(GLfloat)nWidth;

   if (nWidth>=nHeight)
      glOrtho(-1.0/ratio, 1.0/ratio, -1.0, 1.0, -10.0, 1.0);
   else
      glOrtho(-1.0, 1.0, -1.0*ratio, 1.0*ratio, -10.0, 1.0);

   glViewport(0, 0, (GLint)nWidth, (GLint)nHeight);
}

void tScene::printAll(float x, float y, int size)
{
  glColor3f(1.0f,1.0f,1.0f);
  QFont fnt;
  fnt.setPointSize(size);
  fnt.setUnderline(true);

  float y1=y;
  renderText(x,y,"Table",fnt);
  fnt.setUnderline(false);
  y1+=size+2;
  renderText(x,y1," - height:    "+QString::number(table1.h1),fnt);
  y1+=size+2;
  renderText(x,y1," - width 1:     "+QString::number(table1.w1),fnt);
  y1+=size+2;
  renderText(x,y1," - width 2:     "+QString::number(table1.w2),fnt);

  fnt.setUnderline(true);
  y1=y; x+=180;
  renderText(x,y1,"Pedal",fnt);
  fnt.setUnderline(false);
  y1+=size+2;
  renderText(x,y1," - height:    "+QString::number(table1.pedal.h5),fnt);
  y1+=size+2;
  renderText(x,y1," - offset:    "+QString::number(table1.pedal.h4),fnt);

  fnt.setUnderline(true);
  y1=y; x+=150;
  renderText(x,y1,"Chair",fnt);
  fnt.setUnderline(false);
  y1+=size+2;
  renderText(x,y1," - height:    "+QString::number(chair1.h),fnt);
  y1+=size+2;
  renderText(x,y1," - distance:  "+QString::number(table1.x-table1.w2-chair1.x),fnt);

  fnt.setUnderline(true);
  y1=y; x+=150;
  renderText(x,y1,"Seamstress",fnt);
  fnt.setUnderline(false);
  y1+=size+2;
  renderText(x,y1," - stature:   "+QString::number(hum1.h),fnt);
}

void tScene::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glTranslatef(0.5f,-0.75f,0.0f);
  glScalef(nSca,nSca,nSca);

//Стол
  glLineWidth(3.0f);
  glColor4f(0.0f,0.0f,1.0f,1.0f);
  glBegin(GL_LINES); //нижняя перекладина
    glVertex2f(table1.x,0);
    glVertex2f(table1.x,8);

    glVertex2f(table1.x-table1.w2,0);
    glVertex2f(table1.x-table1.w2,8);

    glVertex2f(table1.x,8);
    glVertex2f(table1.x-table1.w2,8);

    glVertex2f(table1.x,0);
    glVertex2f(table1.x-table1.w2,0);
  glEnd();
  glBegin(GL_LINES); //вертикальная стойка
    glVertex2f(table1.x-20,8);
    glVertex2f(table1.x-20,table1.h1-4);

    glVertex2f(table1.x-table1.Sh-20,8);
    glVertex2f(table1.x-table1.Sh-20,table1.h1-4);
  glEnd();
  glBegin(GL_LINES); //крышка
    glVertex2f(table1.x,table1.h1);
    glVertex2f(table1.x,table1.h1-4);

    glVertex2f(table1.x,table1.h1);
    glVertex2f(table1.x-table1.w1,table1.h1);

    glVertex2f(table1.x-table1.w1,table1.h1);
    glVertex2f(table1.x-table1.w1,table1.h1-4);

    glVertex2f(table1.x-table1.w1,table1.h1-4);
    glVertex2f(table1.x,table1.h1-4);
  glEnd();
  glBegin(GL_LINES); //машинка
    glVertex2f(table1.x-table1.w1+table1.d,table1.h1);
    glVertex2f(table1.x-table1.w1+table1.d,table1.h1+30);

    glVertex2f(table1.x-table1.w1+table1.d,table1.h1+30);
    glVertex2f(table1.x-table1.w1+table1.d+10,table1.h1+30);

    glVertex2f(table1.x-table1.w1+table1.d+20,table1.h1);
    glVertex2f(table1.x-table1.w1+table1.d+10,table1.h1+30);
  glEnd();
  glColor4f(0.0f,1.0f,0.0f,1.0f);//педаль
  glLineWidth(2.0f);
  glBegin(GL_LINES);
    glVertex2f(table1.x-table1.w2+table1.pedal.h4,table1.pedal.h5);
    glVertex2f(table1.x-table1.w2+table1.pedal.h4-11,table1.pedal.h5);

    glVertex2f(table1.x-table1.w2+table1.pedal.h4,table1.pedal.h5);
    glVertex2f(table1.x-table1.w2+table1.pedal.h4+8,table1.pedal.h6);

    glVertex2f(table1.x-table1.w2+table1.pedal.h4-11,table1.pedal.h5);
    glVertex2f(table1.x-table1.w2+table1.pedal.h4+8,table1.pedal.h6);
  glEnd();

//Стул
  glLineWidth(5.0f);
  glColor4f(0.0f,0.0f,1.0f,1.0f);
  glBegin(GL_LINES);
    glVertex2f(chair1.x,0);
    glVertex2f(chair1.x,chair1.h);
  glEnd();
  glBegin(GL_LINES);
    glVertex2f(chair1.x-10,0);
    glVertex2f(chair1.x,10);
  glEnd();
  glBegin(GL_LINES);
    glVertex2f(chair1.x,10);
    glVertex2f(chair1.x+10,0);
  glEnd();
  glBegin(GL_LINES);
    glVertex2f(chair1.x-chair1.w,chair1.h);
    glVertex2f(chair1.x+chair1.w,chair1.h);
  glEnd();

//Швея
  glLineWidth(2.0f);
  if ((Check())||(hum1.broke)) glColor4f(1.0f,0.0f,0.0f,1.0f);
  else glColor4f(0.8f,0.0f,1.0f,1.0f);
  for (int i=1; i<7; i++)//carcass
  {
    glBegin(GL_LINES);
      glVertex2f(hum1.joint[i].x,hum1.joint[i].y);
      glVertex2f(hum1.joint[i+1].x,hum1.joint[i+1].y);
    glEnd();
  }
  glBegin(GL_LINES);
    glVertex2f(hum1.joint[6].x,hum1.joint[6].y);
    glVertex2f(hum1.joint[8].x,hum1.joint[8].y);
  glEnd();
  glBegin(GL_LINES);
    glVertex2f(hum1.joint[4].x,hum1.joint[4].y);
    glVertex2f(hum1.joint[9].x,hum1.joint[9].y);
  glEnd();
  glBegin(GL_LINES);
    glVertex2f(hum1.joint[9].x,hum1.joint[9].y);
    glVertex2f(hum1.joint[10].x,hum1.joint[10].y);
  glEnd();
  glBegin(GL_LINES);//foot
    glVertex2f(hum1.joint[1].x,hum1.joint[1].y);
    glVertex2f(table1.x-table1.w2+table1.pedal.h4-11,table1.pedal.h5);

    glVertex2f(hum1.joint[1].x,hum1.joint[1].y);
    glVertex2f(table1.x-table1.w2+table1.pedal.h4+8,table1.pedal.h6);

    glVertex2f(table1.x-table1.w2+table1.pedal.h4+8,table1.pedal.h6);
    glVertex2f(table1.x-table1.w2+table1.pedal.h4-11,table1.pedal.h5);
  glEnd();
  glBegin(GL_LINES);//hand
    glVertex2f(hum1.joint[10].x,hum1.joint[10].y);
    glVertex2f(hum1.joint[10].x+12,hum1.joint[10].y);

    glVertex2f(hum1.joint[10].x,hum1.joint[10].y);
    glVertex2f(hum1.joint[10].x+5,hum1.joint[10].y+3);

    glVertex2f(hum1.joint[10].x+5,hum1.joint[10].y+3);
    glVertex2f(hum1.joint[10].x+12,hum1.joint[10].y);
  glEnd();
  glPointSize(8.0f);//joints
  for (int i=1; i<=17; i++){
    if (hum1.joint[i].f) glColor4f(1.0f,1.0f,0.5f,1.0f);
    else glColor4f(0.8f,0.5f,0.0f,1.0f);
    glBegin(GL_POINTS);
      glVertex2f(hum1.joint[i].x,hum1.joint[i].y);
    glEnd();
  }

  //================================ Вид сверху
  //Стол
  glColor3f(0.0f,0.0f,1.0f);
  glBegin(GL_LINES);
    glVertex2f(z0-table1.w/2, z1+table1.x);
    glVertex2f(z0-table1.w/2, z1+table1.x-table1.w1);

    glVertex2f(z0-table1.w/2, z1+table1.x);
    glVertex2f(z0+table1.w/2, z1+table1.x);

    glVertex2f(z0+table1.w/2, z1+table1.x-table1.w1);
    glVertex2f(z0+table1.w/2, z1+table1.x);

    glVertex2f(z0+table1.w/2, z1+table1.x-table1.w1);
    glVertex2f(z0-table1.w/2, z1+table1.x-table1.w1);
  glEnd();
  glBegin(GL_LINES); //машинка
    glVertex2f(hum1.joint[17].x, z1+table1.x-table1.w1+table1.d);
    glVertex2f(hum1.joint[17].x, z1+table1.x-table1.w1+table1.d+20);

    glVertex2f(hum1.joint[17].x, z1+table1.x-table1.w1+table1.d+20);
    glVertex2f(hum1.joint[17].x+30, z1+table1.x-table1.w1+table1.d+20);

    glVertex2f(hum1.joint[17].x+30, z1+table1.x-table1.w1+table1.d+20);
    glVertex2f(hum1.joint[17].x+30, z1+table1.x-table1.w1+table1.d);

    glVertex2f(hum1.joint[17].x+30, z1+table1.x-table1.w1+table1.d);
    glVertex2f(hum1.joint[17].x, z1+table1.x-table1.w1+table1.d);
  glEnd();

  //Стул
  Circle(z0,z1+chair1.x,chair1.w);

  //Швея
  glLineWidth(2.0f);
  if ((Check())||(hum1.broke)) glColor4f(1.0f,0.0f,0.0f,1.0f);
  else glColor4f(0.8f,0.0f,1.0f,1.0f);
  //carcass
  for (int i=11; i<13; i++)//right
  {
    glBegin(GL_LINES);
      glVertex2f(hum1.joint[i].x,hum1.joint[i].y);
      glVertex2f(hum1.joint[i+1].x,hum1.joint[i+1].y);
    glEnd();
  }
  for (int i=14; i<16; i++)//left
  {
    glBegin(GL_LINES);
      glVertex2f(hum1.joint[i].x,hum1.joint[i].y);
      glVertex2f(hum1.joint[i+1].x,hum1.joint[i+1].y);
    glEnd();
  }
  glBegin(GL_LINES);
    glVertex2f(hum1.joint[11].x,hum1.joint[11].y);
    glVertex2f(hum1.joint[14].x,hum1.joint[14].y);

    glVertex2f(hum1.joint[17].x,hum1.joint[17].y);
    glVertex2f(hum1.joint[17].x,z1+hum1.joint[3].x);
  glEnd();
  //hands
  float r1 = 5, f = atan((hum1.joint[16].y-hum1.joint[15].y)/(hum1.joint[16].x-hum1.joint[15].x));
  if (f<0) f = pi+f;
  Circle(hum1.joint[16].x+r1*cos(f),hum1.joint[16].y+r1*sin(f),r1);
  f = atan((hum1.joint[13].y-hum1.joint[12].y)/(hum1.joint[13].x-hum1.joint[12].x));
  if (f<0) f = pi+f;
  Circle(hum1.joint[13].x+r1*cos(f),hum1.joint[13].y+r1*sin(f),r1);

  //вывод данных
  printAll();

// для тех.информации
  glColor3f(1.0f,1.0f,1.0f);
  QFont fnt;
  fnt.setPointSize(14);

  //for (int i=4; i<=16; i++) if ((i<11)||(i>14))renderText(10,100+(i-4)*16,QString::number(F[12].a),fnt);
  //renderText(10,120,QString::number(hum1.joint[13].x)+"   "+QString::number(hum1.joint[13].y)+"   "+QString::number(hum1.joint[13].a),fnt);
  //renderText(10,120,QString::number(hum1.joint[15].a),fnt);
  //renderText(10,140,QString::number(hum1.joint[9].x)+"   "+QString::number(hum1.joint[9].y)+"   "+QString::number(hum1.joint[9].a),fnt);
  //renderText(10,160,QString::number(hum1.joint[10].x)+"   "+QString::number(hum1.joint[10].y)+"   "+QString::number(hum1.joint[10].a),fnt);
  //renderText(10,180,QString::number(hum1.joint[15].x)+"   "+QString::number(hum1.joint[15].y)+"   "+QString::number(hum1.joint[15].a),fnt);
  //renderText(10,200,QString::number(hum1.joint[16].x)+"   "+QString::number(hum1.joint[16].y)+"   "+QString::number(hum1.joint[16].a),fnt);

}

void tScene::Move(int mode)
{
  float dx[18], dy[18], M[18], L[18];
  bool F[18];
  switch(mode){
    case 1:{
/*      hum1.joint[9].x = -19.6938; hum1.joint[9].y = 83.6674; hum1.joint[9].a = -1.54507;
      hum1.joint[10].x = 8.1888; hum1.joint[10].y = 80; hum1.joint[10].a = -3.25092;
      hum1.joint[12].x = -99.375; hum1.joint[12].y = 35.6908; hum1.joint[12].a = 1.5708;
      hum1.joint[13].x = -115.721; hum1.joint[13].y = 58.1888; hum1.joint[13].a = 2.19912;
      hum1.joint[15].x = -140.625; hum1.joint[15].y = 18.0131; hum1.joint[15].a = 1.5708;
      hum1.joint[16].x = -124.279; hum1.joint[16].y = 40.5113; hum1.joint[16].a = 0.942481;*/
      for (int i=4; i<=17; i++) {
        sBeg.joint[i].x = hum1.joint[i].x;
        sBeg.joint[i].y = hum1.joint[i].y;
        sBeg.joint[i].a = hum1.joint[i].a;
      }
    } break;
    case 2: {
        for (int i=4; i<=10; i++) M[i]=L[i] = R(sBeg.joint[i].x,sBeg.joint[i].y,sEnd.joint[i].x,sEnd.joint[i].y);
        for (int i=15; i<=16; i++) M[i-4]=L[i] = R(sBeg.joint[i].x,sBeg.joint[i].y,sEnd.joint[i].x,sEnd.joint[i].y);

        float tmp;
        for (int i=4; i<12; i++)
          for (int j=i+1; j<=12; j++)
            if (M[i]<M[j]) {
              tmp = M[i];
              M[i] = M[j];
              M[j] = tmp;
            }
        for (int i=4; i<=10; i++) { //горизонтальное приращение для суставов в профиль
          dx[i] = L[i]/M[4];
          if (sBeg.joint[i].x>sEnd.joint[i].x) dx[i] = -dx[i];
        }
        for (int i=15; i<=16; i++) { //приращение для суставов сверху
          dy[i]=L[i]/M[4];
          //if (sBeg.joint[i].x<sEnd.joint[i].x) dx[i] = 1.0f;
          //else dx[i] = -1.0f;
          if (sBeg.joint[i].y>sEnd.joint[i].y) dy[i] = -dy[i];
        }

        //начальное положение
        for (int i=4; i<=17; i++) {
          hum1.joint[i].x = sBeg.joint[i].x;
          hum1.joint[i].y = sBeg.joint[i].y;
          hum1.joint[i].a = sBeg.joint[i].a;
        }

        // движение
        bool F1 = true;
        while (/*(hum1.joint[9].x<sEnd.joint[9].x)&&*/(F1)) {
            for (int i=4; i<=10; i++)
                if (((dx[i]>0)&&(hum1.joint[i].x<sEnd.joint[i].x))||((dx[i]<0)&&(hum1.joint[i].x>sEnd.joint[i].x)))
                {
                  F[i] = true;
                  MoveJoint(i,dx[i],0,0);
                }
                else F[i] = false;
            for (int i=15; i<=16; i++)
              if (((dx[i]>0)&&(hum1.joint[i].x<sEnd.joint[i].x))||((dx[i]<0)&&(hum1.joint[i].x>sEnd.joint[i].x)))
              {
                F[i] = true;
                MoveJoint(i,dx[i],0,0);
              }
              else F[i] = false;
            if (((dy[15]>0)&&(hum1.joint[15].y<sEnd.joint[15].y))||((dy[15]<0)&&(hum1.joint[15].y>sEnd.joint[15].y)))
            {
                F[15] = true;
                MoveJoint(15,0,dy[15],0);
            }
            else F[15] = false;

            F1 = false;
            for (int i=4; i<=10; i++) if (F[i]) F1 = true;
            for (int i=15; i<=16; i++) if (F[i]) F1 = true;

            Sleep(10);
            updateGL();
          }
    } break;
    case 3:{
/*      hum1.joint[9].x = 2.58198; hum1.joint[9].y = 93.186; hum1.joint[9].a = -0.743249;
      hum1.joint[10].x = 27.3395; hum1.joint[10].y = 80; hum1.joint[10].a = -3.63099;
      hum1.joint[12].x = -99.375; hum1.joint[12].y = 52.582; hum1.joint[12].a = 1.5708;
      hum1.joint[13].x = -115.721; hum1.joint[13].y = 77.3395; hum1.joint[13].a = 2.19912;

      hum1.joint[15].x = -140.625;
      hum1.joint[15].y = 18.0131+22.27578;
      hum1.joint[15].a = 1.5708;

      hum1.joint[16].x = -124.279;
      hum1.joint[16].y = 40.5113+19.1507;
      hum1.joint[16].a = 0.942481;*/
      for (int i=4; i<=17; i++) {
        sEnd.joint[i].x = hum1.joint[i].x;
        sEnd.joint[i].y = hum1.joint[i].y;
        sEnd.joint[i].a = hum1.joint[i].a;
      }
    } break;
  }
}

void tScene::mouseMoveEvent(QMouseEvent *pe)
{
}

void tScene::mousePressEvent(QMouseEvent* pe)
{
}

void tScene::mouseReleaseEvent(QMouseEvent* pe)
{
}

void tScene::keyPressEvent(QKeyEvent* pe)
{
  switch (pe->key()){
    case Qt::Key_Tab:{
        if (num>0) hum1.joint[num].f=false;
        if (num<17) num++;
        else num=1;
        hum1.joint[num].f=true;
        carcass=true;
        updateGL();
    } break;
    case Qt::Key_Backspace:{
        if (num>0) hum1.joint[num].f=false;
        if (num>1) num--;
        else num=17;
        hum1.joint[num].f=true;
        carcass=true;
        updateGL();
    } break;
    case Qt::Key_Delete:{
      if (num>0) {
        hum1.joint[num].f=false;
        num=0;
      }
      carcass=false;
      updateGL();
    } break;
    case Qt::Key_Escape:
      this->close();
      break;
    case Qt::Key_Space:
      defaultScene();
      break;
    case Qt::Key_Plus:{
      if (hum1.h<200) hum1.h+=1;
      //Calc();
      Calc1(true);
      if (R(hum1.joint[4].x,hum1.joint[4].y,hum1.joint[10].x,hum1.joint[10].y)>hum1.L[9]+hum1.L[10]) hum1.broke = true; else hum1.broke = false;
      updateGL();
    } break;
    case Qt::Key_Minus:{
      if (hum1.h>140) hum1.h-=1;
      //Calc();
      Calc1(true);
      if (R(hum1.joint[4].x,hum1.joint[4].y,hum1.joint[10].x,hum1.joint[10].y)>hum1.L[9]+hum1.L[10]) {MoveJoint(10,-1,0,0); hum1.broke = true;} else hum1.broke = false;
      updateGL();
    } break;
    case Qt::Key_Down:{
      if (carcass) {if ((num>3)&&(num!=10)) MoveJoint(num,0,-1,0);}
      else if (chair1.h>chair1.Hmin) {chair1.h-=1; MoveJoint(3,0,-1,0);}
      updateGL();
    } break;
    case Qt::Key_Up:{
      if (carcass) {if ((num>3)&&(num!=10)) MoveJoint(num,0,1,0);}
      else if (chair1.h<chair1.Hmax) {chair1.h+=1; MoveJoint(3,0,1,0);}
      updateGL();
    } break;
    case Qt::Key_Left:{
      if (carcass)
        {
          if ((num>2)) MoveJoint(num,-1,0,0);
          else if ((num==1)&&(table1.pedal.h4>table1.pedal.minh4)) {table1.pedal.h4-=0.5; MoveJoint(1,-0.5,0,0);}
        }
      else if (chair1.x>table1.x-table1.w1-70+chair1.w) {chair1.x-=1; MoveJoint(3,-1,0,0);}
      updateGL();
    } break;
    case Qt::Key_Right:{
      if (carcass)
        {
          if ((num>2)) MoveJoint(num,1,0,0);
          else if ((num==1)&&(table1.pedal.h4<table1.pedal.l)) {table1.pedal.h4+=0.5; MoveJoint(1,0.5,0,0);}
        }
      else if (chair1.x<table1.x-table1.w1-5) {chair1.x+=1; MoveJoint(3,1,0,0);}
      updateGL();
    } break;
    //метаморфозы стола
    case Qt::Key_A:{//выше
      if (table1.h1<table1.Hmax) {
          table1.h1+=1;
          if (hum1.joint[10].y<table1.y+table1.h1) MoveJoint(10,0,1,0);
        }
      updateGL();
    } break;
    case Qt::Key_S:{//длиннее
      if (table1.h2<35) table1.h2+=0.5;
      Calc();
      updateGL();
    } break;
    case Qt::Key_D:{
      if (table1.h3<30) {table1.h3+=0.5; MoveJoint(1,-0.5,0,0);}
      Calc();
      updateGL();
    } break;
    case Qt::Key_Z:{//ниже
      if (table1.h1>table1.Hmin) {
          table1.h1-=1;
          if (R(hum1.joint[4].x,hum1.joint[4].y,hum1.joint[10].x,hum1.joint[10].y-1)<hum1.L[10]+hum1.L[9]) MoveJoint(10,0,-1,0);
        }
      Calc();
      updateGL();
    } break;
    case Qt::Key_X:{//длиннее
      if (table1.h2>19) table1.h2-=0.5;
      Calc();
      updateGL();
    } break;
    case Qt::Key_C:{
      if (table1.h3>24) {table1.h3-=0.5; MoveJoint(1,0.5,0,0);}
      Calc();
      updateGL();
    } break;
    //движение педали
    case Qt::Key_F:{
      if (table1.pedal.h4>table1.pedal.minh4) {table1.pedal.h4-=0.5; MoveJoint(1,-0.5,0,0);}
      Calc();
      updateGL();
    } break;
    case Qt::Key_G:{
      if (table1.pedal.h4<table1.pedal.l) {table1.pedal.h4+=0.5; MoveJoint(1,0.5,0,0);}
      Calc();
      updateGL();
    } break;
    case Qt::Key_V:{
      if (table1.pedal.h5>2) {table1.pedal.h5-=0.5; MoveJoint(1,0,-0.5,0);}
      Calc();
      updateGL();
    } break;
    case Qt::Key_B:{
      if (table1.pedal.h5<12) {table1.pedal.h5+=0.5; MoveJoint(1,0,0.5,0);}
      Calc();
      updateGL();
    } break;
    //движение скелета
    case Qt::Key_1:{ //исходное положение
      hum1.FlagM = true;
      Move(1);
      /*hum1.joint[9].x = -19.6938; hum1.joint[9].y = 83.6674; hum1.joint[9].a = -1.54507;
      hum1.joint[10].x = 8.1888; hum1.joint[10].y = 80; hum1.joint[10].a = -3.25092;
      hum1.joint[12].x = -99.375; hum1.joint[12].y = 35.6908; hum1.joint[12].a = 1.5708;
      hum1.joint[13].x = -115.721; hum1.joint[13].y = 58.1888; hum1.joint[13].a = 2.19912;
      hum1.joint[15].x = -140.625; hum1.joint[15].y = 18.0131; hum1.joint[15].a = 1.5708;
      hum1.joint[16].x = -124.279; hum1.joint[16].y = 40.5113; hum1.joint[16].a = 0.942481;*/
      updateGL();
    } break;
    case Qt::Key_2:{ //непосредственно Ъ
      hum1.FlagM = true;
//      Move(1);
      Move(2);
      updateGL();
    } break;
    case Qt::Key_3:{ //конечное положение
      hum1.FlagM = true;
      Move(3);
      /*hum1.joint[9].x = 2.58198; hum1.joint[9].y = 93.186; hum1.joint[9].a = -0.743249;
      hum1.joint[10].x = 27.3395; hum1.joint[10].y = 80; hum1.joint[10].a = -3.63099;
      hum1.joint[12].x = -99.375; hum1.joint[12].y = 52.582; hum1.joint[12].a = 1.5708;
      hum1.joint[13].x = -115.721; hum1.joint[13].y = 77.3395; hum1.joint[13].a = 2.19912;
      hum1.joint[15].x = 0; hum1.joint[15].y = 0; hum1.joint[15].a = 0;
      hum1.joint[16].x = 0; hum1.joint[16].y = 0; hum1.joint[16].a = 0;*/
      updateGL();
    } break;
    case Qt::Key_0:{//выход из режима
      hum1.FlagM = false;
    }break;

  }
}

/*void tScene::BuildFont()
{
  HFONT font;
  base = glGenLists(96);
  font = CreateFont(-24, 0, 0, 0,FW_BOLD, false,false,false,ANSI_CHARSET, OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,FF_DONTCARE|DEFAULT_PITCH,"Courier New");
  SelectObject(hDC, font);
  wglUseFontBitmaps(hDC, 32, 96, base);
}

void tScene::KillFont()
{
  glDeleteLists(base,96);
}

*/
