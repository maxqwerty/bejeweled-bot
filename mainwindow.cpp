#include "mainwindow.h"
#include "solver.h"
#include "helper.h"
#include "globalshortcut/globalshortcutmanager.h"
#include "aboutdlg.h"

#include <QtGui>

#include <QDebug>
#include <QDateTime>
#include <QTextStream>
#include <QFile>

#include <string>

#if defined(Q_WS_WIN)
  #include <windows.h>
#else
  #error Windows only. Sorry.
#endif

namespace {
  // смещение для Chrome : 414x403

  // Размер ячейки поля в пикселах
  const int CELL_W = 40;
  const int CELL_H = 40;

  // Размер игрового поля в пикселах
  const int FIELD_W = 320;
  const int FIELD_H = 320;

  // Смещение в ячейке для анализа цвета точки
  const int PIX_PROBE_X = 19;
  const int PIX_PROBE_Y = 19;

  const char* picTarget    = ":/res/target.png";
  const char* picActive    = ":/res/status.png";
  const char* picInactive  = ":/res/status-offline.png";
  const char* picUnChecked = ":/res/status-busy.png";

  struct RGB
  {
    MainWindow::GemType gem;
    int r;
    int g;
    int b;
  };

  // Таблица соответствий типов кристаллов к захваченным цветовым значениям
  RGB rgb[] = {
    {MainWindow::Blue,       18, 134, 252}, //обычный
    {MainWindow::Blue,       21, 135, 253}, //пламя
    {MainWindow::Blue,       42, 150, 245}, //x3
    {MainWindow::Blue,       26,  95, 155}, //x4
    {MainWindow::Blue,       47, 171, 255}, //крест
    {MainWindow::Blue,       64, 190, 255}, //крест
    {MainWindow::Blue,       68, 199, 255}, //крест
    {MainWindow::Blue,       75, 206, 255}, //крест
    {MainWindow::Blue,       76, 207, 255}, //крест
    {MainWindow::Blue,       77, 208, 255}, //крест
    {MainWindow::Blue,       89, 222, 255}, //крест
    {MainWindow::Blue,       93, 228, 255}, //крест
    {MainWindow::Blue,       96, 232, 255}, //крест
    {MainWindow::Blue,       97, 235, 255}, //крест
    {MainWindow::Blue,      108, 246, 255}, //крест
    {MainWindow::Blue,      113, 253, 255}, //крест

    {MainWindow::Green,      84, 254, 136}, //обычный
    {MainWindow::Green,      85, 254, 134}, //пламя
    {MainWindow::Green,       0, 188,  12}, //x2
    {MainWindow::Green,     109, 255, 177}, //крест
    {MainWindow::Green,     115, 255, 185}, //крест
    {MainWindow::Green,     132, 255, 208}, //крест
    {MainWindow::Green,     137, 255, 216}, //крест
    {MainWindow::Green,     139, 255, 220}, //крест
    {MainWindow::Green,     143, 255, 228}, //крест
    {MainWindow::Green,     144, 255, 228}, //крест
    {MainWindow::Green,     147, 255, 231}, //крест
    {MainWindow::Green,     186, 255, 255}, //крест

    {MainWindow::Magenta,   246,  16, 246}, //обычный
    {MainWindow::Magenta,   246,  19, 246}, //пламя
    {MainWindow::Magenta,   212,  18, 211}, //x2
    {MainWindow::Magenta,   166,  14, 165}, //x5
    {MainWindow::Magenta,   255,  73, 255}, //крест
    {MainWindow::Magenta,   255,  81, 255}, //крест
    {MainWindow::Magenta,   255,  89, 255}, //крест
    {MainWindow::Magenta,   255,  97, 255}, //крест
    {MainWindow::Magenta,   255, 102, 255}, //крест
    {MainWindow::Magenta,   255, 133, 255}, //крест

    {MainWindow::Red,       254,  28,  57}, //обычный
    {MainWindow::Red,       255,  31,  59}, //пламя
    {MainWindow::Red,       245,  17,  32}, //x2
    {MainWindow::Red,       191,  13,  25}, //x5
    {MainWindow::Red,       255,  91, 133}, //крест
    {MainWindow::Red,       255,  91, 135}, //крест
    {MainWindow::Red,       255, 100, 143}, //крест
    {MainWindow::Red,       255, 106, 152}, //крест
    {MainWindow::Red,       255, 111, 156}, //крест
    {MainWindow::Red,       255, 113, 159}, //крест
    {MainWindow::Red,       255, 124, 172}, //крест
    {MainWindow::Red,       255, 143, 196}, //крест

    {MainWindow::Yellow,    254, 251,  37}, //обычный
    {MainWindow::Yellow,    178, 143,  33}, //монетка
    {MainWindow::Yellow,    255, 252,  39}, //пламя
    {MainWindow::Yellow,    186, 183,   0}, //x5
    {MainWindow::Yellow,     86,  84,   0}, //x6

    {MainWindow::Brown,     254, 251, 121}, //обычный
    {MainWindow::Brown,     255, 248, 120}, //пламя
    {MainWindow::Brown,     245, 122,  42}, //x2
    {MainWindow::Brown,     155,  77,  26}, //x4
    {MainWindow::Brown,     255, 255, 172}, //крест
    {MainWindow::Brown,     255, 255, 192}, //крест
    {MainWindow::Brown,     255, 255, 215}, //крест
    {MainWindow::Brown,     255, 255, 218}, //крест
    {MainWindow::Brown,     255, 255, 221}, //крест
    {MainWindow::Brown,     255, 255, 234}, //крест
    {MainWindow::Brown,     255, 255, 249}, //крест

    {MainWindow::White,     254, 254, 254}, //обычный
    {MainWindow::White,     255, 255, 255}, //пламя
    {MainWindow::White,     205, 205, 205}, //x3
    {MainWindow::White,     130, 130, 130}, //x4

    {MainWindow::Universal,  23,  12,   6},
    {MainWindow::Universal,  38,  22,  18},
    {MainWindow::Universal,  63,  91, 113},
    {MainWindow::Universal, 123,  93,  86},
    {MainWindow::Universal, 129, 171, 129},
    {MainWindow::Universal, 153, 105, 152},
    {MainWindow::Universal, 163, 147, 109},
    {MainWindow::Universal, 192, 231, 253},
    {MainWindow::Universal, 212, 191, 159},

    // Конец массива. Не удалять и не редактировать.
    {MainWindow::Undefined,  -1,  -1,  -1}
  };

}

