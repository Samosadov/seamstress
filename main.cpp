#include <QApplication>
#include "scene.h"

int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  tScene scene1;
  scene1.setWindowTitle("Seamstress v.0.1");
  scene1.resize(640,480);
  scene1.show();

  return app.exec();
}
