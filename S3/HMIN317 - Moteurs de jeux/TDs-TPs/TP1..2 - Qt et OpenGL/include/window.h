#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <array>

#include "glwidget.h"


class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

private slots:
    void setCurrentGlWidget();

private:
    std::array<GLWidget*, 4> glWidgets;
    GLWidget *currentGlWidget;
};

#endif
