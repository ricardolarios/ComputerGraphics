#pragma once

#include <QtGui/QMatrix4x4>
#include <QtGui/QVector3D>
#include <QtGui/qvector2d.h>

class Camera
{
protected:
	QVector3D position_;
	QVector3D lookAt_;
	QVector3D up_;
	QMatrix4x4 projection_;
	double current_angle_;

public:
	Camera();
	virtual ~Camera();

	// Manipulate our Camera
	void setPerspective(float fov, float aspect, float near, float far);

	// Move our position.
	void setPosition(const QVector3D& position);
	QVector3D position() const;
	void translateCamera(const QVector3D& delta);

	// Move our gaze
	void setGazeVector(const QVector3D& gaze);
	QVector3D gazeVector() const;
	QVector3D upVector() const;
	void setLookAt(const QVector3D& lookAt);
	void translateLookAt(const QVector3D& delta);
	QVector3D lookAt() const;

	// Rotates the camera around the look at point in a circle
	void rotateAroundLookAt(const double angle);

	// Zooms the camera position along the gaze vector based on the given delta.
	void zoom(const double delta);

	// Get our camera matrix
	QMatrix4x4 getViewMatrix() const;
	QMatrix4x4 getProjectionMatrix() const;

private:

};