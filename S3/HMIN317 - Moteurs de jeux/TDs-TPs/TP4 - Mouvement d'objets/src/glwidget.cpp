#include "glwidget.h"

#include <QMouseEvent>
#include <cmath>
#include <iostream>
#include <QVector4D>

//#define NDEBUG // Enleve la sortie d'erreur OpenGL et Assert pour plus d'éfficacité

#include <cassert>

std::ostream& operator<<(std::ostream& os, const QVector4D& v)
{
    return os << "(" << v.x() << ", " << v.y() << ", " << v.z() << ", " << v.w() << ")";
}

//////// Constructors

GLWidget::GLWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    m_vbo(QOpenGLBuffer::VertexBuffer),
    m_ebo(QOpenGLBuffer::IndexBuffer),
    m_vao(),
    m_shader_program(nullptr),
    m_debug_logger(nullptr),
    // Le constructor par default donne des matrices identitées
    model(),
    view(),
    projection(),
    heightmap(nullptr),
    grass(nullptr),
    rock(nullptr),
    snowrocks(nullptr),
    surface(nullptr)
{
    //OpenGLError::pushErrorHandler(this);
//    model.translate(0, 0, 10);

    // Création d'un profile pour la fenetre
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setDepthBufferSize(24);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3,3); // Exige le support d'OpenGL 3 par le hardware

    this->setFormat(format);

    model.rotate(45, {1, 0, 0});
//    view.translate(0, 0, 3);
    timer.start(16, this);
}

GLWidget::GLWidget(int fps, QWidget *parent) :
    QOpenGLWidget(parent),
    m_vbo(QOpenGLBuffer::VertexBuffer),
    m_ebo(QOpenGLBuffer::IndexBuffer),
    m_vao(),
    m_shader_program(nullptr),
    m_debug_logger(nullptr),
    // Le constructor par default donne des matrices identitées
    model(),
    view(),
    projection(),
    heightmap(nullptr),
    grass(nullptr),
    rock(nullptr),
    snowrocks(nullptr),
    surface(nullptr)
{
    //OpenGLError::pushErrorHandler(this);
//    model.translate(0, 0, 10);

    // Création d'un profile pour la fenetre
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setDepthBufferSize(24);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3,3); // Exige le support d'OpenGL 3 par le hardware

    this->setFormat(format);

    model.rotate(45, {1, 0, 0});
//    view.translate(0, 0, 3);

    assert(fps > 0 && (1000 / fps) > 0);

    timer.start(1000 / fps, this);
}

GLWidget::~GLWidget()
{
    terminateGL();
}


//////// Init functions (appelées dans initializeGL())

void GLWidget::initializeDebugGL()
{
    printVersionInformation();

    m_debug_logger = new QOpenGLDebugLogger(this);

    if (m_debug_logger->initialize())
    {
        qDebug() << "GL_DEBUG Debug Logger" << m_debug_logger << "\n";
        connect(m_debug_logger, SIGNAL(messageLogged(QOpenGLDebugMessage)), this, SLOT(messageLogged(QOpenGLDebugMessage)));
        m_debug_logger->startLogging();
    }
    else
    {
        qDebug() << "GL_DEBUG Debug Logger (NONE)\n";
    }
}

void GLWidget::initializeShadersGL()
{
    // Pro tips : Checker la sortie d'erreur ça évite de perdre 1 journée de débuggage (true story) vshader != vshaser.

    // Create Shader (Do not release until VAO is created)
    // make_unique;
    m_shader_program = new QOpenGLShaderProgram(this);

    // Assert verifie que l'execution des fonctions s'est bien passé
    bool success = false;

    success = m_shader_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vshader.glsl");   assert(success);
    success = m_shader_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fshader.glsl"); assert(success);
    success = m_shader_program->link(); assert(success);
    success = m_shader_program->bind(); assert(success);

    // Create Vertex Array Object (permet de se rappeler des attributs et indices liés au vbo)
    success = m_vao.create(); assert(success);
    m_vao.bind();
    {
        // Create VBO (vao enregistrera les info avec le release)
        success = m_vbo.create();  assert(success);
        success = m_vbo.bind();    assert(success);

        surface = new Surface({-0.5f, 0.5f}, {0.5f, -0.5f}, 64, 64);

        m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
        m_vbo.allocate(surface->vertexes.constData(), sizeof(Surface::Vertex) * surface->vertexes.size());

        // Set VBO attributes
        m_shader_program->enableAttributeArray("a_position");
        m_shader_program->enableAttributeArray("a_tex_coord");
        m_shader_program->setAttributeBuffer("a_position", GL_FLOAT, offsetof(Surface::Vertex, position), sizeof(Surface::Vertex::position)/sizeof(GLfloat), sizeof(Surface::Vertex));
        m_shader_program->setAttributeBuffer("a_tex_coord", GL_FLOAT,  offsetof(Surface::Vertex, tex_coord), sizeof(Surface::Vertex::tex_coord)/sizeof(GLfloat), sizeof(Surface::Vertex));
//        m_shader_program->setUniformValue("texture", 0);
//        m_shader_program->bindAttributeLocation;

        // Create EBO (vao enregistrera les info avec le release)
        success = m_ebo.create(); assert(success);
        success = m_ebo.bind();   assert(success);
        m_ebo.allocate(surface->vertexes_indices.constData(), sizeof(decltype(surface->vertexes_indices)::value_type) * surface->vertexes_indices.size());
    }
    m_vao.release();

    m_shader_program->release();
}

