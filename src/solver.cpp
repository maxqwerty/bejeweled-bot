#include "solver.h"

#include <QDebug>

namespace {
  const int board_w = 8;
  const int board_h = 8;
}

//[----------------------------------------------------------------------------]

Solver::Solver()
{
}

//[----------------------------------------------------------------------------]

int Solver::index(int x, int y)
{
  return y * board_w + x;
}

//[----------------------------------------------------------------------------]

bool Solver::F(int x, int y)
{
  return m_field[index(x, y)] == 1;
}

//[----------------------------------------------------------------------------]

bool Solver::DoSolve(const QList<int>& field)
{
  m_field = field;

  if(solver01() || solver02() || solver03() ||
     solver11() || solver12() || solver13() ||
     solver21() || solver22() ||
     solver31() || solver32())
    return true;

  m_whichSolver = "NONE";

  return false;
}

//[----------------------------------------------------------------------------]

#define FWD1 for(int y = 0; y < board_h-2; ++y) \
               for(int x = 0; x < board_w-1; ++x)

#define REV1 for(int y = 0; y < board_h-2; ++y) \
               for(int x = board_w-1; x > 0; --x)

#define FWD2 for(int y = 0; y < board_h-1; ++y) \
               for(int x = 0; x < board_w-2; ++x)

#define REV2 for(int y = board_h-1; y > 0; --y) \
               for(int x = 0; x < board_w-2; ++x)

#define FWD22 for(int y = 0; y < board_h-3; ++y) \
                for(int x = 0; x < board_w; ++x)

#define FWD32 for(int y = 0; y < board_h; ++y) \
                for(int x = 0; x < board_w-3; ++x)

//[----------------------------------------------------------------------------]
//[ 10
//[ 10
//[ 01
//[----------------------------------------------------------------------------]
bool Solver::solver01()
{
  FWD1 if(F(x, y) && F(x, y+1) && F(x+1, y+2))
  {
    m_from = QPoint(x+1, y+2);
    m_to   = QPoint(x,   y+2);
    m_whichSolver = "solver01 FWD";
    return true;
  }

  REV1 if(F(x, y) && F(x, y+1) && F(x-1, y+2))
  {
    m_from = QPoint(x-1, y+2);
    m_to   = QPoint(x,   y+2);
    m_whichSolver = "solver01 REV";
    return true;
  }

  return false;
}

//[----------------------------------------------------------------------------]
//[ 10
//[ 01
//[ 10
//[----------------------------------------------------------------------------]
bool Solver::solver02()
{
  FWD1 if(F(x, y) && F(x+1, y+1) && F(x, y+2))
  {
    m_from = QPoint(x+1, y+1);
    m_to   = QPoint(x,   y+1);
    m_whichSolver = "solver02 FWD";
    return true;
  }

  REV1 if(F(x, y) && F(x-1, y+1) && F(x, y+2))
  {
    m_from = QPoint(x-1, y+1);
    m_to   = QPoint(x,   y+1);
    m_whichSolver = "solver02 REV";
    return true;
  }

  return false;
}

//[----------------------------------------------------------------------------]
//[ 01
//[ 10
//[ 10
//[----------------------------------------------------------------------------]
bool Solver::solver03()
{
  FWD1 if(F(x+1, y) && F(x, y+1) && F(x, y+2))
  {
    m_from = QPoint(x+1, y);
    m_to   = QPoint(x,   y);
    m_whichSolver = "solver03 FWD";
    return true;
  }

  REV1 if(F(x-1, y) && F(x, y+1) && F(x, y+2))
  {
    m_from = QPoint(x-1, y);
    m_to   = QPoint(x,   y);
    m_whichSolver = "solver03 REV";
    return true;
  }

  return false;
}

//[----------------------------------------------------------------------------]
//[ 101
//[ 010
//[----------------------------------------------------------------------------]
bool Solver::solver11()
{
  FWD2 if(F(x, y) && F(x + 1, y + 1) && F(x + 2, y))
  {
    m_from = QPoint(x+1, y+1);
    m_to   = QPoint(x+1, y);
    m_whichSolver = "solver11 FWD";
    return true;
  }

  REV2 if(F(x, y) && F(x + 1, y - 1) && F(x + 2, y))
  {
    m_from = QPoint(x+1, y-1);
    m_to   = QPoint(x+1, y);
    m_whichSolver = "solver11 REV";
    return true;
  }

  return false;
}

//[----------------------------------------------------------------------------]
//[ 011
//[ 100
//[----------------------------------------------------------------------------]
bool Solver::solver12()
{
  FWD2 if(F(x, y + 1) && F(x + 1, y) && F(x + 2, y))
  {
    m_from = QPoint(x, y+1);
    m_to   = QPoint(x, y);
    m_whichSolver = "solver12 FWD";
    return true;
  }

  REV2 if(F(x, y - 1) && F(x + 1, y) && F(x + 2, y))
  {
    m_from = QPoint(x, y-1);
    m_to   = QPoint(x, y);
    m_whichSolver = "solver12 REV";
    return true;
  }

  return false;
}

//[----------------------------------------------------------------------------]
//[ 110
//[ 001
//[----------------------------------------------------------------------------]
bool Solver::solver13()
{
  FWD2 if(F(x, y) && F(x + 1, y) && F(x + 2, y + 1))
  {
    m_from = QPoint(x+2, y+1);
    m_to   = QPoint(x+2, y);
    m_whichSolver = "solver13 FWD";
    return true;
  }

  REV2 if(F(x, y) && F(x + 1, y) && F(x + 2, y - 1))
  {
    m_from = QPoint(x+2, y-1);
    m_to   = QPoint(x+2, y);
    m_whichSolver = "solver13 REV";
    return true;
  }

  return false;
}

//[----------------------------------------------------------------------------]
//[ 1
//[ 0
//[ 1
//[ 1
//[----------------------------------------------------------------------------]
bool Solver::solver21()
{
  FWD22 if(F(x, y) && !F(x, y+1) && F(x, y+2) && F(x, y+3))
  {
    m_from = QPoint(x, y);
    m_to   = QPoint(x, y+1);
    m_whichSolver = "solver21";
    return true;
  }

  return false;
}

//[----------------------------------------------------------------------------]
//[ 1
//[ 1
//[ 0
//[ 1
//[----------------------------------------------------------------------------]
bool Solver::solver22()
{
  FWD22 if(F(x, y) && F(x, y+1) && !F(x, y+2) && F(x, y+3))
  {
    m_from = QPoint(x, y+3);
    m_to   = QPoint(x, y+2);
    m_whichSolver = "solver22";
    return true;
  }

  return false;
}

//[----------------------------------------------------------------------------]
//[ 1011
//[----------------------------------------------------------------------------]
bool Solver::solver31()
{
  FWD32 if(F(x, y) && !F(x+1, y) && F(x+2, y) && F(x+3, y))
  {
    m_from = QPoint(x,   y);
    m_to   = QPoint(x+1, y);
    m_whichSolver = "solver31";
    return true;
  }

  return false;
}

//[----------------------------------------------------------------------------]
//[ 1101
//[----------------------------------------------------------------------------]
bool Solver::solver32()
{
  FWD32 if(F(x, y) && F(x+1, y) && !F(x+2, y) && F(x+3, y))
  {
    m_from = QPoint(x+3, y);
    m_to   = QPoint(x+2, y);
    m_whichSolver = "solver32";
    return true;
  }

  return false;
}

//[----------------------------------------------------------------------------]
