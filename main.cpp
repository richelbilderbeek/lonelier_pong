#include "game_widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  game_widget w;
#ifdef Q_OS_WASM
  w.showFullScreen();
#else
  w.show();
#endif
  w.setFocus();
  return a.exec();
}
