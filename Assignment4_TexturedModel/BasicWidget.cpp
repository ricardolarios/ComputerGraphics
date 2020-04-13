#pragma once
#include "BasicWidget.h"
#include <iostream>

// I'm storing these here, because if I try to create them in the constructor or elsewhere, 
// it doesn't seem to create them properly, and can't parse it at all. Don't know why!
// Was originally planning on storing a map of these parsers and setting the curr_key to different ones based
// on key clicks (i.e. 1 would go to bunny, 2 to monkey, etc.). 
//ObjFileParser* house = new ObjFileParser("../objects/house/house_obj.obj");
//ObjFileParser* bunny = new ObjFileParser("../objects/bunny.obj");
//ObjFileParser* monkey = new ObjFileParser("../objects/monkey.obj");
//ObjFileParser* cube = new ObjFileParser("../objects/cube.obj");

//////////////////////////////////////////////////////////////////////
// Publics
BasicWidget::BasicWidget(QWidget* parent) : QOpenGLWidget(parent), logger_(this)
{
    this->is_wireframe_mode_ = false;
    //this->curr_obj_ = house;
    setFocusPolicy(Qt::StrongFocus);
}

BasicWidget::~BasicWidget()
{
    for (auto renderable : renderables_)
    {
        delete renderable;
    }
    renderables_.clear();
}

//////////////////////////////////////////////////////////////////////
// Privates

QString BasicWidget::vertexShaderString() const
{
    QString str =
        "#version 330\n"
        "layout(location = 0) in vec3 position;\n"
        "layout(location = 1) in vec4 color;\n"
        "out vec4 vertColor;\n"
        "void main()\n"
        "{\n"
        "  gl_Position = vec4(position, 1.0);\n"
        "  vertColor = color;\n"
        "}\n";
    return str;
}

QString BasicWidget::fragmentShaderString() const
{
    QString str =
        "#version 330\n"
        "in vec4 vertColor;\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "  color = vertColor;\n"
        "}\n";
    return str;
}


void BasicWidget::createShader()
{
    QOpenGLShader vert(QOpenGLShader::Vertex);
    vert.compileSourceCode(vertexShaderString());
    QOpenGLShader frag(QOpenGLShader::Fragment);
    frag.compileSourceCode(fragmentShaderString());
    bool ok = shaderProgram_.addShader(&vert);
    if (!ok)
    {
        qDebug() << shaderProgram_.log();
    }
    ok = shaderProgram_.addShader(&frag);
    if (!ok)
    {
        qDebug() << shaderProgram_.log();
    }
    ok = shaderProgram_.link();
    if (!ok)
    {
        qDebug() << shaderProgram_.log();
    }
}

///////////////////////////////////////////////////////////////////////
// Protected
void BasicWidget::keyReleaseEvent(QKeyEvent* keyEvent)
{
  // TODO
  // Handle key events here.
    //if (keyEvent->key() == Qt::Key_1)
    //{
    //    // Second
    //    this->curr_obj_ = bunny;
    //    this->initialize_parser();
    //    update();  // We call update after we handle a key press to trigger a redraw when we are ready
    //}
    //else if (keyEvent->key() == Qt::Key_2)
    //{
    //    // second solution
    //    this->curr_obj_ = monkey;
    //    this->initialize_parser();
    //    update();  // We call update after we handle a key press to trigger a redraw when we are ready
    //}
    //else if (keyEvent->key() == Qt::Key_3)
    //{
    //    this->curr_obj_ = cube;
    //    this->initialize_parser();
    //    update();
    //}
    //else if (keyEvent->key() == Qt::Key_4)
    //{
    //    this->curr_obj_ = house;
    //    this->initialize_parser();
    //    update();
    //}
    if (keyEvent->key() == Qt::Key_W)
    {
        // I'm choosing to have the W key toggle between wireframe and non-wireframe.
        this->is_wireframe_mode_ 
            ? glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) 
            : glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        this->is_wireframe_mode_ = !this->is_wireframe_mode_;
        update();
    }
    else if (keyEvent->key() == Qt::Key_Q)
    {
        qDebug() << "Q key hit, quitting program.";
        exit(0);
    }
    else
    {
        qDebug() << "You Pressed an unsupported Key!";
    }
  // ENDTODO
}
void BasicWidget::initializeGL()
{
    makeCurrent();
    initializeOpenGLFunctions();

    Renderable* ren = new Renderable("../../objects/house/house_obj.obj");

    this->renderables_.push_back(ren);
    //this->initialize_parser();

    glViewport(0, 0, width(), height());
}

void BasicWidget::resizeGL(int w, int h)
{
    if (!logger_.isLogging())
    {
        logger_.initialize();
        // Setup the logger for real-time messaging
        connect(&logger_, &QOpenGLDebugLogger::messageLogged, [=]()
            {
                const QList<QOpenGLDebugMessage> messages = logger_.loggedMessages();
                for (auto msg : messages)
                {
                    qDebug() << msg;
                }
            });
        logger_.startLogging();
    }
    glViewport(0, 0, w, h);
    view_.setToIdentity();
    view_.lookAt(QVector3D(0.0f, 0.0f, 2.0f),
        QVector3D(0.0f, 0.0f, 0.0f),
        QVector3D(0.0f, 1.0f, 0.0f));
    projection_.setToIdentity();
    projection_.perspective(70.f, (float)w / (float)h, 0.001, 1000.0);
    glViewport(0, 0, w, h);
}


void BasicWidget::paintGL()
{
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // TODO:  render.
  for (auto renderable : renderables_)
  {
      renderable->draw(view_, projection_);
  }

  update();
}
