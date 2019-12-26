#include "surface.h"

#include <QVector2D>
#include <QVector3D>
#include <cassert>

std::ostream& operator<<(std::ostream& os, const QVector3D& v)
{
    return os << "(" << v.x() << ", " << v.y() << ", " << v.z() << ")";
}

std::ostream& operator<<(std::ostream& os, const QVector2D& v)
{
    return os << "(" << v.x() << ", " << v.y() << ")";
}

Surface::Surface(QVector2D topLeft, QVector2D bottomRight, size_t nb_quad_x, size_t nb_quad_y) :
    nb_quad_x(nb_quad_x),
    nb_quad_y(nb_quad_y)
{
//    qDebug() << "Optimisation à faire sur la taille des tableaux\n";

    QVector2D start_pos = topLeft;
    QVector2D end_pos = bottomRight;
    QVector2D dimension = (end_pos - start_pos);
    QVector2D quad_offset = QVector2D(dimension.x() / (nb_quad_x - 1), dimension.y() / (nb_quad_y - 1));

//    qDebug() << "start_pos : " << start_pos << "\n";
//    qDebug() << "end_pos : " << end_pos << "\n";
//    qDebug() << "quad_offset : " << quad_offset << "\n";

    QVector2D tex_start(0, 1);
    QVector2D tex_end(1, 0);
    QVector2D tex_dimension = (tex_end- tex_start);
    QVector2D tex_quad_offset = QVector2D(tex_dimension.x() / (nb_quad_x - 1), tex_dimension.y() / (nb_quad_y - 1));

    for(size_t y = 0 ; y < nb_quad_y ; y++)
    {
        for(size_t x = 0 ; x < nb_quad_x ; x++)
        {
            // Vertices
            QVector2D next_position = start_pos + quad_offset * QVector2D(x, y);
            // TexCoord
            QVector2D next_texCoord = tex_start + tex_quad_offset * QVector2D(x, y);

            vertexes.push_back({next_position, next_texCoord});

//            qDebug() << "Vertex" << QVector2D(y, x) << " = " << next_position << " & " << next_texCoord << "\n";
        }
    }

    // Création des triangles (indices entre les sommets)
    for(size_t y = 0 ; y < nb_quad_y - 1 ; y++)
    {
        for(size_t x = 0 ; x < nb_quad_x - 1 ; x++)
        {
            // Récupere les indices pérmentant de former un quadrilatère composé de 2 triangles
            GLushort quad_indice_a = y * nb_quad_x + x;
            GLushort quad_indice_b = y * nb_quad_x + x + 1;
            GLushort quad_indice_c = (y + 1) * nb_quad_x + x;
            GLushort quad_indice_d = (y + 1) * nb_quad_x + (x + 1);

            // Le 1er triangle
            vertexes_indices.push_back(quad_indice_a);
            vertexes_indices.push_back(quad_indice_c);
            vertexes_indices.push_back(quad_indice_b);

            // Le 2ème triangle
            vertexes_indices.push_back(quad_indice_b);
            vertexes_indices.push_back(quad_indice_c);
            vertexes_indices.push_back(quad_indice_d);

//            qDebug() << "Quad" << QVector2D(y, x) << " = " << QVector3D(quad_indice_a, quad_indice_b, quad_indice_c) << " & " << QVector3D(quad_indice_b, quad_indice_c, quad_indice_d) << "\n";
        }
    }
}

Surface::Vertex& Surface::operator()(size_t quad_index_y, size_t quad_index_x)
{
    assert(quad_index_y < nb_quad_y && quad_index_y < nb_quad_y);

    return vertexes[quad_index_y * nb_quad_x + quad_index_x];
}
