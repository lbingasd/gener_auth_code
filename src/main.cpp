#include "pdd_auth_code.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PDDAuthCode w;
    w.show();
    return a.exec();
}
