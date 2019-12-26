#include <QtWidgets>

#include "window.h"

Window::Window()
{
    QGridLayout *mainLayout = new QGridLayout;

    for (int i = 0; i < glWidgets.size(); ++i)
    {
        glWidgets[i] = new GLWidget(pow(10, i));
//        glWidgets[i]->resize(QSize(400, 400));

        mainLayout->addWidget(glWidgets[i], i / 2, i % 2);
        connect(glWidgets[i], &GLWidget::clicked, this, &Window::setCurrentGlWidget);
    }

    setLayout(mainLayout);

    currentGlWidget = glWidgets[0];
    currentGlWidget->setFocus();

    QTimer *timer = new QTimer(this);
//    connect(timer, &QTimer::timeout, this, &Window::rotateOneStep);
    timer->start(20);

    setWindowTitle(tr("TP1..2"));

    qInfo() << "Clicker sur une des fenetres pour prendre le focus\n";
    qInfo() << "UP/DOWN : Gerer la vitesse de rotation de la caméra\n";
    qInfo() << "Left/right : Bouger la caméra à gauche/droite\n";
    qInfo() << "U/O : Modifier les valeur de heighmap\n";
}

void Window::setCurrentGlWidget()
{
    currentGlWidget = qobject_cast<GLWidget *>(sender());
    currentGlWidget->setFocus();
    qDebug() << currentGlWidget << " is focused\n";
}
