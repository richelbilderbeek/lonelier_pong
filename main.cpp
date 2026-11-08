#include "game_widget.h"
#include <QApplication>
#include <QDebug>
#include <QKeyEvent>

class key_logger : public QObject
{
public:
  bool eventFilter(QObject *o, QEvent *e) override
  {
    if (e->type() == QEvent::KeyPress) qDebug() << "DEBUG app KeyPress to" << o;
    if (e->type() == QEvent::MouseButtonPress) qDebug() << "DEBUG app MousePress to" << o << "focus" << QApplication::focusWidget();
    return false;
  }
};

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  key_logger logger;
  a.installEventFilter(&logger);
  game_widget w;
#ifdef Q_OS_WASM
  w.showFullScreen();
#else
  w.show();
#endif
  w.setFocus();
  return a.exec();
}