void GLWidget::initializeTexturesGL()
{
    heightmap = new QOpenGLTexture(QImage(":/textures/heightmap.png").mirrored());

    // Set nearest filtering mode for texture minification
    heightmap->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    heightmap->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating, Ex: texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    heightmap->setWrapMode(QOpenGLTexture::Repeat);

    grass = new QOpenGLTexture(QImage(":/textures/grass.png").mirrored());

    grass->setMinificationFilter(QOpenGLTexture::Nearest);
    grass->setMagnificationFilter(QOpenGLTexture::Linear);
    grass->setWrapMode(QOpenGLTexture::Repeat);

    rock = new QOpenGLTexture(QImage(":/textures/rock.png").mirrored());

    rock->setMinificationFilter(QOpenGLTexture::Nearest);
    rock->setMagnificationFilter(QOpenGLTexture::Linear);
    rock->setWrapMode(QOpenGLTexture::Repeat);

    snowrocks = new QOpenGLTexture(QImage(":/textures/snowrocks.png").mirrored());

    snowrocks->setMinificationFilter(QOpenGLTexture::Nearest);
    snowrocks->setMagnificationFilter(QOpenGLTexture::Linear);
    snowrocks->setWrapMode(QOpenGLTexture::Repeat);
}

void GLWidget::terminateGL()
{
    // Make sure the context is current when deleting the texture and the buffers.
    makeCurrent();

    m_vbo.destroy();
    m_ebo.destroy();
    m_vao.destroy();
    delete m_shader_program;
    delete m_debug_logger;

    doneCurrent();
}

//////// OpenGL overrides


void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();

#ifndef   NDEBUG // pas de sortie d'erreur si NDEBUG est definie
    initializeDebugGL();
#endif // NDEBUG

    initializeTexturesGL();
    initializeShadersGL();

    glEnable(GL_DEPTH_TEST); // Optimisation : donne un ordre de dessin au faces en fonction de leur distance à la caméra
//    glEnable(GL_CULL_FACE);  // Optimisation : pas de calcul sur les face invisible du point de vue de la caméra

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//    glClearDepth(1);  // Set background depth to farthest

//   glCullFace(GL_BACK);
//   glColor3d(0, 0, 1);

//   glCullFace(GL_FRONT);
//   glColor3d(1, 0, 0);

//   glPolygonMode(GL_FRONT, GL_LINE);

//    glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
//    glShadeModel(GL_SMOOTH);   // Enable smooth shading
//    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}

void GLWidget::resizeGL(int width, int height)
{
    projection.setToIdentity();
//    projection.perspective(45.0f, GLfloat(width) / height, 0.01f, 1000.0f);
    projection.perspective(0.0f, GLfloat(width) / height, 0.01f, 1000.0f);

}

void GLWidget::paintGL()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    bool success = false;

    success = m_shader_program->bind(); assert(success);
    {
        // Quand on bouge la caméra c'est comme si tout les objets se déplaçaient dans le sens inverse
//        QMatrix4x4 left_handed_camera_view = view.inverted(); // inversion de la matrice

        // Change le mode de vue de la caméra +z dans la direction de vue de la camera maintenant
//        left_handed_camera_view.setColumn(2, -view.column(2)); // inverse la colonne contenant les coordonées z

        m_shader_program->setUniformValue("u_mvp_matrix", projection * view * model);
        m_shader_program->setUniformValue("u_height_factor", u_height_factor);

        m_shader_program->setUniformValue("u_heightmap", heightmap->textureId());
        m_shader_program->setUniformValue("u_grass", grass->textureId());
        m_shader_program->setUniformValue("u_rock", rock->textureId());
        m_shader_program->setUniformValue("u_snowrocks", snowrocks->textureId());

        m_vao.bind();
        {
            heightmap->bind(heightmap->textureId());
            grass->bind(grass->textureId());
            rock->bind(rock->textureId());
            snowrocks->bind(snowrocks->textureId());
            glDrawElements(GL_TRIANGLES, surface->vertexes_indices.size(), GL_UNSIGNED_SHORT, 0);
        }
        m_vao.release();
    }
    m_shader_program->release();
}


//////// Event overrides


