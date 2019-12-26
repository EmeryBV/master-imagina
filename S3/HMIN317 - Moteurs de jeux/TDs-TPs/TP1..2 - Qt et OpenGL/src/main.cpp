#include <QApplication>
//#include <QLabel>

#include "window.h"
//#include "glwidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("TP1/TP2");

    Window window;
    window.show();

    return app.exec();
}
 
