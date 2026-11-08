#include "game_widget.h"

#include <QKeyEvent>
#include <QPainter>

#ifdef Q_OS_WASM
#include <emscripten/val.h>

/// In the browser, Qt only receives key presses if its window element
/// has the browser's keyboard focus, which clicking the canvas does not give
void focus_browser_window()
{
  using emscripten::val;
  const val container = val::global("document").call<val>(
    "getElementById", std::string("qt-shadow-container"));
  if (container.isNull() || container["shadowRoot"].isNull()) return;
  const val input = container["shadowRoot"].call<val>(
    "querySelector", std::string(".qt-window-input-element"));
  if (input.isNull()) return;
  input.call<void>("focus");
}
#endif

game_widget::game_widget(QWidget *parent)
  : QWidget(parent),
    m_ball_x(m_width / 2),
    m_ball_dx(1),
    m_ball_r((m_height / 2) - 1),
    m_player_x(m_width - m_player_sz)
{
  setFixedSize(m_width, m_height);
  setFocusPolicy(Qt::StrongFocus);
  setWindowTitle("Lonelier Pong");
  connect(&m_timer, &QTimer::timeout, this, &game_widget::tick);
  m_timer.start(1000 / 60); // Processing's default frame rate
}

void game_widget::tick()
{
  m_ball_x += m_ball_dx;
  if (m_ball_x - 2 == m_ball_r) {
    if (m_player_x < m_ball_r) {
      m_ball_dx = -m_ball_dx;
    } else {
      m_ball_x = m_width / 2;
      m_player_x = m_player_sz;
    }
  }
  if (m_ball_x + m_player_sz == m_width - m_ball_r) {
    if (m_player_x > m_width - m_ball_r) {
      m_ball_dx = -m_ball_dx;
    } else {
      m_ball_x = m_width / 2;
      m_player_x = m_width - m_player_sz;
    }
  }
  update();
}

void game_widget::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Left) {
    m_player_x = m_player_sz;
  } else if (event->key() == Qt::Key_Right) {
    m_player_x = m_width - m_player_sz;
  } else {
    QWidget::keyPressEvent(event);
  }
}

void game_widget::mousePressEvent(QMouseEvent *event)
{
#ifdef Q_OS_WASM
  // Wait until the browser has finished handling the click,
  // as that moves the focus to the page body
  QTimer::singleShot(50, this, focus_browser_window);
#endif
  QWidget::mousePressEvent(event);
}

void game_widget::paintEvent(QPaintEvent *)
{
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.fillRect(rect(), Qt::white);
  painter.setPen(QPen(Qt::black, 2));
  painter.setBrush(Qt::white);
  painter.drawEllipse(QPoint(m_ball_x, m_height / 2), m_ball_r, m_ball_r);
  // Processing's rectMode(CENTER): a 2 x 100 paddle centered at player_x
  painter.drawRect(QRect(m_player_x - 1, 0, 2, m_height));
}