void GLWidget::keyPressEvent(QKeyEvent *e)
{
    float translate_offset = 0.1;
    float rotate_offset = 5;

    switch (e->key())
    {
    case Qt::Key::Key_Up:    rotate_speed += 0.5; update(); break;
    case Qt::Key::Key_Down:  rotate_speed = rotate_speed - 0.5 > 0 ? rotate_speed - 0.5 : 0; update(); break;
    case Qt::Key::Key_Left:  model.translate(+translate_offset, 0, 0); update(); break;
    case Qt::Key::Key_Right: model.translate(-translate_offset, 0, 0); update(); break;

    case Qt::Key::Key_I: model.rotate(+rotate_offset, 1, 0, 0); update(); break;
    case Qt::Key::Key_K: model.rotate(-rotate_offset, 1, 0, 0); update(); break;
    case Qt::Key::Key_J: model.rotate(-rotate_offset, 0, 1, 0); update(); break;
    case Qt::Key::Key_L: model.rotate(+rotate_offset, 0, 1, 0); update(); break;

    case Qt::Key::Key_U: u_height_factor -= 0.05f; update(); break;
    case Qt::Key::Key_O: u_height_factor += 0.05f; update(); break;
    }
//    qDebug() << "pressed " << e->key() << "\n";
}


void GLWidget::mousePressEvent(QMouseEvent *e)
{

//    // Save mouse press position
//    angularSpeed = 0;
//    mousePressPosition = QVector2D(e->localPos());

//    std::cerr << "MousePosition = " << QVector2D(e->localPos()) << "\n";
}

void GLWidget::mouseReleaseEvent(QMouseEvent *e)
{

//    // Mouse release position - mouse press position
//    QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

//    // Rotation axis is perpendicular to the mouse position difference
//    // vector
//    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

//    // Accelerate angular speed relative to the length of the mouse sweep
//    qreal acc = diff.length() / 100.0;

//    // Calculate new rotation axis as weighted sum
//    rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

//    // Increase angular speed
//    angularSpeed += acc;
    emit clicked();
}

void GLWidget::mouseMoveEvent(QMouseEvent *e)
{

//    if (e->buttons() & Qt::LeftButton)
//    {
//        // Mouse release position - mouse press position
//        QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

//        // Rotation axis is perpendicular to the mouse position difference
//        // vector
//        rotationAxis = QVector3D(diff.y(), diff.x(), 0.0).normalized();
//        angularSpeed += diff.length()/100;

//        rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;

//        mousePressPosition = QVector2D(e->localPos());
//        update();
//    }

}

void GLWidget::timerEvent(QTimerEvent *e)
{

//    // Decrease angular speed (friction)
//    angularSpeed *= 0.99;

//    // Stop rotation when speed goes below threshold
//    if (angularSpeed < 0.01) {
//        angularSpeed = 0.0;
//    } else {
//        // Update rotation
//        rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;

//        QMatrix4x4 rot;
//        rot.rotate(rotation);
//        model = rot;

//        // Request an update
//        update();
//    }
    view.rotate(rotate_speed, {0, 1, 0});
    update();
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(300, 300);
}

//////// Debug functions


void GLWidget::printVersionInformation()
{
  QString glType;
  QString glVersion;
  QString glProfile;

  // Get OpenGL version
  glType = (context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
  glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

  // Get OpenGL profile info
#define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
  switch (format().profile())
  {
    CASE(NoProfile);
    CASE(CoreProfile);
    CASE(CompatibilityProfile);
  }
#undef CASE

  qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";
}

void GLWidget::messageLogged(const QOpenGLDebugMessage &msg)
{
    QString error;

    // Format based on severity
    switch (msg.severity())
    {
    case QOpenGLDebugMessage::NotificationSeverity:
        error += "--";
        break;
    case QOpenGLDebugMessage::HighSeverity:
        error += "!!";
        break;
    case QOpenGLDebugMessage::MediumSeverity:
        error += "!~";
        break;
    case QOpenGLDebugMessage::LowSeverity:
        error += "~~";
        break;
    }

    error += " (";

    // Format based on source
#define CASE(c) case QOpenGLDebugMessage::c: error += #c; break
    switch (msg.source())
    {
    CASE(APISource);
    CASE(WindowSystemSource);
    CASE(ShaderCompilerSource);
    CASE(ThirdPartySource);
    CASE(ApplicationSource);
    CASE(OtherSource);
    CASE(InvalidSource);
    }
#undef CASE

    error += " : ";

    // Format based on type
#define CASE(c) case QOpenGLDebugMessage::c: error += #c; break
    switch (msg.type())
    {
    CASE(ErrorType);
    CASE(DeprecatedBehaviorType);
    CASE(UndefinedBehaviorType);
    CASE(PortabilityType);
    CASE(PerformanceType);
    CASE(OtherType);
    CASE(MarkerType);
    CASE(GroupPushType);
    CASE(GroupPopType);
    }
#undef CASE

    error += ")";

    qDebug() << qPrintable(error) << "\n" << qPrintable(msg.message()) << "\n";
}
