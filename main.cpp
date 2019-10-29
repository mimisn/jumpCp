#include "contentwindown.h"
#include "loginwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginWindow w;
    //ContentWindown x;
    w.show();
    //x.show();

    return a.exec();
}
