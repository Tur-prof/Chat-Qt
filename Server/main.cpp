#include <QApplication>
#include <QtDebug>

#include "StartScreen.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  auto startScreen = new StartScreen;
  startScreen->setAttribute(Qt::WA_DeleteOnClose);
  startScreen->show();
  return a.exec();
}
