#include <QApplication>
#include <QCheckBox>

#include "testTriStateCheckbox.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

////  testTriStateCheckbox w;
////  w.show();

////  QCheckBox cb1( "Should Show Normal" );
//  QCheckBox cb2( "Should Show TriState" );
////  QCheckBox cb3( "Should Show Checked" );

////  cb1.setCheckState( Qt::Unchecked );

//  cb2.setTristate( true );
//  cb2.setCheckState( Qt::Unchecked );
//  cb2.setCheckState( Qt::PartiallyChecked );
//  cb2.setCheckState( Qt::Checked );

////  cb3.setCheckState( Qt::Checked );

////  cb1.show();
//  cb2.show();
////  cb3.show();

  testTriStateCheckbox tsc;
  tsc.show();

  a.exec();
}
