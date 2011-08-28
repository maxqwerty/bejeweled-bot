#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"

#include <QList>
#include <QLabel>
#include <QGridLayout>
#include <QPixmap>
#include <QImage>
#include <QColor>
#include <QTimer>

class Helper;

//[----------------------------------------------------------------------------]

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:

  enum GemType
  {
    Undefined,
    White,
    Yellow,
    Green,
    Red,
    Magenta,
    Blue,
    Brown,
    Universal
  };

  explicit MainWindow(QWidget* parent = 0);
  ~MainWindow();

protected:
  void closeEvent(QCloseEvent* e);

  void mouseReleaseEvent(QMouseEvent* e);
  void mouseMoveEvent(QMouseEvent* e);

  bool eventFilter(QObject* obj, QEvent* event);

private:

  GemType guessGemType(QRgb color);
  void    extractSingleColor(QList<int>& field, GemType gem);

  void    captureWindow(WId hW);
  void    capturePixels();
  void    findAndExecuteMove();
  void    doTurn(QPoint from, QPoint to);
  void    sendMouseLClick(WId hWnd, int x, int y);

  void    showHelper(int x_src, int y_src, int x_dst, int y_dst);
  void    hideHelper();

  WId     lookupWindow();
  QString getWindowClassName(WId hwnd);
  void    setupClientOffsets(WId hwnd);

  void    loadConfig();
  void    saveConfig();

private slots:
  void on_actionAuto_play_triggered(bool on);
  void on_actionStay_on_top_triggered(bool on);
  void on_actionAbout_triggered();

  void on_tbtnRefresh_clicked();

  void on_chkThumbnail_stateChanged(int state);

  void timer();

  void dumpField();

  void startStop();

private:
  Ui::MainWindow ui;

  WId            m_hWnd;
  Helper*        m_helper;

  QPixmap        m_pixmap;
  QImage         m_image;
  QList<QRgb>    m_colors;

  QTimer         m_timer;
  bool           m_isActive;
  bool           m_findWindowMode;

  int            m_clientOfsX;
  int            m_clientOfsY;
};

//[----------------------------------------------------------------------------]

#endif // MAINWINDOW_H

//[----------------------------------------------------------------------------]
