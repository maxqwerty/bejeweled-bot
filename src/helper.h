#ifndef HELPER_H
#define HELPER_H

#include <QWidget>

//[----------------------------------------------------------------------------]

class Helper : public QWidget
{
  Q_OBJECT

public:
  Helper();
  void setHelper(int x, int y, int x1, int y1);
};

//[----------------------------------------------------------------------------]

#endif // HELPER_H

//[----------------------------------------------------------------------------]