//[----------------------------------------------------------------------------]

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent),
    m_hWnd(0),
    m_isActive(false),
    m_findWindowMode(false),
    m_clientOfsX(0),
    m_clientOfsY(0)
{
  ui.setupUi(this);

  Qt::WindowFlags flags = windowFlags();
  flags &= ~Qt::WindowMaximizeButtonHint;
  flags &= ~Qt::WindowMinimizeButtonHint;
  setWindowFlags(flags);

  m_helper = new Helper;
  hideHelper();

  GlobalShortcutManager::instance()->connect(
        QKeySequence::fromString("Alt+A"), this, SLOT(startStop()));

  // DEBUG dump field. Смотри описание метода dumpField().
//  GlobalShortcutManager::instance()->connect(
//        QKeySequence::fromString("Alt+S"), this, SLOT(dumpField()));

  ui.lblTarget->installEventFilter(this);

  connect(&m_timer, SIGNAL(timeout()), this, SLOT(timer()));

  ui.chkThumbnail->setEnabled(false);
  ui.tbtnRefresh->setEnabled(false);
  ui.actionAuto_play->setEnabled(false);

  loadConfig();
}

//[----------------------------------------------------------------------------]

MainWindow::~MainWindow()
{
  m_colors.clear();
  GlobalShortcutManager::instance()->clear();
}

//[----------------------------------------------------------------------------]

void MainWindow::on_actionStay_on_top_triggered(bool on)
{
  Qt::WindowFlags flags = windowFlags();

  if(on)
    flags |= Qt::WindowStaysOnTopHint;
  else
    flags &= ~Qt::WindowStaysOnTopHint;

  setWindowFlags(flags);
  show();
}

//[----------------------------------------------------------------------------]

void MainWindow::timer()
{
  if(!m_hWnd)
    return;

  captureWindow(m_hWnd);
  capturePixels();

  findAndExecuteMove();
}

//[----------------------------------------------------------------------------]

void MainWindow::captureWindow(WId hW)
{
  hideHelper();
  setupClientOffsets(m_hWnd);

  m_pixmap = QPixmap::grabWindow(hW, ui.edOfsX->value(), ui.edOfsY->value(), FIELD_W, FIELD_H);
  m_image  = m_pixmap.toImage();

  if(ui.chkThumbnail->isChecked())
  {
    QPixmap thmb = m_pixmap.scaled(QSize(FIELD_W, FIELD_H), Qt::KeepAspectRatio);
    ui.lblThumb->setPixmap(thmb);
  }
}

