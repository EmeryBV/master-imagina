#ifndef GLWidget_H
#define GLWidget_H


//#include <QMatrix4x4>
//#include <QQuaternion>
//#include <QVector2D>
#include <QBasicTimer>

//////// OpenGL Includes

// Core
#include <QOpenGLWidget>
//#include <QOpenGLWindow>
#include <QOpenGLFunctions>         // Wrapper pour utilliser l'interface OpenGL dans une class

// Debug
#include <QOpenGLDebugLogger>       // Permet de recuperer les erreurs d'execution d'OpenGL (Désactiver pour accelerer OpenGL)
#include <QOpenGLDebugMessage>      // Wrapper pour afficher facilement les messages d'erreur OpenGL

// GPU Memory
#include <QOpenGLBuffer>

    /// <QOpenGLBuffer> est une encapsulation qui permet d'utilliser un VBO OpenGL (Vertex Buffer Object).
    /// Un VBO permet d'allouer de la mémoire sur le GPU.
    /// Il permet également de spécifier au GPU comment cette mémoire doit être utilisé (Layout)
    /// Exemple de layout:
    ///     le buffer est composée de 10 elements,
    ///     chaque elements est composé d'une coordonée de point3D puis d'une coordonée de texture2D.
    /// Du coté CPU chaque VBO est représenté par un identifiant unique (int).
    /// Ces idientifiants permettent au CPU de communiquer avec le GPU via l'interface Opengl.
    /// Exemple de communication CPU -> GPU:
    ///     Charger le VBO d'identifiant 1 en mémoire GPU.
    ///     Dessiner le VBO d'identifiant 1

#include <QOpenGLVertexArrayObject>

    /// <QOpenGLVertexArrayObject> est une encapsulation qui permet d'utilliser un VAO OpenGL (Vertex Array Object).
    /// Un VAO est un objet stocké dans la mémoire GPU qui permet de manipuler facilement un VBO.
    /// Le VAO permet d'initialiser un fois les informations de bind et d'attributs d'un VBO et de les gérer
    /// automatiquement pour l'affichage ensuite.
    /// Exemple initialisation:
    ///     Pour chaque VBO
    ///         Bind the Vertex Array Object lié au vbo
    ///         Set Vertex state, bind attributes, etc.
    ///         Unbind the Vertex Array Object lié au vbo
    /// Exemple d'affichage:
    ///     Pour chaque VBO
    ///         Bind the Vertex Array Object lié au vbo
    ///         Draw VBO
    ///         Unbind the Vertex Array Object lié au vbo
    /// Cela permet d'economiser l'ecriture de code comme le fait de devoir remettre les attribue de chaque vbo avant de dessiner
    /// Pour chaque vbo on va creer un vao pour se simplifier la vie.

// Shader program
#include <QOpenGLShaderProgram>

    /// <QOpenGLShaderProgram> est une encapsulation qui permet de compiler et executer un Shader en GLSL.
    /// Un Shader est simplement un programme qui s'execute sur le GPU
    /// De manière générale le processus d'utiliation d'un shader est le suivants:
    /// Le CPU s'occupe de charger dans la mémoire GPU un ensemble de position possédants des attributs (textures, etc..) ou non (VBO + Layout)
    /// Phase vertex shader : prend en entrée des sommets et convertie leurs position en fonctions d'un matrice de projection (ou mvp).
    /// Phase Interpolation : le GPU interpole les information de sommets pour dessiner des triangles
    /// Phase fragment shader: applique des calculs sur les données interpolées (couleur des pixels, etc...)

#include <QOpenGLTexture>

#include "surface.h"
//#include "geometryengine.h"

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:

    //////// Constructors

    explicit GLWidget(QWidget *parent = nullptr);
    GLWidget(int fps, QWidget *parent = nullptr);

    ~GLWidget();

signals:
    void clicked();

protected:

    //////// Init functions (appelées dans initializeGL())

    void initializeDebugGL();
    void initializeShadersGL();
    void initializeTexturesGL();

    void terminateGL(); // s'occupe de nettoyer les objets opengl VAO / VBO / Textures / Shaders

    //////// OpenGL overrides

    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

    //////// Event overrides

    void keyPressEvent(QKeyEvent *e) override;
//    void keyReleaseEvent(QKeyEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void timerEvent(QTimerEvent *e) override;

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    //////// Debug functions

    void printVersionInformation();

protected slots:

    void messageLogged(const QOpenGLDebugMessage& msg);

private:

    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_ebo; // element buffer object
    QOpenGLVertexArrayObject m_vao;
    QOpenGLShaderProgram *m_shader_program;
    QOpenGLDebugLogger *m_debug_logger;

    QMatrix4x4 model;
    QMatrix4x4 view;
    QMatrix4x4 projection;

    QOpenGLTexture *heightmap;
    QOpenGLTexture *grass;
    QOpenGLTexture *rock;
    QOpenGLTexture *snowrocks;

    Surface *surface;


    QBasicTimer timer;
    float rotate_speed = 1.0;
    float u_height_factor = 0.1;
    /*
    QOpenGLShaderProgram program;
    QOpenGLDebugLogger m_debugLogger;

    GeometryEngine *geometries;




    QVector2D mousePressPosition;
    QVector3D rotationAxis;
    qreal angularSpeed;
    QQuaternion rotation;
    */
};

#endif // GLWidget_H
 
