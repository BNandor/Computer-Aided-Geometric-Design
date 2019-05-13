#include <QApplication>
#include "GUI/MainWindow.h"
#include "Core/Matrices.h"
#include <iostream>

using namespace std;

using namespace cagd;

int main(int argc, char **argv)
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);

    // creating an application object and setting one of its attributes
    QApplication app(argc, argv);

    // if you have installed a different version of Qt, it may happen that
    // the application attribute Qt::AA_UseDesktopOpenGL is not recognized
    // on Windows its existence is critical for our applications
    // on Linux or Mac you can uncomment this line
    app.setAttribute(Qt::AA_UseDesktopOpenGL, true);

    // creating a main window object
    MainWindow mwnd;
    mwnd.showMaximized();

//    Matrix<int> M(3, 5);

//    M(2, 3) = -1;

//    cout << M << endl;

    // running the application
    return app.exec();
}
