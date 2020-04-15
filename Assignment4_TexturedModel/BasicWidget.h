#pragma once

#include <QtGui>
#include <QtWidgets>
#include <QtOpenGL>
#include "ObjFileParser.h"
#include "Renderable.h"
#include "Camera.h"

/**
 * This is just a basic OpenGL widget that will allow a change of background color.
 *
 * Removed the buffers from this widget and instead put that responsibility on the parser objects.
 */
class BasicWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

private:
    QMatrix4x4 world_;
    Camera camera_;

    bool is_wireframe_mode_;

    QMatrix4x4 model_;
    QMatrix4x4 view_;
    QMatrix4x4 projection_;

    QVector<Renderable*> renderables_;

    QOpenGLDebugLogger logger_;
    QElapsedTimer frameTimer_;

    // Mouse controls.
    enum MouseControl { NoAction = 0, Rotate, Zoom };
    QPoint lastMouseLoc_;
    MouseControl mouseAction_;

protected:
    // Required interaction overrides
    void keyReleaseEvent(QKeyEvent* keyEvent) override;
    void mousePressEvent(QMouseEvent* mouseEvent) override;
    void mouseMoveEvent(QMouseEvent* mouseEvent) override;
    void mouseReleaseEvent(QMouseEvent* mouseEvent) override;

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
