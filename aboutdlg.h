#ifndef ABOUTDLG_H
#define ABOUTDLG_H

#include "ui_aboutdlg.h"

//[----------------------------------------------------------------------------]

class AboutDlg : public QDialog
{
  Q_OBJECT

public:
  explicit AboutDlg(QWidget *parent = 0);

private slots:
  void on_btnOk_clicked();
  void on_btnAboutQt_clicked();

private:
  Ui::AboutDlg ui;
};

//[----------------------------------------------------------------------------]

#endif // ABOUTDLG_H

//[----------------------------------------------------------------------------]
