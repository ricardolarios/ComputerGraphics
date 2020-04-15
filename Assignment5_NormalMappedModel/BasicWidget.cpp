#pragma once
#include "BasicWidget.h"
#include <iostream>

//////////////////////////////////////////////////////////////////////
// Publics
BasicWidget::BasicWidget(QWidget* parent) : QOpenGLWidget(parent), logger_(this)
{
    setFocusPolicy(Qt::StrongFocus);
    this->is_wireframe_mode_ = false;
    camera_.setPosition(QVector3D(0.5, 0.5, -2.0));
    camera_.setLookAt(QVector3D(0.5, 0.5, 0.0));
    world_.setToIdentity();
}

BasicWidget::~BasicWidget()
{
    for (auto renderable : renderables_)
    {
        delete renderable;
    }
    renderables_.clear();
}

///////////////////////////////////////////////////////////////////////
// Protected
void BasicWidget::keyReleaseEvent(QKeyEvent* keyEvent)
{
  // TODO: MAKE WIREFRAME WORK CORRECTLY
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
    // Change the axes of rotation! TODO: Maybe in future, update to take a vector from cin.
    else if (keyEvent->key() == Qt::Key_X)
    {
        for (auto renderable : renderables_)
        {
            renderable->setRotationAxis(QVector3D(1.0, 0.0, 0.0));
            renderable->setRotationAngle(0.0);
        }
    }
    else if (keyEvent->key() == Qt::Key_Y)
    {
        for (auto renderable : renderables_)
        {
            renderable->setRotationAxis(QVector3D(0.0, 1.0, 0.0));
            renderable->setRotationAngle(0.0);
        }
    }
    else if (keyEvent->key() == Qt::Key_Z)
    {
        for (auto renderable : renderables_)
        {
            renderable->setRotationAxis(QVector3D(0.0, 0.0, 1.0));
            renderable->setRotationAngle(0.0);
        }
    }
    else
    {
        qDebug() << "You Pressed an unsupported Key!";
    }
  // ENDTODO
}

void BasicWidget::mousePressEvent(QMouseEvent* mouseEvent)
{
    if (mouseEvent->button() == Qt::LeftButton)
    {
        mouseAction_ = Rotate;
    }
    else if (mouseEvent->button() == Qt::RightButton)
    {
        mouseAction_ = Zoom;
    }
    lastMouseLoc_ = mouseEvent->pos();
}

void BasicWidget::mouseMoveEvent(QMouseEvent* mouseEvent)
{
    if (mouseAction_ == NoAction)
    {
        return;
    }
    QPoint delta = mouseEvent->pos() - lastMouseLoc_;
    lastMouseLoc_ = mouseEvent->pos();
    if (mouseAction_ == Rotate)
    {
        for (auto renderable : renderables_)
        {
            // TODO: i guess update is fine, maybe make a rotate method instead?
            renderable->update(delta.x());
        }
    }
    else if (mouseAction_ == Zoom)
    {
        // TODO:  Implement zoom by moving the camera
        // Zooming is moving along the gaze direction by some amount.
        this->camera_.zoom(delta.y());
          //this->camera_.setPosition(this->camera_.position() - );
    }
    update();
}

void BasicWidget::mouseReleaseEvent(QMouseEvent* mouseEvent)
{
    mouseAction_ = NoAction;
}

void BasicWidget::initializeGL()
{
    makeCurrent();
    initializeOpenGLFunctions();
    // Get path from std::in
    std::cout << "Please input the path to your obj file." << std::endl;
    std::string path;
    std::cin >> path;
    Renderable* ren = new Renderable(path);

    this->renderables_.push_back(ren);
    frameTimer_.start();

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

    camera_.setPerspective(70.f, (float)w / (float)h, 0.001, 1000.0);
    glViewport(0, 0, w, h);
}


void BasicWidget::paintGL()
{
    qint64 msSinceRestart = frameTimer_.restart();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // TODO:  render.
    for (auto renderable : renderables_)
    {
        renderable->draw(world_, camera_.getViewMatrix(), camera_.getProjectionMatrix());
    }

    update();
}
