#include "aboutdlg.h"

#include <QMessageBox>

//[----------------------------------------------------------------------------]

AboutDlg::AboutDlg(QWidget *parent)
  : QDialog(parent)
{
  ui.setupUi(this);
}

//[----------------------------------------------------------------------------]

void AboutDlg::on_btnOk_clicked()
{
  close();
}

//[----------------------------------------------------------------------------]

void AboutDlg::on_btnAboutQt_clicked()
{
  QMessageBox::aboutQt(this, "About Qt");
}

//[----------------------------------------------------------------------------]