//[----------------------------------------------------------------------------]
//[ Поиск координат хода игрока.
//[ Выполнение хода
//[----------------------------------------------------------------------------]
void MainWindow::findAndExecuteMove()
{
  Solver solver;

  QList<int> field;

  extractSingleColor(field, White);
  if(solver.DoSolve(field))
  {
    doTurn(solver.from(), solver.to());
    return;
  }

  extractSingleColor(field, Yellow);
  if(solver.DoSolve(field))
  {
    doTurn(solver.from(), solver.to());
    return;
  }

  extractSingleColor(field, Green);
  if(solver.DoSolve(field))
  {
    doTurn(solver.from(), solver.to());
    return;
  }

  extractSingleColor(field, Red);
  if(solver.DoSolve(field))
  {
    doTurn(solver.from(), solver.to());
    return;
  }

  extractSingleColor(field, Magenta);
  if(solver.DoSolve(field))
  {
    doTurn(solver.from(), solver.to());
    return;
  }

  extractSingleColor(field, Blue);
  if(solver.DoSolve(field))
  {
    doTurn(solver.from(), solver.to());
    return;
  }

  extractSingleColor(field, Brown);
  if(solver.DoSolve(field))
  {
    doTurn(solver.from(), solver.to());
    return;
  }
}

//[----------------------------------------------------------------------------]
//[ Заполняет m_colors значениями цветов (RGB) из скриншота
//[----------------------------------------------------------------------------]
void MainWindow::capturePixels()
{
  m_colors.clear();
  int idx = 0;

  for(int y = 0; y < FIELD_H; y += CELL_H)
    for(int x = 0; x < FIELD_W; x += CELL_W)
    {
      QRgb pix = m_image.pixel(x + PIX_PROBE_X, y + PIX_PROBE_Y);
      m_colors.push_back(pix);
      ++idx;
    }

  QRgb tl = m_image.pixel(        0,         0);
  QRgb tr = m_image.pixel(        0, FIELD_H-1);
  QRgb bl = m_image.pixel(FIELD_W-1, 0);
  QRgb br = m_image.pixel(FIELD_W-1, FIELD_H-1);

  //qDebug() << "tl:" << tl << " tr:" << tr << " bl:" << bl << " br:" << br;

  // Индикатор правильной настройки смещений
  // сравнивает угловые значения цвета точек с теми,
  // что должны быть на игровом поле при старте игры
  if((tl == 4282786058UL) && (tr == 4282063107UL) &&
     (bl == 4282786058UL) && (br == 4282063107UL))
  {
    ui.lblCheck->setPixmap(QPixmap(picActive));
  }
  else
  {
    ui.lblCheck->setPixmap(QPixmap(picUnChecked));
  }
}

//[----------------------------------------------------------------------------]
//[ WM_LBUTTONDOWN
//[ wParam: MK_LBUTTON
//[ lParam:
//[         The low-order word specifies the x-coordinate of the cursor.
//[         The coordinate is relative to the upper-left corner of the client area.
//[         The high-order word specifies the y-coordinate of the cursor.
//[         The coordinate is relative to the upper-left corner of the client area.
//[ WM_LBUTTONUP
//[ wParam: 0
//[ lParam: the same
//[
//[ ! Платформозависимая - только Win32
//[----------------------------------------------------------------------------]
void MainWindow::sendMouseLClick(WId hWnd, int x, int y)
{
  if(!hWnd)
    return;

  LPARAM lParam = MAKELPARAM(x, y);

  ::SendMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, lParam);
  ::Sleep(3);
  ::SendMessage(hWnd, WM_LBUTTONUP, 0, lParam);
}

//[----------------------------------------------------------------------------]

MainWindow::GemType MainWindow::guessGemType(QRgb color)
{
  bool done = false;
  int  idx  = 0;

  while(!done)
  {
    GemType gem = rgb[idx].gem;

    if(gem == Undefined)
    {
      done = true;
    }
    else
    {
      if(color == qRgb(rgb[idx].r, rgb[idx].g, rgb[idx].b))
        return gem;
    }
    ++idx;
  }

  return Undefined;
}

//[----------------------------------------------------------------------------]

void MainWindow::extractSingleColor(QList<int>& field, GemType gem)
{
  field.clear();

  foreach(QRgb color, m_colors)
  {
    GemType currentGem = guessGemType(color);

    // бонус в виде универсального кристалла попадает в массив любого цвета
    field.push_back(((currentGem == gem) || (currentGem == Universal)) ? 1 : 0);
  }
}

