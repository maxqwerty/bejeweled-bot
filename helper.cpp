#include "helper.h"

//[----------------------------------------------------------------------------]

Helper::Helper()
  : QWidget(0, Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint)
{
  setStyleSheet(
        "background-color: #FF0000;"
        );
}

//[----------------------------------------------------------------------------]

void Helper::setHelper(int x, int y, int x1, int y1)
{
  int w = x1-x;
  if(w == 0)
    w = 3;

  int h = y1-y;
  if(h == 0)
    h = 3;

  setGeometry(qMin(x, x1), qMin(y, y1), qAbs(w), qAbs(h));
}

//[----------------------------------------------------------------------------]
