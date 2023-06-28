#include <QApplication>

#include "qcppdialogimpl.h"

//signal handlers should be installed...
int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    QCPPDialogImpl w(0);
    w.show();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
