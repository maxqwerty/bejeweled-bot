#ifndef SOLVER_H
#define SOLVER_H

#include <QPoint>
#include <QList>
#include <QString>

//[----------------------------------------------------------------------------]

class Solver
{
public:

  Solver();
  bool DoSolve(const QList<int>& field);

  QPoint from() const {return m_from;}
  QPoint to()   const {return m_to;}

  QString solverString() const {return m_whichSolver;}

private:
  int  index(int x, int y);
  bool F(int x, int y);

  bool solver01();
  bool solver02();
  bool solver03();

  bool solver11();
  bool solver12();
  bool solver13();

  bool solver21();
  bool solver22();

  bool solver31();
  bool solver32();

  QString m_whichSolver;

  QPoint m_from;
  QPoint m_to;

  QList<int> m_field;
};

//[----------------------------------------------------------------------------]

#endif // SOLVER_H

//[----------------------------------------------------------------------------]
