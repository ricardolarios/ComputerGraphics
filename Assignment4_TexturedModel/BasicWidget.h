#pragma once

#include <QtGui>
#include <QtWidgets>
#include <QtOpenGL>
#include "ObjFileParser.h"
#include "Renderable.h"

/**
 * This is just a basic OpenGL widget that will allow a change of background color.
 *
 * Removed the buffers from this widget and instead put that responsibility on the parser objects.
 */
class BasicWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

private:
    QString vertexShaderString() const;
    QString fragmentShaderString() const;
    void createShader();
    ObjFileParser* curr_obj_;
    // Map of file names to the ObjFileParser that parsed that file. Allows reuse without reading file again.
    //std::map<const char*, ObjFileParser*> possible_obj_;
    //char* curr_key_; // key to the current ObjFileParser we are using to render.
    bool is_wireframe_mode_;

    QMatrix4x4 model_;
    QMatrix4x4 view_;
    QMatrix4x4 projection_;

    QVector<Renderable*> renderables_;

    QOpenGLDebugLogger logger_;


protected:
    // Required interaction overrides
    void keyReleaseEvent(QKeyEvent* keyEvent) override;

    // Required overrides form QOpenGLWidget
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    QOpenGLShaderProgram shaderProgram_;

public:
    BasicWidget(QWidget* parent = nullptr);
    virtual ~BasicWidget();

    // Make sure we have some size that makes sense.
    QSize sizeHint() const { return QSize(800, 600); }
};
