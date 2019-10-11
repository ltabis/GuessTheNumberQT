//
// Created by tabis on 10/10/2019.
//

#include "Exception.hpp"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char **argv)
{
    try {
        QApplication a(argc, argv);
        MainWindow w;
        w.show();

        return a.exec();
    } catch (Log::Exception &exception) {
        exception.debugErrorMessage();
    }
    return 0;
}