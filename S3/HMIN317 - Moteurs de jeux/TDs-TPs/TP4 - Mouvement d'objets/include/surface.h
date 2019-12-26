#ifndef SURFACE_H
#define SURFACE_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

#include <iostream>

std::ostream& operator<<(std::ostream& os, const QVector2D& v);
std::ostream& operator<<(std::ostream& os, const QVector3D& v);

class Surface
{
public:

    struct Vertex
    {
        QVector3D position;
        QVector2D tex_coord;
//        QVector3D color;
    };

    // rect : la forme du rectangle qui sera formé par la surface
    // nb_quad_x : le nombre de quad en largeur
    // nb_quad_y : le nombre de quad en hauteur
    Surface(QVector2D topLeft, QVector2D bottomRight, size_t nb_quad_x, size_t nb_quad_y);

    Surface::Vertex& operator()(size_t quad_index_y, size_t quad_index_x);

    QVector<Surface::Vertex> vertexes;
    QVector<GLushort> vertexes_indices;

private:

    size_t nb_quad_x;
    size_t nb_quad_y;
};

#endif
 
