#ifndef SCENE_H
#define SCENE_H

#include <QGLWidget>
//#include <string>

typedef struct {
    const static float minh4 = 7.5;
    float h4,h5,h6,l;
} tPedal;

typedef struct {
    const static float Hmin = 67;
    const static float Hmax = 84;
    const static float Sh = 11.5;
    float x, y, w1, w2;
    float h1, h2, h3, w;
    float d;
    tPedal pedal;
} tTable;

typedef struct {
    const static float w = 20, Hmin = 45, Hmax = 65;
    float h, x;
} tChair;

typedef struct {
    float x, y, z, a, o;
    bool f;
} tJoint;

typedef struct {
  tJoint joint[27];
} tcur;

typedef struct {
    float h;
    tJoint joint[27];
    float L[13];
    bool broke;
    bool FlagM;
} seamstress;

class tScene : public QGLWidget
{
private:
  GLfloat nSca;

  QPoint ptrMousePosition;

  void Circle(float x0, float y0, float r);
  void Calc();
  void Calc1(bool f = false);
  float R(float x1, float y1, float x2, float y2);  //расстояние между точками
  bool Check();
  void defaultScene(); //сброс сцены
  void MoveJoint(int k, float dx, float dy, float dt);  //движение сустава
  void printAll(float x = 10.0f, float y = 15.0f, int size = 14);  //вывод статистики
  void Move(int mode);  //анимированное движение скелета
protected:
  void initializeGL();
  void resizeGL(int nWidth, int nHeight);
  void paintGL();
  void mousePressEvent(QMouseEvent* pe);
  void mouseMoveEvent(QMouseEvent* pe);
  void mouseReleaseEvent(QMouseEvent* pe);
  void keyPressEvent(QKeyEvent* pe);

public:
  tScene(QWidget* parent = 0);
};
#endif // SCENE_H
