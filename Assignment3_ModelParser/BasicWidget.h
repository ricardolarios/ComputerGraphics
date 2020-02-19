#pragma once

#include <QtGui>
#include <QtWidgets>
#include <QtOpenGL>
#include "ObjFileParser.h"

/**
 * This is just a basic OpenGL widget that will allow a change of background color.
 */
class BasicWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

private:
    QString vertexShaderString() const;
    QString fragmentShaderString() const;
    void createShader();
    QOpenGLVertexArrayObject vao_;
    QOpenGLVertexArrayObject vao_m_;
    ObjFileParser* curr_obj_;
    // Map of file names to the ObjFileParser that parsed that file. Allows reuse without reading file again.
    //std::map<const char*, ObjFileParser*> possible_obj_;
    //char* curr_key_; // key to the current ObjFileParser we are using to render.
    bool is_wireframe_mode_;

    // Helper, initializes the buffers that are stored in the current ObjFileParser using the current GL context.
    void initialize_parser();

protected:
    // Required interaction overrides
    void keyReleaseEvent(QKeyEvent* keyEvent) override;

    // Required overrides form QOpenGLWidget
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    QOpenGLBuffer vbo_;
    QOpenGLBuffer nbo_; // for normals
    QOpenGLBuffer ibo_;
    QOpenGLBuffer nibo_; // normal indices buffer
    QOpenGLBuffer vbo_m_;
    QOpenGLBuffer nbo_m_; // for normals
    QOpenGLBuffer ibo_m_;
    QOpenGLBuffer nibo_m_; // normal indices buffer
    //QOpenGLBuffer cbo_; // unused, using vbo_
    QOpenGLShaderProgram shaderProgram_;

public:
    BasicWidget(QWidget* parent = nullptr);
    virtual ~BasicWidget();

    // Make sure we have some size that makes sense.
    QSize sizeHint() const { return QSize(800, 600); }
};