//[----------------------------------------------------------------------------]

void MainWindow::doTurn(QPoint from, QPoint to)
{
  int x_src = ui.edOfsX->value() + m_clientOfsX + (from.x() * CELL_W) + CELL_W/2-3;
  int y_src = ui.edOfsY->value() + m_clientOfsY + (from.y() * CELL_H) + CELL_H/2-3;
  int x_dst = ui.edOfsX->value() + m_clientOfsX + (to.x()   * CELL_W) + CELL_W/2-3;
  int y_dst = ui.edOfsY->value() + m_clientOfsY + (to.y()   * CELL_H) + CELL_H/2-3;

  if(ui.chkHelper->isChecked())
  {
    showHelper(x_src, y_src, x_dst, y_dst);
  }

  if(ui.chkAutoPlay->isChecked())
  {
    sendMouseLClick(m_hWnd, x_src - m_clientOfsX, y_src - m_clientOfsY);
    sendMouseLClick(m_hWnd, x_dst - m_clientOfsX, y_dst - m_clientOfsY);
  }
}

//[----------------------------------------------------------------------------]

void MainWindow::hideHelper()
{
  m_helper->hide();
}

//[----------------------------------------------------------------------------]

void MainWindow::showHelper(int x_src, int y_src, int x_dst, int y_dst)
{
  m_helper->setHelper(x_src, y_src, x_dst, y_dst);
  m_helper->show();
}

//[----------------------------------------------------------------------------]

void MainWindow::closeEvent(QCloseEvent *e)
{
  if(m_timer.isActive())
    m_timer.stop();

  delete m_helper;

  saveConfig();

  QMainWindow::closeEvent(e);
}

//[----------------------------------------------------------------------------]
//[ Функция для отладки
//[ Создаёт в папке программы два файла:
//[
//[ 1) скриншот игрового поля в файле fld_<день_минуты_секунды>.png
//[ 2) захваченные значения точек в формате R, G, B в файле fld_<день_минуты_секунды>.txt
//[----------------------------------------------------------------------------]
void MainWindow::dumpField()
{
  if(!m_hWnd)
    return;

  m_pixmap = QPixmap::grabWindow(m_hWnd, ui.edOfsX->value(), ui.edOfsY->value(), FIELD_W, FIELD_H);
  m_image  = m_pixmap.toImage();

  capturePixels();

  QString txt = QString("%1/fld_%2.txt")
      .arg(QApplication::applicationDirPath())
      .arg(QDateTime::currentDateTime().toString("DD_mm_ss"));

  QFile data(txt);
  bool bOk = data.open(QFile::WriteOnly | QFile::Truncate);
  if(!bOk)
  {
    QMessageBox::warning(this, "Warning", "Unable to create dump files. Check application folder access rights");
    return;
  }

  QTextStream s(&data);

  int idx = 0;
  foreach(QRgb clr, m_colors)
  {
    s << QString("%1,%2,%3")
         .arg(qRed(clr),   3, 10, QLatin1Char(' '))
         .arg(qGreen(clr), 3, 10, QLatin1Char(' '))
         .arg(qBlue(clr),  3, 10, QLatin1Char(' '));
    ++idx;
    if(idx >= 8)
    {
      s << "\n";
      idx = 0;
    }
    else
    {
      s << "  ";
    }
  }

  QString pic = QString("%1/fld_%2.png")
      .arg(QApplication::applicationDirPath())
      .arg(QDateTime::currentDateTime().toString("DD_mm_ss"));

  m_image.save(pic);
}

//[----------------------------------------------------------------------------]

void MainWindow::startStop()
{
  if(ui.actionAuto_play->isEnabled())
  {
    bool checked = ui.actionAuto_play->isChecked();
    on_actionAuto_play_triggered(!checked);
    ui.actionAuto_play->setChecked(!checked);
  }
}

//[----------------------------------------------------------------------------]

void MainWindow::on_actionAuto_play_triggered(bool on)
{
  if(on)
  {
    m_timer.setInterval(ui.edTimeout->value());
    ui.edTimeout->setEnabled(false);

    ui.lblActive->setPixmap(QPixmap(picActive));
    if(!m_timer.isActive())
      m_timer.start();
  }
  else
  {
    ui.edTimeout->setEnabled(true);
    ui.lblActive->setPixmap(QPixmap(picInactive));
    if(m_timer.isActive())
      m_timer.stop();
  }
}

