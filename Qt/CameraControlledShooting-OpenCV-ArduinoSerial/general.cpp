#include <QTime>

//http://stackoverflow.com/questions/3752742/how-do-i-create-a-pause-wait-function-using-qt
static void delay( int millisecondsToWait ) { //why is static needed? otherwise it would say "double definition"
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime ) {
        //QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}
