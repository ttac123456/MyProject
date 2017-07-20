#include <QApplication>
#include <QCheckBox>

#include "testTriStateCheckbox.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  testTriStateCheckbox tsc;
  tsc.show();

  a.exec();
}