//[----------------------------------------------------------------------------]
//[ Ищет handle окна над которым находится курсор мышки.
//[ Если окно не найдено или ошибка - возвращает 0
//[ ! Платформозависимая - только Win32
//[----------------------------------------------------------------------------]
WId MainWindow::lookupWindow()
{
  POINT Point;

  Point.x = QCursor::pos().x();
  Point.y = QCursor::pos().y();

  WId hwnd = ::WindowFromPoint(Point);

  return hwnd;
}

//[----------------------------------------------------------------------------]

QString MainWindow::getWindowClassName(WId hwnd)
{
  if(hwnd)
  {
    std::vector<TCHAR> buff(1024);
    ::GetClassName(hwnd, buff.data(), 1022);
    return QString::fromWCharArray(buff.data());
  }

  return QString("UNKNOWN");
}

//[----------------------------------------------------------------------------]
//[ Получение информации об окне - смещение до клиентской части.
//[----------------------------------------------------------------------------]
void MainWindow::setupClientOffsets(WId hwnd)
{
  if(!hwnd)
    return;

  WINDOWINFO wi;

  wi.cbSize = sizeof(WINDOWINFO);

  ::GetWindowInfo(hwnd, &wi);

  m_clientOfsX = wi.rcClient.left;
  m_clientOfsY = wi.rcClient.top;
}

//[----------------------------------------------------------------------------]
//[ Перехватывает нажатие левой кнопки мышки на QLabel с мишенью
//[----------------------------------------------------------------------------]
bool MainWindow::eventFilter(QObject* obj, QEvent* event)
{
  if(obj == ui.lblTarget)
  {
    if(event->type() == QEvent::MouseButtonPress)
    {
      QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
      if(!m_findWindowMode && (mouseEvent->buttons() & Qt::LeftButton))
      {
        m_findWindowMode = true;
        grabMouse(QCursor(QPixmap(picTarget)));
      }
    }
  }
  return QMainWindow::eventFilter(obj, event);
}

//[----------------------------------------------------------------------------]

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
  if(m_findWindowMode)
  {
    m_hWnd = lookupWindow();

    if(m_hWnd)
    {
      ui.lblWindowActive->setPixmap(QPixmap(picActive));
      ui.chkThumbnail->setEnabled(true);
      ui.tbtnRefresh->setEnabled(true);
      ui.actionAuto_play->setEnabled(true);

      captureWindow(m_hWnd);
    }

    m_findWindowMode = false;
    releaseMouse();
  }

  QMainWindow::mouseReleaseEvent(e);
}

//[----------------------------------------------------------------------------]

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
  if(m_findWindowMode)
  {
    ui.edWndClass->setText(getWindowClassName(lookupWindow()));
  }
  QMainWindow::mouseMoveEvent(e);
}

//[----------------------------------------------------------------------------]

void MainWindow::on_tbtnRefresh_clicked()
{
  captureWindow(m_hWnd);
}

//[----------------------------------------------------------------------------]

void MainWindow::on_chkThumbnail_stateChanged(int state)
{
  if(state == Qt::Checked)
  {
    captureWindow(m_hWnd);
  }
  else
  {
    ui.lblThumb->setPixmap(0);
  }
}

//[----------------------------------------------------------------------------]

void MainWindow::on_actionAbout_triggered()
{
  AboutDlg dlg(this);
  dlg.exec();
}

//[----------------------------------------------------------------------------]

void MainWindow::loadConfig()
{
  QSettings settings(QDir::toNativeSeparators(QApplication::applicationDirPath() + "/settings.conf"),
                     QSettings::IniFormat);
  ui.edOfsX->setValue(settings.value("OffsetX").toInt());
  ui.edOfsY->setValue(settings.value("OffsetY").toInt());
  ui.edTimeout->setValue(settings.value("TimerDalay").toInt());
  ui.chkAutoPlay->setChecked(settings.value("AutoPlay").toBool());
  ui.chkHelper->setChecked(settings.value("ShowHelper").toBool());
}

//[----------------------------------------------------------------------------]

void MainWindow::saveConfig()
{
  QSettings settings(QDir::toNativeSeparators(QApplication::applicationDirPath() + "/settings.conf"),
                     QSettings::IniFormat);
  settings.setValue("OffsetX",    ui.edOfsX->value());
  settings.setValue("OffsetY",    ui.edOfsY->value());
  settings.setValue("AutoPlay",   ui.chkAutoPlay->isChecked());
  settings.setValue("ShowHelper", ui.chkHelper->isChecked());
  settings.setValue("TimerDalay", ui.edTimeout->value());
}

//[----------------------------------------------------------------------------]
