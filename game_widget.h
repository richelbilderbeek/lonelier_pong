#ifndef GAME_WIDGET_H
#define GAME_WIDGET_H

#include <QTimer>
#include <QWidget>

/// Lonelier Pong, ported from the Processing sketch 'lonelier_pong.pde'
class game_widget : public QWidget
{
  Q_OBJECT

public:
  explicit game_widget(QWidget *parent = nullptr);

protected:
  void keyPressEvent(QKeyEvent *event) override;
  void paintEvent(QPaintEvent *event) override;

private:
  void tick();

  static constexpr int m_width = 600;
  static constexpr int m_height = 100;
  static constexpr int m_player_sz = 2;

  int m_ball_x;
  int m_ball_dx;
  int m_ball_r;
  int m_player_x;
  QTimer m_timer;
};

#endif // GAME_WIDGET_H
